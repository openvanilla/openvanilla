// OVIMDayi.mm

#define OVDEBUG

#import <Cocoa/Cocoa.h>
#import <OpenVanilla/OpenVanilla.h>
#import <OpenVanilla/OVLoadable.h>
#import <OpenVanilla/OVUtility.h>

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
    if (!in) 
    {
        throw ovException;
        return tab;
    }
    
    murmur("reading %s", fname);
    
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
            id keyobj=[[NSString stringWithCString: key] uppercaseString];
            id valueobj=MakeNSStr(value);

//            murmur("adding key=%s, value=%s", key, value);
	    if(keyobj && valueobj)	
	        [tab.keytable setObject: valueobj forKey: keyobj];
        }
        
        if (state==2)
        {
            id keyobj=[[NSString stringWithCString: key] uppercaseString];
            id valueobj=MakeNSStr(value);

	    if(!keyobj || !valueobj)
		continue;

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

class OVDayiIM;

class OVDayiContext : public OVIMContext
{
public:
    OVDayiContext(OVDayiIM *p, DayiTable *t) : parent(p), tab(t)
    {
        keyseqlen=0;
        keyseq[0]=0;
        candi=0;
        autoreleasepool=[NSAutoreleasePool new];
        murmur("new im Context instance created");
    }
    
    virtual ~OVDayiContext()
    {
        [autoreleasepool release];
        murmur("im Context instance destroyed");
    }
    
    virtual int activate(OVService *)
    {
        murmur("im Context activated");
    }
    
    virtual int deactivate(OVService *)
    {
        murmur("im Context deactivated");
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf,
        OVTextBar *textbar, OVService *srv)
    {
        murmur("recevied key code=%d", key->code());
 
        if (candi)
        {
            int keycode=key->upper();
            if (keycode==ovkReturn) keycode=ovkSpace;
        
            if (keycode==ovkEscape)
            {
                textbar->hide();
                delkeyseq();
                buf->clear()->update(ovLangTradChinese);
                candi=0;
                return 1;
            }

            int i, nextsend=0, l=strlen(tab->selkey);
            // murmur("searching selkey=%s, len=%d", tab->selkey, l);

            for (i=0; i<l; i++) if (keycode==tab->selkey[i]) break;
            // murmur("index=%d, candistr_length=%d", i, [(NSString*)candistr length]);
            if (i==l || i >[(NSString*)candistr length]) 
            {
                // test if char key
                char bbuf[2];
                bbuf[1]=0;
                bbuf[0]=key->upper();
        
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
            buf->clear()->append((char*)[v UTF8String])->send(ovLangTradChinese);
            
            if (nextsend)
            {
                addkeyseq(key->upper());
                updatekeyseqdisplay(buf);
                if (key->code()=='=') goto SPAGHETTI;

            }
            
            textbar->hide();
            candi=0;
            return 1;
        }
        
 
        if (key->code()==ovkEscape)
        {
            buf->clear()->update();
            clearkeyseq();
            return 1;
        }
 
        if (key->isCode(2, ovkDelete, ovkBackspace))
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
            
            murmur("query string=%s ", keyseq);
            id qs=[NSString stringWithCString: keyseq];
            id v=[tab->chartable objectForKey: qs];
            if (v)
            {
                murmur("number of candidates: %d", [(NSString*)v length]);
                if ([(NSString*)v length] > 1)
                {
                    char bbb[256];
                    sprintf (bbb, " (%d candidates)    ", [(NSString*)v length]);

                    candi=1;
                    clearkeyseq();
                    candistr=[[NSString alloc]initWithString: v];
                    if (!candistr) murmur("copy candistr err!");
                    textbar->clear()->append((char*)[v UTF8String])->append(bbb)->
                        update();
                    textbar->show();

                    buf->clear()->append((char*)"？")->update(ovLangTradChinese);
                    return 1;
                }
            
                murmur("result=%s", [v UTF8String]);
                buf->clear()->append((char*)[v UTF8String])->
                    send(ovLangTradChinese);
                
                
//              buf->clear();
                clearkeyseq();
            }
            
            return 1;
        }
        
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!keyseqlen) return 0;
            clearkeyseq();
            buf->send(ovLangTradChinese);
            return 1;
        }
    
        if (key->isPrintable())
        {
            char bbuf[2];
            bbuf[1]=0;
            if (key->isShift())
            {
                bbuf[0]=key->lower();
                buf->clear()->append(bbuf)->send();
                return 1;
            }
            
            
            if (key->isCapslock()) return 0;

            if (addkeyseq(key->upper()))
            {
                updatekeyseqdisplay(buf);        
                if (key->code()=='=') goto SPAGHETTI;
                return 1;
            }
            
            bbuf[0]=key->code();
            buf->clear()->append(bbuf)->send();    
            return 1;        
        }    
   
        return 0;
    }

    
    
protected:
    OVDayiIM *parent;
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
            murmur("keyseq=%s ", [ss UTF8String]);
            id v=[tab->keytable objectForKey: ss];
            if (v)
            {
                [ms appendString: v];
                murmur("appended=%s, now=%s ", [v UTF8String], [ms UTF8String]);
            }
        }

        char *cc=(char*)[ms UTF8String];
        buf->clear()->append((void*)cc)->update();
    }
        
    void clearkeyseq()
    {
        keyseqlen=0;
        keyseq[0]=0;
    }
    
    int addkeyseq(char c)
    {
        char buf[2];
        buf[1]=0;
        buf[0]=c;
            
        id ss=[NSString stringWithCString: buf];
        id v=[tab->keytable objectForKey: ss];
        if (!v) return 0;
    
        if (keyseqlen<9) 
        {
            keyseq[keyseqlen++]=c; 
            keyseq[keyseqlen]=0;
        }
        
        return 1;
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
    
    id autoreleasepool;
};

class OVDayiIM : public OVInputMethod
{
public:
    OVDayiIM()
    {
        murmur("Dayi IM instance created");
        autoreleasepool=[NSAutoreleasePool new];
    }
    
    virtual ~OVDayiIM()
    {
        murmur("Dayi IM instance destroyed");
        [autoreleasepool release];
    }

    virtual int identifier(char *s)
    {
        return strlen(strcpy(s, "OVDayiIM"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OpenVanilla 大易輸入法試作版"));
        return strlen(strcpy((char*)s, "OpenVanilla Dayi Module"));
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        murmur("Dayi module terminated by IM loader");
        return 1;
    }

    virtual int initialize(OVDictionary*, OVDictionary* local, OVService*, char* path)
    {
        murmur("begin to initialize Dayi at path %s", path);
        char cinfile[256];
        char dayipath[256];
        strcpy(dayipath, path);
        
        if (local->keyExist("cin-filename"))
        {
            local->getString("cin-filename", cinfile);   
        }
        else
        {
            local->setString("cin-filename", "dayi3.cin");
            strcpy(cinfile, "dayi3.cin");            
        }
        
        strcat(dayipath, cinfile);
        tab=ReadDayi(dayipath); // XXX hard-coded path
        return 1;
    }
    
    OVIMContext *newContext() { return new OVDayiContext(this, &tab); }
    
    DayiTable tab;
    id autoreleasepool;
};

OVLOADABLEWRAPPER(OVDayiIM);

id autoreleasepool;
extern "C" void _init()
{
    murmur ("init!\n");
    
    NSApplicationLoad();    
    
    murmur ("Cocoa multithread mode: %d", [NSThread isMultiThreaded]);
    
    murmur ("alloc");
    autoreleasepool=[NSAutoreleasePool alloc];
    
    murmur ("init");
    [autoreleasepool init];
}  

extern "C" int OVLoadableCanUnload() 
{
    return 0; 
}



