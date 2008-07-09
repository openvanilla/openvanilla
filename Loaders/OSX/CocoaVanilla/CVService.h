// CVService.h: Cocoa-Vanilla implementation of OVService
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __CVService_h
#define __CVSerivce_h

#include <OpenVanilla/OpenVanilla.h>

class CVService : public OVService {
public:
    CVService(NSString *usrpath, id displayserver);
    virtual ~CVService();
    virtual void beep();
    virtual void notify(const char *msg);
    virtual const char *locale();
    virtual const char *userSpacePath(const char *modid);
    virtual const char *pathSeparator();
    virtual const char *toUTF8(const char *encoding, const char *src);
    virtual const char *fromUTF8(const char *encoding, const char *src);
    virtual const char *UTF16ToUTF8(unsigned short *src, int len);
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr);
    
    virtual void closeNotification();
    virtual void fadeNotification();
    virtual void setNotificationPosition(Point p);
    
    virtual void setShouldBeep(int s);
    virtual void setBeepSound(NSString *s);
	
	virtual void changeDisplayServer(id newServer);	
protected:
    CFStringRef u8buf;
    char *userbuf;
    UniChar *u16buf;
    
    NSString *lctag;
    NSString *userspace;
	id dspsrvr;
	Point notifypos;
    
    int shouldbeep;
    NSString *beepsound;
};

#endif
