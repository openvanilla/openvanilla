#define OVDEBUG
#include "VXCIN.h"

int main(int argc, char**argv)
{
    if (argc < 3) return 0;
    
    id pool=[NSAutoreleasePool new];
    
    OVEncoding enc=ovEncodingUTF8;
    
    if (!strcasecmp(argv[1], "big5")) enc=ovEncodingBig5HKSCS;
    if (!strcasecmp(argv[1], "euc")) enc=ovEncodingEUC_CN;
    
    VXCIN cin;
    cin.read(argv[2], enc);
    
    NSString* ss;
    ss=cin.getKey('a');
    if (ss) murmur ("key code 'a'=%s", [ss UTF8String]);
    ss=cin.getKey('1');
    if (ss) murmur ("key code '1'=%s", [ss UTF8String]);
    
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
