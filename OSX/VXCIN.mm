// VXCIN.mm

#define OVDEBUG
#include "VXCIN.h"
#include "VXUtility.h"

VXCIN::VXCIN()
{
    selkey[0]=0;
    ename[0]=0;
    cname[0]=0;
        
    keytable=[NSMutableDictionary new];
    chartable=[NSMutableDictionary new];
    if (!keytable || !chartable) throw ovExceptionMemory;
}

VXCIN::~VXCIN()
{
    [keytable release];
    [chartable release];
}

NSMutableArray* VXCIN::find(char *key)
{
    murmur ("VXCIN::find, query string=%s", key);
    NSString* qs=(NSString*)VXCreateCFString(key);
    NSArray* a=[chartable objectForKey: qs];    
    [qs release];
    return a;
}

void VXCIN::read(char *fname, OVEncoding enc, int shiftselkey)
{
    FILE *in=fopen(fname, "r");
        
    if (!in) throw ovException;
    murmur("VXCIN::read, filename=%s", fname);
    
    char buf[256];
    int state=0;
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
            murmur ("selkey=%s", selkey);
        }

        if (!strcasecmp(key, "%ename"))
        {
            strcmp(ename, value);
            murmur("ename=%s", ename);
        }
            
        if (!strcasecmp(key, "%cname"))
        {
            strcmp(cname, value);
            murmur("cname=%s", cname);
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
}

/*
int main(int argc, char**argv)
{
    if (argc < 2) return 0;
    
    id pool=[NSAutoreleasePool new];
    
    VXCIN cin;
    cin.read(argv[1]);
    
    while (!feof(stdin))
    {
        char buf[256];
        fscanf(stdin, "%s", buf);
        
        NSArray *ar=cin.find(buf);
        
        if (!ar)
        {
            murmur("key %s not found!", buf);
            continue;
        }
        
        int c=[ar count];
        murmur("total canidates=%d", c);
        for (int i=0; i<c; i++)
        {
            murmur("candidate %d=%s", i, [[ar objectAtIndex: i] UTF8String]);
        }
    }
    
    [pool release];
}
*/

