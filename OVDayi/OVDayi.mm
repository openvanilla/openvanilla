// OVIMExample.mm

#include <Cocoa/Cocoa.h>
#include "openvanilla.h"

id pool=nil;

extern "C" void _init()
{
    pool=[NSAutoreleasePool new];
    if (pool) fprintf (stderr, "initializing dylib, creating autorelease pool\n");
}

id MakeNSStr(char *s)
{
    return [[NSString alloc] initWithBytes:s length:strlen(s) encoding: 
            CFStringConvertEncodingToNSStringEncoding
                (kCFStringEncodingBig5_HKSCS_1999)]; 
}

struct DayiTable
{
    id keytable;
    id chartable;
    char selkey[32];
};

DayiTable ReadDayi(char *fname)
{
    DayiTable tab;
    FILE *in=fopen(fname, "r");
    if (!in) return tab;
    
    fprintf (stderr, "reading %s\n", fname);
    
    tab.keytable=[NSMutableDictionary new];
    tab.chartable=[NSMutableDictionary new];
    if (!tab.keytable || !tab.chartable) return tab;
    
    char buf[256];
    int state=0;
    while (!feof(in))
    {
        char key[256], value[256];
        
        fgets(buf, 255, in);
        if (buf[0]=='#') continue;
        
        sscanf(buf, "%s %s", key, value);
        // printf ("key=%s, value=%s\n", key, value);

        if (!strcmp(key, "%selkey"))
        {   
            strcpy(tab.selkey, " ");
            strcat(tab.selkey, value);
            printf ("selkey=%s\n", tab.selkey);
        }

        if (!strcmp(key, "%keyname") && !strcmp(value, "begin"))
        {
            state=1;
            continue;
        }

        if (!strcmp(key, "%keyname") && !strcmp(value, "end"))
        {
            state=0;
            continue;
        }

        if (!strcmp(key, "%chardef") && !strcmp(value, "begin"))
        {
            state=2;
            continue;
        }
        
        if (!strcmp(key, "%chardef") && !strcmp(value, "end"))
        {
            state=0;
            continue;
        }
        
        if (state==1)
        {
            // printf ("adding key=%s, value=%s\n", key, value);
            [tab.keytable setObject: MakeNSStr(value)
                forKey: [[NSString stringWithCString: key] uppercaseString]];
        }
        
        if (state==2)
        {
            id keyobj=[[NSString stringWithCString: key] uppercaseString];
            id valueobj=MakeNSStr(value);
            id str=[tab.chartable objectForKey: keyobj];
            
            if (str)
            {
//                printf ("dup key=%s\n", key);
                [str appendString: valueobj];
            }
            else
            {
                [tab.chartable setObject: [NSMutableString stringWithString: valueobj] forKey: keyobj];
            }
        }
    }
    
    
    fclose(in);
    return tab;
}



class OVExampleIM;

class OVExampleSession : public OVIMSession
{
public:
    OVExampleSession(OVExampleIM *p, DayiTable *t) : parent(p), tab(t)
    {
        keyseqlen=0;
        keyseq[0]=0;
        candi=0;
        fprintf (stderr, "new im session instance created\n");
    }
    
    virtual ~OVExampleSession()
    {
        fprintf (stderr, "im session instance destroyed\n");
    }
    
    virtual int activate(OVService *)
    {
        fprintf (stderr, "im session activated\n");
    }
    
    virtual int deactivate(OVService *)
    {
        fprintf (stderr, "im session deactivated\n");
    }
    
    virtual int keyevent(OVKeyCode *key, OVBuffer *buf,
        OVTextBar *textbar, OVService *srv)
    {
        fprintf (stderr, "recevied key code=%d\n", key->code());
 
        if (candi)
        {
            int keycode=key->uppercode();
            if (keycode==ovkReturn) keycode=ovkSpace;
        
            if (keycode==ovkEscape)
            {
                textbar->hide();
                delkeyseq();
                buf->clear()->updatedisplay(ovLangTradChinese);
                candi=0;
                return 1;
            }

            int i, nextsend=0, l=strlen(tab->selkey);
            // fprintf (stderr, "searching selkey=%s, len=%d\n", tab->selkey, l);

            for (i=0; i<l; i++) if (keycode==tab->selkey[i]) break;
            // fprintf (stderr, "index=%d, candistr_length=%d\n", i, [(NSString*)candistr length]);
            if (i==l || i >[(NSString*)candistr length]) 
            {
                // test if char key
                char bbuf[2];
                bbuf[1]=0;
                bbuf[0]=key->uppercode();
        
                id ss=[NSString stringWithCString: bbuf];
                id vvv=[tab->keytable objectForKey: ss];
                if (vvv)
                {
                    i=0;
                    nextsend=1;
                }
                else return 1;
            }
                 
            NSRange r;
            r.location=i;
            r.length=1;
            id v=[candistr substringWithRange: r];
            buf->clear()->append([v UTF8String])->send(ovLangTradChinese);
            
            if (nextsend)
            {
                addkeyseq(key->uppercode());
                updatekeyseqdisplay(buf);
                if (key->code()=='=') goto SPAGHETTI;

            }
            
            textbar->hide();
            candi=0;
            return 1;
        }
        
 
        if (key->code()==ovkEscape)
        {
            buf->clear()->updatedisplay();
            clearkeyseq();
            return 1;
        }
 
        if (key->iscode(2, ovkDelete, ovkBackspace))
        {
            if (!keyseqlen) return 0;
            delkeyseq();
            updatekeyseqdisplay(buf);
            return 1;
        }
        
        if (key->code()==ovkSpace)
        {
SPAGHETTI:
            if (!keyseqlen) return 0;
            
            fprintf (stderr, "query string=%s ", keyseq);
            id qs=[NSString stringWithCString: keyseq];
            id v=[tab->chartable objectForKey: qs];
            if (v)
            {
                fprintf (stderr, "number of candidates: %d\n", [(NSString*)v length]);
                if ([(NSString*)v length] > 1)
                {
                    candi=1;
                    clearkeyseq();
                    candistr=[[NSString alloc]initWithString: v];
                    if (!candistr) fprintf (stderr, "copy candistr err!\n");
                    textbar->show();
                    textbar->clear()->append([v UTF8String]);
                    textbar->update();
                    buf->clear()->append("？")->updatedisplay(ovLangTradChinese);
                    return 1;
                }
            
                fprintf (stderr, "result=%s", [v UTF8String]);
                buf->clear()->append([v UTF8String])->send(ovLangTradChinese);
                clearkeyseq();
            }
            fprintf (stderr, "\n");
            
            return 1;
        }
        
        if (key->iscode(2, ovkReturn, ovkMacEnter))
        {
            if (!keyseqlen) return 0;
            clearkeyseq();
            buf->send(ovLangTradChinese);
            return 1;
        }
    
        if (key->isprintable())
        {
            if (key->isshift())
            {
                char bbuf[3];
                bbuf[1]=0;
                bbuf[0]=key->lowercode();
                buf->clear()->append(bbuf)->send();
                return 1;
            }
            
            
            if (key->iscapslock()) return 0;

            addkeyseq(key->uppercode());
            updatekeyseqdisplay(buf);
            
            if (key->code()=='=') goto SPAGHETTI;
            return 1;
        }    
 

    
        return 0;
    }

    
    
protected:
    OVExampleIM *parent;
    DayiTable *tab;
    int candi;
    id candistr;
    
    void updatekeyseqdisplay(OVBuffer *buf)
    {
        id ms=[NSMutableString new];
        
        for (int i=0; i<keyseqlen; i++)
        {
            char buf[2];
            buf[1]=0;
            buf[0]=keyseq[i];
            
            id ss=[NSString stringWithCString: buf];
            fprintf (stderr, "keyseq=%s ", [ss UTF8String]);
            id v=[tab->keytable objectForKey: ss];
            if (v)
            {
                [ms appendString: v];
                fprintf (stderr, "appended=%s, now=%s ", [v UTF8String], [ms UTF8String]);
            }
        }
        
        char *cc=(char*)[ms UTF8String];
        buf->clear()->append((void*)cc)->updatedisplay();
        fprintf (stderr, "\n");
    }
        
    void clearkeyseq()
    {
        keyseqlen=0;
        keyseq[0]=0;
    }
    
    void addkeyseq(char c)
    {
        char buf[2];
        buf[1]=0;
        buf[0]=c;
            
        id ss=[NSString stringWithCString: buf];
        id v=[tab->keytable objectForKey: ss];
        if (!v) return;
    
        if (keyseqlen<9) 
        {
            keyseq[keyseqlen++]=c; 
            keyseq[keyseqlen]=0;
        }
    }
    
    void delkeyseq()
    {
        if (keyseqlen)
        {
            keyseq[--keyseqlen]=0;
        }
    }
    
    int keyseqlen;
    char keyseq[10];
};

class OVExampleIM : public OVInputMethod
{
public:
    OVExampleIM()
    {
        fprintf (stderr, "new IM instance created\n");
        tab=ReadDayi("/tmp/dayi3.cin"); // XXX hard-coded path

        fprintf (stderr, "creating new per-instance autorelease pool\n");
        pool=[[NSAutoreleasePool alloc] init];
        if (pool) fprintf (stderr, "per-instance autorelease pool creation succeeded\n");
    }
    
    virtual ~OVExampleIM()
    {
        fprintf (stderr, "IM instance destroyed\n");
        fprintf (stderr, "releasing per-instance autorelease pool\n");
        [pool release];
    }
    
    OVIMSession *newsession() { return new OVExampleSession(this, &tab); }
    
    DayiTable tab;
    id pool;
};


extern "C" OVExampleIM *create()
{
//    if (!pool) pool=[[NSAutoreleasePool alloc] init];
    return new OVExampleIM;
}

extern "C" void destroy(OVExampleIM *o)
{
    delete o;
//    [pool release];
}



