// OVIMExample.cpp
// OpenVanilla Example Input Method
// OpenVanilla 輸入法設計範例
// 2005 openavnilla.org

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVIMExample;

const int ebMaxKeySeq=10;

class KeySeq
{
public:
    KeySeq() { len=0; buf[0]=0; }
    void add(char c)
	{
	if (len == ebMaxKeySeq) return;
		buf[len++]=c;
		buf[len]=0;
	}
	void remove()
	{
		if (!len) return;
		buf[--len]=0;
	}
	void clear() { len=0;  buf[0]=0;}
	char buf[ebMaxKeySeq];
	int len;
};

class OVExampleContext : public OVIMContext
{
public:
    OVExampleContext(OVIMExample *p)
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
	
    virtual int clear()
    {
		keyseq.clear();
		return 1;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
        fprintf (stderr, "Recevied key code=%d\n", key->code());
		
		/* We do not need such Modifiers */
		if (key->isOpt() || key->isCommand() || key->isCtrl()) return 0; 
		    
		/* You can use textbar->show(); to display the Candidate Window */
        if (buf->length()) textbar->show();
    
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!buf->length()) return 0;   // if buffer is empty, don't process
            buf->send();
            textbar->clear()->hide();
			keyseq.clear();
            fprintf (stderr, "Sending the content of input buffer to the app\n");
            return 1;   // key processed
        }
		
		if (key->isCode(1, ovkSpace))
		{
            if (!buf->length()) return 0;   // if buffer is empty, don't process
			buf->clear()->append((char *)"Example")->update()->send();
            textbar->clear()->hide();
			keyseq.clear();
			return 1;   // key processed
		}
		
		if (key->isCode(2, ovkDelete, ovkBackspace))
		{
            if (!buf->length()) return 0;   // if buffer is empty, don't process
			char str[2];
			keyseq.remove();
			buf->clear()->append((char *)keyseq.buf)->update();
			sprintf(str, "%2d", keyseq.len);
			textbar->clear()->append("Buffer Length:")->append(str)->update();
			return 1;   // key processed
		}
        
        if (key->isPrintable())
        {
            char str[2];
            //str[1]=0;
            //str[0]=key->code();
            //buf->append(str)->update();
			keyseq.add(key->code());
			buf->clear()->append((char *)keyseq.buf)->update();
			sprintf(str, "%2d", keyseq.len);
			textbar->clear()->append("Buffer Length:")->append(str)->update();
            return 1;   // key processed
        }
        
        return 0;   // key not processed
    }
    
protected:
    OVIMExample *parent;
	KeySeq keyseq;
};

class OVIMExample : public OVInputMethod
{
public:
    OVIMExample()
    {
        fprintf (stderr, "IM moudle instance created by loadble moudle\n");
    }
    
    virtual ~OVIMExample()
    {
        fprintf (stderr, "IM module instance destroyed by loadable module\n");
    }
        
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMExample"));
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
OVLOADABLEWRAPPER(OVIMExample)
OVLOADABLECANUNLOAD