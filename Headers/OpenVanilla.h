// OpenVanilla.h: Defines the OpenVanilla IM Framework
//
// This header file is used both by IM modules and Loader implementations.
//
// Copyright (c) 2004 by Lukhnos D. Liu (lukhnos at openvanilla.org), 
// Kang-ming Liu (gugod at gugod.org)

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

const unsigned int ovVersion=0x00700000;    // for 0.7

class OVObject
{
public:
    virtual ~OVObject() {}
};

typedef int OVEncoding;
typedef int OVException;

enum
{
    oveUTF8=0,
    oveUTF16Auto=1,     // if you're using uint16 to store UTF-16 codepoints
    oveUTF16BE=2,
    oveUTF16LE=3,
    oveBig5=0x10,
    oveBig5HKSCS=0x10,
    oveEUC_CN=0x20
};

class OVBuffer : public OVObject
{
public:
    virtual OVBuffer* clear()=0;
    virtual OVBuffer* send()=0; 
    virtual OVBuffer* update(int cursorpos=-1, int hilightfrom=-1, 
        int hilightto=-1)=0;
    virtual OVBuffer* append (const void *s, OVEncoding e=oveUTF8, int len=0)=0;
	virtual OVBuffer* appendChar(char c)
        { char buf[2]; buf[0]=c; buf[1]=0; return append(buf); }
    virtual int length()=0;
};


class OVService : public OVObject
{
public:
    virtual OVService* beep()=0;
    virtual OVService* clearTextMsg()=0;
    virtual OVService* showTextMsg(const void *str, OVEncoding enc=oveUTF8, 
        int len=0)=0;
};


class OVCandidateBox : public OVObject
{
public:
    // "high-level" functions
    virtual OVCandidateBox* clear(int hide=0);
    virtual OVCandidateBox* append(const void *str, OVEncoding enc=oveUTF8,
        int len=0)=0;
    virtual OVCandidateBox* show();
    virtual OVCandidateBox* finalize();
    
    // "low-level" functions, use these only if you want to process candidateEvent
    virtual OVCandidateBox* hide();
    virtual OVCandidateBox* update();
    virtual OVCandidateBox* send(int index, OVBuffer *buf);
    virtual int onScreen();

    // this is usually called by the Loader; IM doesn't need to call it
    virtual int setSelectKey(const char *selkey);
    
    // called by the Loader after calling current IM's candidateEvent
    virtual int candidateEvent(OVKey *key, OVBuffer *buf, OVService *srv);
};


class OVList : public OVObject
{
public:
    virtual int count()=0;
    virtual int getInt(int index)=0;
    virtual int getString(int index, char *str, OVEncoding enc=oveUTF8, int 
        maxlen=0)=0;
};

class OVDictionary : public OVObject
{
public:
    virtual int keyExist(const char *key, OVEncoding enc=oveUTF8)=0;
    virtual int setInt(const char *key, int value, OVEncoding enc=oveUTF8)=0;
    virtual int getInt(const char *key, OVEncoding enc=oveUTF8)=0;
    virtual char* setString(const char *key, const char *value, 
        OVEncoding enc=oveUTF8);
    virtual int getString(const char *key, char *str, OVEncoding enc=oveUTF8, 
        int maxlen=0)=0;
    virtual int newDictionary(const char *key, int replaceIfExist=0);
   
    // these objects must be deleted by the caller
    OVList* getList(const char *key, OVEncoding enc=oveUTF8)=0;
    OVDictionary* getDictionary(char *key, OVEncoding enc=oveUTF8)=0;

    // we implement this two functions
    virtual int getIntDefault(const char *key, int defvalue, 
        OVEncoding enc=oveUTF8)=0;
    virtual int getStringDefault(const char *key, char *str, 
        const void *defvalue, OVEncoding enc=oveUTF8, int maxlen=0)=0;
};

// IM keyEvent handler and context data goes here.
class OVIMContext : public OVObject
{
public:
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, 
        OVCandidateBox *candi, OVService *srv)
    {
        return 0;
    }
};


class OVComponent : public OVObject
{
public:
    virtual const char *identifier()=0;
    virtual const char *localizedName(const char *locale, OVEncoding *enc)
        { return identifier(); }
    
    virtual int initialize(OVDictionary *loaderPref, OVDictionary *modulePref,
                           const char *modulePath, const char *userPath,
                           const char *seperator) { return 1 }
    virtual void terminate(OVDictionary *modulePref) { return 1 }
    virtual void update(OVDictionary *loaderPref, OVDictionary *modulePref)
        { return 1 }
};
                           
class OVInputMethod : public OVComponent
{
public:
    virtual OVIMContext *newConext()=0;
    virtual void deleteContext(OVIMContext* t) { delete t; }
    
    virtual const char *getCandidateSelectKeys() { return "1234567890"; }
};

class OVKeyFilter : public OVComponent
{
public:
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidateBox *candi,
        OVService *srv)
    {
        return 0;
    }
};

class OVBufferFilter : public OVComponent
{
public:
    virtual OVEncoding bufferEncoding()=0;
    virtual int process(void *data, int datalen, void *output, int maxoutputlen,
        OVCandidateBox *candi);
};


