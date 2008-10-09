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

#import <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUTF8Helper.h>
#include "LVConfig.h"
#include <string>

using namespace std;
using namespace OpenVanilla;

class LVKeyCode : public OVKeyCode {
public:
    bool capsLock;
    bool ctrl;
    bool opt;
    int keyCode;
    
public:
    LVKeyCode()
        : capsLock(false)
        , ctrl(false)
        , opt(false)
        , keyCode(0)
    {
    }

    virtual int code()
    {
        return keyCode;
    }
    
    virtual bool isShift()
    {
        return false;
    }
    
    virtual bool isCapslock()
    {
        return capsLock;
    }
    
    virtual bool isCtrl()
    {
        return ctrl;
    }
    
    virtual bool isAlt()
    {
        return opt;
    }
    
    virtual bool isNum()
    {
        return false;
    }    
};

class LVBuffer : public OVBuffer {
public:
    LVBuffer()
        : m_shouldCommit(false)
        , m_shouldUpdate(false)
        , m_cursorIndex(0)
        , m_highlightFrom(-1)
        , m_highlightTo(-1)
    {
    }
    
    virtual OVBuffer* clear()
    {
        m_composingBuffer.clear();
        m_shouldUpdate = false;
        m_cursorIndex = 0;
        m_highlightFrom = -1;
        m_highlightTo = -1;
        return this;
    }
    
    virtual OVBuffer* append(const char *s)
    {
        m_composingBuffer += s;
        return this;
    }
    
    virtual OVBuffer* send()
    {
        m_committedString += m_composingBuffer;
        m_composingBuffer.clear();
        m_shouldCommit = true;
        return this;
    }
    
    virtual OVBuffer* update()
    {
        m_shouldUpdate = true;
        vector<string> codepoints = OVUTF8Helper::SplitStringByCodePoint(m_composingBuffer);
        m_cursorIndex = codepoints.size();
        m_highlightFrom = -1;
        m_highlightTo = -1;
        return this;
    }
    
    virtual OVBuffer* update(int cursorPos, int markFrom = -1, int markTo = -1)
    {
        m_shouldUpdate = true;
        m_cursorIndex = cursorPos;
        m_highlightFrom = markFrom;
        m_highlightTo = markTo;
        return this;
    }
    
    virtual bool isEmpty()
    {
        return m_composingBuffer.length() == 0;
    }

public:
    virtual void clearUpdateState()
    {
        m_shouldUpdate = false;
    }
    
    virtual void clearCommittedString()
    {
        m_committedString.clear();
        m_shouldCommit = false;
    }
    
    virtual bool shouldUpdate()
    {
        return m_shouldUpdate;
    }
    
    virtual bool shouldCommit()
    {
        return m_shouldCommit;
    }
    
    virtual const string composingBuffer()
    {
        return m_composingBuffer;
    }
    
    virtual const string committedString()
    {
        return m_committedString;
    }
    
    virtual int cursorIndex()
    {
        return m_cursorIndex;
    }
    
    virtual int highlightFrom()
    {
        return m_highlightFrom;
    }
    
    virtual int highlightTo()
    {
        return m_highlightTo;
    }
    
protected:
    int m_cursorIndex;
    int m_highlightFrom;
    int m_highlightTo;
    bool m_shouldUpdate;
    bool m_shouldCommit;
    string m_composingBuffer;
    string m_committedString;
};

class LVCandidate : public OVCandidate  {
public:	
    LVCandidate() 
        : m_shouldUpdate(false)
        , m_visible(false)
    {
    }
    
    virtual OVCandidate* clear()
    {
        m_candidateText.clear();
        return this;
    }
    
    virtual OVCandidate* append(const char *s)
    {
        m_candidateText += s;
        return this;
    }
    
    virtual OVCandidate* hide()
    {
        m_visible = false;
        return this;
    }
    
    virtual OVCandidate* show()
    {
        m_visible = true;
        return this;
    }
    
    virtual OVCandidate* update()
    {
        m_shouldUpdate = true;
        return this;
    }
    
    virtual bool onScreen()
    {
        return m_visible;
    }

public:
    virtual const string candidateText()
    {
        return m_candidateText;
    }
    
    virtual void clearUpdateState()
    {
        m_shouldUpdate = false;
    }
    
protected:
    string m_candidateText;
    bool m_visible;
    bool m_shouldUpdate;
};

class LVDictionary : public OVDictionary {
public:
    LVDictionary(NSDictionary *dictionary)
    {
        m_dictionary = [dictionary retain];
    }
    
    ~LVDictionary()
    {
        [m_dictionary release];
    }
    
    virtual bool keyExist(const char *key)
    {
        id v = [m_dictionary objectForKey:[NSString stringWithUTF8String:key]];
        return !!v;
    }
    
    virtual int getInteger(const char *key)
    {
        id v = [m_dictionary objectForKey:[NSString stringWithUTF8String:key]];
        return [v intValue];
    }
    
    virtual int setInteger(const char *key, int value)
    {
        [m_dictionary setObject:[NSNumber numberWithInt:value] forKey:[NSString stringWithUTF8String:key]];
        return value;
    }
    
    virtual const char *getString(const char *key)
    {
        id v = [m_dictionary objectForKey:[NSString stringWithUTF8String:key]];
        return [[v description] UTF8String];
    }
    
    virtual const char *setString(const char *key, const char *value)
    {
        [m_dictionary setObject:[NSString stringWithUTF8String:value] forKey:[NSString stringWithUTF8String:key]];        
        return value;
    }
    
protected:
    NSMutableDictionary *m_dictionary;
};

class LVService : public OVService {
public:
	LVService()
		: _UTF16ShortBuffer(0)
	{
		_conversionBuffer = [NSMutableString new];
	}
	
	~LVService()
	{
		[_conversionBuffer release];
	}
	
    virtual void beep()
    {
        NSBeep();
    }
    
    virtual void notify(const char *msg)
    {
        NSLog(@"notify message: %s", msg);
    }
    
    virtual const char *locale()
    {
        return "en";
    }
    
    virtual const char *userSpacePath(const char *modid);
    
    virtual const char *pathSeparator()
    {
        return "/";
    }

    virtual const char *toUTF8(const char *encoding, const char *src)
	{
	    #warning finish this
        return src;
	}
    virtual const char *fromUTF8(const char *encoding, const char *src)
	{		
	    #warning finish this
        return src;
	}
	
    virtual const char *UTF16ToUTF8(unsigned short *src, int len)
	{
		wstring tmp(len, 0);
		for (int i = 0 ; i < len ; i++) {
			tmp[i] = src[i];
		}
		
		_UTF8Buffer = OVUTF8::FromUTF16(tmp);
		return _UTF8Buffer.c_str();
	}
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr)
	{
		// dangerous		
		_UTF16Buffer = OVUTF16::FromUTF8(src);
		
		_UTF16ShortBuffer = (unsigned short *)realloc(_UTF16ShortBuffer, (_UTF16Buffer.length() + 1) * sizeof(unsigned short));
		size_t i;
		for (i = 0 ; i < _UTF16Buffer.length() ; i++) {
			_UTF16ShortBuffer[i] = _UTF16Buffer[i];
		}
		
		_UTF16ShortBuffer[i] = 0;
		
		*rcvr = _UTF16ShortBuffer;
		return _UTF16Buffer.length();
	}
	
protected:
	unsigned short *_UTF16ShortBuffer;
	wstring _UTF16Buffer;
	string _UTF8Buffer;
	NSMutableString *_conversionBuffer;
};

