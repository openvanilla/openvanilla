// OVIMphp.cpp

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

#define PHP_PATH "/usr/bin/php"
#define OUT_PATH "/tmp/ovphp-out.tmp"
#define IN_PATH "/tmp/ovphp-in.tmp"

class OVIMPHP;

const int ebMaxKeySeq=1024;
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
void clear() { len=0; buf[0]=0; }
char buf[ebMaxKeySeq];
int len;
};

class OVPHPContext : public OVIMContext
{
public:
    OVPHPContext(OVIMPHP *p)
    {
        parent=p;
        fprintf (stderr, "New IM context created\n");
    }
    
    virtual ~OVPHPContext()
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
		textbar->clear()->append((char *)"OV PHP");
    		
		if (key->isCode(2, ovkDelete, ovkBackspace))
		{
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			buf->clear()->append(keyseq.buf)->update();
			if(!keyseq.len) textbar->hide();
			return 1;
		}
    
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!buf->length()) return 0;   // if buffer is empty, don't process
			struct stat st;
			char buffer[256];
			stat(PHP_PATH, &st);
			if(!st.st_size) 
				buf->append((char *)"You did not istall PHP on your Mac!")->send()->clear();
			
			buf->clear()->update();
						
			FILE *fp=fopen(OUT_PATH, "w");
			if (!fp) return 0;
			fprintf(fp, "<?php \n error_reporting(0);\n");
			fputs(keyseq.buf, fp);
			fprintf(fp, "\n?>");
			fclose(fp);
			
			sprintf(buffer, "%s %s > %s", PHP_PATH, OUT_PATH, IN_PATH);
			system(buffer);
			strcpy(buffer, "");
			
			fp=fopen(IN_PATH, "r");
			if (!fp) return 0;
			while (!feof(fp))
			{
				fgets(buffer, sizeof(buffer), fp);
				buf->append(buffer)->send()->clear();
			}
			fclose(fp);
			unlink(OUT_PATH);
			unlink(OUT_PATH);
            textbar->clear()->hide();
			keyseq.clear();
            return 1;   // key processed
        }
        
        if (key->isPrintable())
        {
            char str[2];
            str[1]=0;
            str[0]=key->code();
            buf->append(str)->update();
			keyseq.add(key->code());
			textbar->show();
            return 1;   // key processed
        }
        
        return 0;   // key not processed
    }
    
protected:
    OVIMPHP *parent;
	KeySeq keyseq;
};

class OVIMPHP : public OVInputMethod
{
public:
    OVIMPHP(){}
    virtual ~OVIMPHP(){}
        
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMPHP"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV PHP"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV PHP"));
        else
            return strlen(strcpy((char*)s, "OV PHP"));
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
        return new OVPHPContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVIMPHP)
OVLOADABLECANUNLOAD
