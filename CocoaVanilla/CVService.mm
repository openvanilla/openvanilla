// CVService.mm: Cocoa-Vanilla implementation of OVService
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <Carbon/Carbon.h>
#include "CVLoaderUtility.h"
#include "CVService.h"
#import "OVDisplayServer.h"

CFStringEncoding CVGetCFEncoding(const char *encoding);
NSString *CVGetAppBundleLocale();

CFStringEncoding CVGetCFEncoding(const char *encoding) {
    if (!strcasecmp(encoding, "big5")) {
        return kCFStringEncodingBig5_HKSCS_1999;
    }
    
    return kCFStringEncodingUTF8;
}

NSString *CVGetAppBundleLocale() {
    NSBundle *b=[NSBundle mainBundle];
    if (!b) return @"en";
    
    NSArray *a=[b preferredLocalizations]; 
    if (!a) return @"en";
    if (![a count]) return @"en";
    
    NSString *s=[a objectAtIndex:0];
    NSString *l=(NSString*)CFLocaleCreateCanonicalLocaleIdentifierFromString(NULL, (CFStringRef)s);
    if (!l) return @"en";
    [l autorelease];
    return l;
}

CVService::CVService(NSString *usrpath, id displayserver) {
    u8buf=NULL;
    u16buf=NULL;
    userbuf=NULL;
    
    lctag=CVGetAppBundleLocale();
    [lctag retain];
    
	dspsrvr=displayserver;
	notifypos=(Point){0, 0};
    userspace=[usrpath stringByStandardizingPath];
    [userspace retain];
    
    shouldbeep=1;
    beepsound=[[NSString alloc] initWithString:@""];
}

CVService::~CVService() {
    if (u8buf) CFRelease(u8buf);
    if (u16buf) free(u16buf);
    if (userbuf) free(userbuf);
    [lctag release];
    [userspace release];
    [beepsound release];
}

void CVService::beep() {
    if (!shouldbeep) return;
    
    if (![beepsound length]) {
        // SysBeep(30);
    }
    else {
        NSSound *s=[NSSound soundNamed:beepsound];
        if (s) [s retain]; else s=[[NSSound alloc] initWithContentsOfFile:beepsound byReference:YES];        
        if (s) {
            [s play];
            [s release];
        }
        else ; // SysBeep(30);
    }
}

void CVService::setShouldBeep(int s) {
    shouldbeep=s;
}

void CVService::setBeepSound(NSString *s) {
    [beepsound release];
    beepsound=[[NSString alloc] initWithString:s];
}

void CVService::notify(const char* msg) {
    // if notifypos.h==0, we don't notify --
    // usually this means an app has started IM service but
    // hasn't displayed the cursor yet
    if (!notifypos.h) return;
	[dspsrvr notifyMessage:[NSString stringWithUTF8String:msg] position:notifypos];
}

void CVService::setNotificationPosition(Point p) {
	notifypos=p;
}

void CVService::closeNotification() {
	[dspsrvr notifyClose];
}

void CVService::fadeNotification() {
    if (!notifypos.h)
        closeNotification();
    else
        [dspsrvr notifyFade];
}

const char *CVService::locale() {
    return [lctag UTF8String];
}

const char *CVService::userSpacePath(const char *modid) {
	NSString *usp=[NSString stringWithFormat:@"%@/%@/",
		userspace, [NSString stringWithUTF8String:modid]];

    if (!CVIfPathExists(usp)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", usp]);
		system([[NSString stringWithFormat:@"mkdir -p %@", usp] UTF8String]);
	}

    return [usp UTF8String];
}

const char *CVService::pathSeparator() {
    return "/";
}

const char *CVService::toUTF8(const char *encoding, const char *src) {
    if (u8buf) {
        CFRelease(u8buf);
        u8buf=NULL;
    }
    u8buf=CFStringCreateWithCString(NULL, src, CVGetCFEncoding(encoding));
    if (u8buf) return [(NSString*)u8buf UTF8String];
    return NULL;
}

const char *CVService::fromUTF8(const char *encoding, const char *src) {
    if (userbuf) {
        free(userbuf); 
        userbuf=NULL;
    }
    userbuf=(char*)calloc(1, strlen(src)*2+1);
    if (!userbuf) return NULL;
    if (u8buf) {
        CFRelease(u8buf);
        u8buf=NULL;
    }
    u8buf=(CFStringRef)[[NSString alloc] initWithUTF8String:src];
    if (!u8buf) return NULL;
    if (CFStringGetCString(u8buf, userbuf, strlen(src)*2+1, CVGetCFEncoding(encoding))) {
        return userbuf;
    }
    return NULL;
}

const char *CVService::UTF16ToUTF8(unsigned short *src, int len) {
    if (u8buf) {
        CFRelease(u8buf);
        u8buf=NULL;
    }
    u8buf=(CFStringRef)[[NSString alloc] initWithCharacters:(UniChar*)src length:len];
    if (!u8buf) return NULL;
    return [(NSString*)u8buf UTF8String];
}

int CVService::UTF8ToUTF16(const char *src, unsigned short **rcvr) {
    if (u16buf) {
        free(u16buf);
        u16buf=NULL;
    }
    if (u8buf) {
        CFRelease(u8buf);
        u8buf=NULL;
    }
    *rcvr=NULL;
    NSString *u8=[[NSString alloc] initWithUTF8String:src];
    u8buf=(CFStringRef)u8;
    if (!u8buf) return 0;
    
    int l=[u8 length];
    u16buf=(UniChar*)calloc(1, l*sizeof(UniChar));
    if (!u16buf) return 0; 
    [u8 getCharacters:u16buf]; 
    *rcvr=u16buf;
    return l;
}
