// OpenVanilla.h: The Framework
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

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

#define OV_VERSION   (0x00080000)      // version 0.8.0 (00_08_00_00)


class OVBase {
public:	
    virtual ~OVBase() {}
};

class OVKeyCode : public OVBase  {
public:
	
    virtual int code() = 0;
    virtual bool isShift() = 0;
    virtual bool isCapslock() = 0;

    virtual bool isCtrl() = 0;
    virtual bool isAlt() = 0;
    virtual bool isOpt() { return isAlt(); }
    virtual bool isCommand() { return isAlt(); }
    virtual bool isNum() = 0;
    virtual bool isFunctionKey()
    {
        return isCtrl() || isAlt() || isOpt() || isCommand() || isNum();
    }
};

class OVBuffer : public OVBase {
public:	
    virtual OVBuffer* clear() = 0;	
    virtual OVBuffer* append(const char *s) = 0;
    virtual OVBuffer* send() = 0;
    virtual OVBuffer* update() = 0;
    virtual OVBuffer* update(int cursorPos, int markFrom = -1, int markTo = -1) = 0;
    virtual bool isEmpty() = 0;
};

class OVCandidate : public OVBase  {
public:
	
    virtual OVCandidate* clear() = 0;
    virtual OVCandidate* append(const char *s) = 0;
    virtual OVCandidate* hide() = 0;
    virtual OVCandidate* show() = 0;
    virtual OVCandidate* update() = 0;
    virtual bool onScreen() = 0;
};

class OVDictionary : public OVBase {
public:
	
    virtual bool keyExist(const char *key) = 0;
    virtual int getInteger(const char *key) = 0;
    virtual int setInteger(const char *key, int value) = 0;
    virtual const char *getString(const char *key) = 0;
    virtual const char *setString(const char *key, const char *value) = 0;
    virtual int getIntegerWithDefault(const char *key, int value)
    {
        if (!keyExist(key)) setInteger(key, value); return getInteger(key);
    }
    
    virtual const char *getStringWithDefault(const char *key, const char *value) 
    {
        if (!keyExist(key)) setString(key, value); return getString(key);
    }
};

class OVService : public OVBase {
public:
	
    virtual void beep() = 0;
    virtual void notify(const char *msg) = 0;
    virtual const char *locale() = 0 ;
    virtual const char *userSpacePath(const char *modid) = 0;
    virtual const char *pathSeparator() = 0;
    // limited support for encoding conversion
    virtual const char *toUTF8(const char *encoding, const char *src) = 0;
    virtual const char *fromUTF8(const char *encoding, const char *src) = 0;
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) = 0;
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) = 0;
};

class OVInputMethodContext : public OVBase  {
public:
	
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {}	
    virtual void clear() {}
    virtual void end() {}
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*) = 0;
};

class OVModule : public OVBase {
public:
    virtual const char *moduleType() = 0;
    virtual const char *identifier() = 0;
    virtual const char *localizedName(const char *locale) { return identifier(); }
	virtual int initialize(OVDictionary* moduleCfg, OVService* srv, const char *modulePath) { return 1; }
    virtual void update(OVDictionary* moduleCfg, OVService* srv) {}
};

class OVInputMethod : public OVModule
{
public:	
    virtual OVInputMethodContext* newContext() = 0;
    virtual const char *moduleType() { return "OVInputMethod"; }
};

class OVOutputFilter : public OVModule
{
public:	
    virtual const char *process(const char *source, OVService* srv) = 0;
    virtual const char *moduleType() { return "OVOutputFilter"; }
};
 
enum 
{
    ovkEsc =27, ovkSpace = 32, ovkReturn = 13,
    ovkDelete = 127, ovkBackspace = 8,
    ovkUp = 30, ovkDown = 31, ovkLeft = 28, ovkRight = 29,
    ovkHome = 1, ovkEnd = 4, ovkPageUp = 11, ovkPageDown = 12,
    ovkTab = 9
};
#endif
