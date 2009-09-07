//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "NSStringExtension.h"

@implementation NSString (SplitBySpaceWithQuote)
+ (NSString *)stringByColor:(NSColor *)c
{
	if ([c colorSpaceName] == NSDeviceWhiteColorSpace || [c colorSpaceName] == NSCalibratedWhiteColorSpace) {
		c = [NSColor colorWithCalibratedRed:1.0 green:1.0 blue:1.0 alpha:[c alphaComponent]];
	}
	else if ([c colorSpaceName] == NSDeviceBlackColorSpace || [c colorSpaceName] == NSCalibratedBlackColorSpace) {
		c = [NSColor colorWithCalibratedRed:0.0 green:0.0 blue:0.0 alpha:[c alphaComponent]];
	}	
	return [NSString stringWithFormat:@"%f %f %f", [c redComponent], [c greenComponent], [c blueComponent]];
}
- (NSColor *)colorByString
{
	NSArray *s = [self splitBySpaceWithQuote];
	if ([s count] < 3)
		return [NSColor blackColor];	
	return [NSColor colorWithDeviceRed:[[s objectAtIndex:0] floatValue] green:[[s objectAtIndex:1] floatValue] blue:[[s objectAtIndex:2] floatValue] alpha:1.0];
}
- (NSString *)stringByQuoting
{
    int l = [self length];
    UniChar *s = (UniChar*)calloc(1, l * sizeof(UniChar));
    [self getCharacters:s];
    UniChar *buf = (UniChar*)calloc(1, (l * 2 + 2) * sizeof(UniChar));
    int i, p = 0;
    
    buf[p++] = '\"';
    for (i = 0; i < l; i++) {
        if (s[i] == '\"')
			buf[p++] = '\\';
        buf[p++] = s[i];
    }
    buf[p++]='\"';
    NSString *r = [NSString stringWithCharacters:buf length:p];
    free(s);
    free(buf);
    return r;
}
- (NSString *)stringByChomping
{
    int i;
    int l = [self length];
    if (!l)
		return @"";
    
    for (i = l-1; i >= 0; i--) {
        if ([self characterAtIndex:i]!='\n')
			break;
    }
    
    if (i == -1)
		return @"";
	
    NSRange r = (NSRange){0, i+1};
    return [self substringWithRange:r];
}
- (NSArray *)splitBySpaceWithQuote
{
    NSMutableArray *ma = [[NSMutableArray new] autorelease];
    int p = 0, q = 0;
    int l = [self length];
    UniChar *s = (UniChar*)calloc(1, l*sizeof(UniChar));
    [self getCharacters:s];
    UniChar *buf = (UniChar*)calloc(1, l*sizeof(UniChar));
    int bufp = 0;
    int emptystr = 0;
    
    while (p<l) {
        // skip spaces
        if (s[p]==32)
			p++;
        
        // first non-space char
        if (s[p]=='\"') {
            q=1;
            p++;
        }

        bufp = 0;
        emptystr = 0;
        while (p < l) {
            if (!q && (s[p] == 32 || s[p] == '\"'))
				break;
            if (q && s[p]=='\"') {
                q = 0;
                p++;
                if (!bufp) 
					emptystr=1;      // make an emptystr regardless
                break;
            }
            if (s[p]=='\\' && p+1 < l && s[p+1]=='\"') {
                p += 2;
                buf[bufp++] = '\"';
            }
            else buf[bufp++] = s[p++];
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

