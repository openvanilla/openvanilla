#include "NSStringExtension.h"

@implementation NSString (SplitBySpaceWithQuote) 
-(NSString*)stringByChomping {
    int i;
    int l=[self length];
    if (!l) return [[NSString new] autorelease];
    
    for (i=l-1; i>=0; i--) {
        if ([self characterAtIndex:i]!='\n') break;
    }
    
    if (i==-1) return [[NSString new] autorelease];
    NSRange r=(NSRange){0, i+1};
    return [self substringWithRange:r];
}
-(NSArray*)splitBySpaceWithQuote {
    NSMutableArray *ma=[[NSMutableArray new] autorelease];
    int p=0, q=0;
    int l=[self length];
    UniChar *s=(UniChar*)calloc(1, l*sizeof(UniChar));
    [self getCharacters:s];
    UniChar *buf=(UniChar*)calloc(1, l*sizeof(UniChar));
    int bufp=0;
    int emptystr=0;
    
    while (p<l) {
        // skip spaces
        if (s[p]==32) p++;
        
        // first non-space char
        if (s[p]=='\"') {
            q=1;
            p++;
        }

        bufp=0;
        emptystr=0;
        while (p<l) {
            if (!q && (s[p]==32 || s[p]=='\"')) break;
            if (q && s[p]=='\"') {
                q=0;
                p++;
                if (!bufp) emptystr=1;      // make an emptystr regardless
                break;
            }
            if (s[p]=='\\' && p+1 < l && s[p+1]=='\"') {
                p+=2;
                buf[bufp++]='\"';
            }
            else buf[bufp++]=s[p++];
        }
        
        if (bufp || emptystr) {
            [ma addObject:[NSString stringWithCharacters:buf length:bufp]];
        }
    }
    
    free(s);
    free(buf);
    return ma;
}
@end
