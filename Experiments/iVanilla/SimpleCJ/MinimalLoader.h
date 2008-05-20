// This code is experimental and does not guarantee it will run.
// No build instruction is given. It might harm your system.
//
// Copyright (c) 2007-2008 Yao-Chung Chang and Lukhnos D. Liu
// Released under the same terms under which OpenVanilla is released 
// (The New BSD License)
//
// See LICENSE.TXT under OpenVanilla's source repository trunk/License/
// for the full text of the New BSD License.
//

#ifndef MinimalLoader_h
#define MinimalLoader_h

#include "OpenVanilla.h"
#include "OVUtility.h"
#include <string>
#include <vector>

using namespace std;

void MLInitialize(int moduleID = 0);
void MLStart();
void MLCancel();
void MLEnd();
bool MLKey(int);
bool MLIsCommited();
string MLCommitedString();
string MLComposingBuffer();
string MLCandidatesString();

class MLKeyCode : public OVKeyCode {
public:
    MLKeyCode(int c) : m_code(c)
    {
    }
    
	virtual int code()
	{
		return m_code;
	}
	
    virtual int isShift()
	{
		return 0;
	}
	
    virtual int isCapslock()
	{
		return 0;
	}
	
    virtual int isCtrl()
	{
		return 0;
	}
	
    virtual int isAlt()
	{
		return 0;
	}
	
    virtual int isNum()
	{
		return 0;
	}
    
protected:
	int m_code;
};

class MLBuffer : public OVBuffer {
public:
    MLBuffer() : m_isCommitted(false)
    {
    }
    
    virtual bool isCommited()
    {
        return m_isCommitted;
    }
    
    virtual string bufferString()
    {
        return string(m_buffer);
    }
    
    virtual OVBuffer* clear()
	{
		m_buffer = "";
		m_isCommitted = false;
		return this;
	}
	
    virtual OVBuffer* append(const char *s)
    {
        m_isCommitted = false;
        m_buffer += s;
        return this;
    }
    
    virtual OVBuffer* send()
    {
        m_isCommitted = true;
        return this;      
    }
    
    virtual OVBuffer* update()
    {
        m_isCommitted = false;
        return this;
    }
    
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1)
    {
        m_isCommitted = false;
        return this;
    }
    
    virtual int isEmpty()
    {
        return (m_buffer.length() > 0) ? 0 : 1;
    }

protected:
	string m_buffer;
	bool m_isCommitted;
};

class MLCandidate : public OVCandidate {
public:
    MLCandidate() : m_isOnScreen(false)
    {
    }
    
    virtual string candidatesString()
    {
        return string(m_candidates);
    }
    
    virtual OVCandidate* clear()
    {
        m_candidates = "";
		return this;
    }
    
    virtual OVCandidate* append(const char *s)
    {
        m_candidates += s;
        return this;
    }
    
    virtual OVCandidate* hide()
    {
        m_isOnScreen = false;
        return this;
    }
    
    virtual OVCandidate* show()
    {
        m_isOnScreen = true;
        return this;
    }
    
    virtual OVCandidate* update()
    {
        return this;
    }
    
    virtual int onScreen()
    {
        return m_isOnScreen ? 1 : 0;
    }
    
protected:
    string m_candidates;
    bool m_isOnScreen;
};

class MLDictionary : public OVDictionary {
public:
    virtual int keyExist(const char *key)
    {
		
        return 0;
    }
    
    virtual int getInteger(const char *key)
    {
		return atoi(getString(key));
    }
    
    virtual int setInteger(const char *key, int value)
    {
		char buf[256];
		snprintf(buf, 255, "%d", value);
		setString(key, buf);
		return value;
    }
    
    virtual const char* getString(const char *key)
    {
		int i = indexForKey(key);
		if (i == -1)
        	return "";

		return m_dictionary[i].second.c_str();
    }
    
    virtual const char* setString(const char *key, const char *value)
    {
		int i = indexForKey(key);
		if (i == -1)
			m_dictionary.push_back(pair<string, string>(string(key), string(value)));
		else
			m_dictionary[i].second = value;
			
        return value;
    }

protected:
	virtual int indexForKey(const char *key)
	{
		size_t size = m_dictionary.size();
		for (size_t i = 0; i < size; i++) {
			if (m_dictionary[i].first == key) return (int)i;
		}
		
		return -1;
	}
	
	vector<pair<string, string> > m_dictionary;
};

class MLService : public OVService {
public:
    virtual void beep()
    {
    }
    
    virtual void notify(const char *msg)
    {
    }
    
    virtual const char *locale()
    {
        return "en";
    }
    
    virtual const char *userSpacePath(const char *modid)
    {
        return "/tmp/";
    }
    
    virtual const char *pathSeparator()
    {
        return "/";
    }

    // limited support for encoding conversion
    virtual const char *toUTF8(const char *encoding, const char *src)
    {
        return src;
    }
    
    virtual const char *fromUTF8(const char *encoding, const char *src)
    {
        return src;
    }
    
    virtual const char *UTF16ToUTF8(unsigned short *src, int len)
    {
        return "";
    }
        
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr)
    {
        return 0;
    }
};

#endif