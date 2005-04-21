// CVService.mm
#include <Carbon/Carbon.h>
#include "CVService.h"

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

CVService::CVService(NSString *usrpath, CVInfoBox *ib) {
    u8buf=NULL;
    u16buf=NULL;
    userbuf=NULL;
    
    lctag=CVGetAppBundleLocale();
    [lctag retain];
    
    infobox=ib;
    userspace=[usrpath stringByStandardizingPath];
    [userspace retain];
}

CVService::~CVService() {
    if (u8buf) CFRelease(u8buf);
    if (u16buf) free(u16buf);
    if (userbuf) free(userbuf);
    [lctag release];
    [userspace release];
}

void CVService::beep() {
    SysBeep(30);
}

void CVService::notify(const char* msg) {
    [infobox clear];
    [infobox append:[NSString stringWithUTF8String:msg]];
    [infobox update];
    [infobox show];
}

void CVService::setNotificationPosition(Point p) {
    [infobox setPosition:p];
}

void CVService::closeNotification() {
    [infobox hide];
}

const char *CVService::locale() {
    return [lctag UTF8String];
}

const char *CVService::userSpacePath(const char *modid) {
    fprintf (stderr, "CVService: not fully implemented, requesting user space for module %s\n", modid);
    
    NSString *usp=[userspace stringByAppendingString:@"/"];
    fprintf (stderr, "CVService: currently returning %s\n", [usp UTF8String]);
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
