// VXCIN.mm

#define OVDEBUG
#include "VXCIN.h"
#include "VXUtility.h"

VXCIN::VXCIN()
{
    selkey[0]=endkey[0]=ename[0]=0;
        
    encoding=ovEncodingUTF8;
    keytable=[NSMutableDictionary new];
    chartable=[NSMutableDictionary new];
    cname=NULL;
    if (!keytable || !chartable) throw ovExceptionMemory;
}

VXCIN::~VXCIN()
{
    if (keytable) [keytable release];
    if (chartable) [chartable release];
    if (cname) [cname release];
}

int isEndKey(char c)
{
    if (!strlen(endkey)) return 0;
    if (strchr(endkey, tolower(c))) return 1;
    return 0;
}


int VXCIN::selKeyOrder(char keycode)
{
    char c=tolower(keycode);
    int l=strlen(selkey);
    for (int i=0; i<l; i++) if (c==selkey[i]) return i;
    return -1;
}

int VXCIN::name(char *locale, void *s, OVEncoding *enc)
{
    *enc=ovEncodingUTF8;
    
    if (!strlen(ename)) return strlen(strcpy((char*)s, "VXCIN no ename"));
    if (!strcasecmp(locale, "zh_tw") || !strcasecmp(locale, "zh_cn"))
        return strlen(strcpy((char*)s, [cname UTF8String]));
    return strlen(strcpy((char*)s, ename));
}

NSArray* VXCIN::find(char *key)
{
    murmur ("VXCIN::find, query string=%s", key);
    NSString* qs=(NSString*)VXCreateCFString(key);
    NSArray* a=[chartable objectForKey: qs];    
    [qs release];
    return a;
}

NSString* VXCIN::getKey(char keycode)
{
    char buf[2];
    buf[0]=tolower(keycode);
    buf[1]=0;
    
    NSString* qs=(NSString*)VXCreateCFString(buf);
    NSString* s=[keytable objectForKey: qs];
    murmur ("VXCIN::getKey, keycode=%s, string=%s", buf, s ? [s UTF8String] : NULL);
    [qs release];
    return s;
}

void VXCIN::read(char *fname, OVEncoding enc, int shiftselkey)
{
    FILE *in=fopen(fname, "r");
    if (!in) throw ovException;

    encoding=enc;    
    int state=0;
    char buf[256];

    while (!feof(in))
    {
        char key[256], value[256];
        
        fgets(buf, 255, in);
        if (buf[0]=='#') continue;
        
        sscanf(buf, "%s %s", key, value);

        if (!strcasecmp(key, "%selkey"))
        {   
            if (shiftselkey) strcpy(selkey, " ");
            strcat(selkey, value);
        }

        if (!strcasecmp(key, "%ename"))
        {
            strcpy(ename, value);
        }

        if (!strcasecmp(key, "%endkey"))
        {
            strcpy(endkey, value);
        }            

        if (!strcasecmp(key, "%cname"))
        {
            
            cname=(NSString*)VXCreateCFString(value, enc);
        }

        #define NAMESTATE(a, b, c)  \
            if (!strcasecmp(key, a) && !strcasecmp(value, b))   \
                { state=c; continue; }

        NAMESTATE("%keyname", "begin", 1);
        NAMESTATE("%keyname", "end", 0);
        NAMESTATE("%chardef", "begin", 2);
        NAMESTATE("%chardef", "end", 0);
        
        if (state==1)
        {
            id keyobj=[[NSString stringWithCString: key] lowercaseString];
            NSString* valueobj=(NSString*)VXCreateCFString(value, enc);

            if(keyobj && valueobj)
                [keytable setObject: valueobj forKey: keyobj];
        }
        
        if (state==2)
        {
            id keyobj=[[NSString stringWithCString: key] lowercaseString];
            NSString* valueobj=(NSString*)VXCreateCFString(value, enc);

            if(!keyobj || !valueobj) continue;

            id array=[chartable objectForKey: keyobj];
                        
            if (array)
            {
                [array addObject: valueobj];
            }
            else
            {
                array=[NSMutableArray new];
                [array addObject: valueobj];
                [chartable setObject: array forKey: keyobj];
            }
        }
    }
    
    fclose(in);
    murmur ("Finished read %s, ename=%s, cname=%s, selkey=\"%s\", number of keytable=%d, number of chartable=%d", fname, ename, [cname UTF8String], selkey, [keytable count], [chartable count]);
}