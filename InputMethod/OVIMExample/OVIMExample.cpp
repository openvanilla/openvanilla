// OVIMExample.cpp

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVIMExample;

class OVExampleContext : public OVIMContext
{
public:
    OVExampleContext(OVExampleIM *p)
    {
        parent=p;
        fprintf (stderr, "New IM context created\n");
    }
    
    virtual ~OVExampleContext()
    {
        fprintf (stderr, "IM context destroyed\n");
    }
    
    virtual int activate(OVService *)
    {
        fprintf (stderr, "IM context activated\n");
        return 1;
    }
    
    virtual int deactivate(OVService *)
    {
        fprintf (stderr, "IM context deactivated\n");
        return 1;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
        fprintf (stderr, "Recevied key code=%d\n", key->code());
    
        if (buf->length()) textbar->show();
    
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!buf->length()) return 0;   // if buffer is empty, don't process
            buf->send();
            textbar->clear()->hide();
            fprintf (stderr, "Sending the content of input buffer to the app\n");
            return 1;   // key processed
        }
        
        if (key->isPrintable())
        {
            char str[2];
            str[1]=0;
            str[0]=key->code();
            textbar->append(str)->update();
            buf->append(str)->update();
            return 1;   // key processed
        }
        
        return 0;   // key not processed
    }
    
protected:
    OVExampleIM *parent;
};

class OVExampleIM : public OVInputMethod
{
public:
    OVExampleIM()
    {
        fprintf (stderr, "IM moudle instance created by loadble moudle\n");
    }
    
    virtual ~OVExampleIM()
    {
        fprintf (stderr, "IM module instance destroyed by loadable module\n");
    }
        
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVExampleIM"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV 輸入法範例"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV 输入法范例"));
        else
            return strlen(strcpy((char*)s, "OV Example"));
    }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
    {
        fprintf (stderr, "IM module initialized by IM loader\n");
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        fprintf (stderr, "IM module terminated by IM loader\n");
        return 1;
    }

    virtual int update(OVDictionary*, OVDictionary*)
    {
        fprintf (stderr, "IM loader requested updating module's config\n");
        return 1;
    }

    virtual OVIMContext* newContext() 
    {
        fprintf (stderr, "IM module creating a new IM context\n"); 
        return new OVExampleContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        fprintf (stderr, "IM module destroying a new IM context\n");
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVExampleIM)
OVLOADABLECANUNLOAD
