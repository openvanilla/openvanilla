// OVIMExample.cpp

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVEnglishIM;

class OVEnglishContext : public OVIMContext
{
public:
    OVEnglishContext(OVEnglishIM *p)
    {
        parent=p;
    }
    
    virtual ~OVEnglishContext()
    {
    }
    
    virtual int activate(OVService *)
    {
        return 1;
    }
    
    virtual int deactivate(OVService *)
    {
        return 1;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
	if (key->isOpt() || key->isCommand() || key->isCtrl())
	{
	    buf->clear()->update();
	    return 0;
	}
        
        if (key->isPrintable())
        {
            char str[2];
            str[1]=0;
            str[0]=key->code();
            buf->append(str)->update();
	    buf->send();
            return 1;   // key processed
        }
        
        return 0;   // key not processed
    }
    
protected:
    OVEnglishIM *parent;
};

class OVEnglishIM : public OVInputMethod
{
public:
    OVEnglishIM()
    {
    }
    
    virtual ~OVEnglishIM()
    {
    }
        
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVEnglishIM"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV 英數"));
        else
            return strlen(strcpy((char*)s, "OV English"));
    }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
    {
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }

    virtual int update(OVDictionary*, OVDictionary*)
    {
        return 1;
    }

    virtual OVIMContext* newContext() 
    {
        return new OVEnglishContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVEnglishIM)
OVLOADABLECANUNLOAD
