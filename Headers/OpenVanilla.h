

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
    virtual int setSelectKey(const char *selkey);
    virtual OVCandidateBox* clear();
    virtual OVCandidateBox* append(const void *str, OVEncoding enc=oveUTF8,
        int len=0)=0;
    virtual OVCandidateBox* show();
    virtual OVCandidateBox* finalize();
};


enum {
    oveUTF8=1,
    oveUTF16BE=2,
    oveUTF16LE=3,
    oveUTF16Auto=4,
    oveBig5=8,
    oveBig5HKSCS=8,
    oveEUC_CN=9
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


class OVIMContext : public OVObject
{
public:
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, 
        OVCandidateBox *candi, OVService *srv)=0;
};


class OVComponent : public OVObject
{
public:
    virtual const char *identifier()=0;
    virtual const char *localizedName(const char *locale, OVEncoding *enc)=0;
    
    virtual int initialize(OVDictionary *loaderPref,
                           OVDictionary *modulePref,
                           const char *modulePath,
                           const char *userPath,
                           const char *seperator)=0;
    virtual void terminate(OVDictionary *modulePref)=0;
    virtual void update(OVDictionary *loaderPref, 
                        OVDictionary *modulePref)=0;
};
                           
class OVInputMethod : public OVComponent
{
public:
    virtual OVIMContext *newConext()=0;
    virtual void deleteContext(OVIMContext*)=0;
    
    virtual const char *getCandidateSelectKeys();
    virtual const char *getValidKeys();
};

class OVKeyFilter : public OVComponent
{
public:
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidateBox *candi,
        OVService *srv)=0;
};

class OVBufferFilter : public OVComponent
{
public:
    virtual OVEncoding bufferEncoding()=0;
    virtual int process(void *data, int datalen, void *output, int maxoutputlen,
        OVCandidateBox *candi);
};


