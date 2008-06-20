// CVConfig.mm: configuration management
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#include "CVConfig.h"

BOOL CVTimeTagEqual(const CVTimeTag *s1, const CVTimeTag *s2)
{
    return !memcmp(s1, s2, sizeof(CVTimeTag));
}

CVTimeTag CVGetFileTimeStamp(NSString *filename)
{
    const char *f = [filename UTF8String];
    struct stat s;
    CVTimeTag t;
    bzero(&t, sizeof(t));
    if (f && !stat(f, &s)) t=s.st_mtimespec;
    return t;
}

NSMutableDictionary *CVReadPropertyList(NSString *filename)
{
    NSData *d = [NSData dataWithContentsOfFile:filename];
    if (d) {        
        NSString *errMsg;
        NSPropertyListFormat fmt;
        
        // note p is already autoreleased (just like [NSString stringWith...])
        id p = [NSPropertyListSerialization propertyListFromData:d
            mutabilityOption:NSPropertyListMutableContainersAndLeaves 
            format:&fmt errorDescription:&errMsg];
        if (p) {
            if ([p isKindOfClass: [NSMutableDictionary class]]) {
                return p;
            }
        }
    }
    return nil;
}

CVTimeTag CVWritePropertyList(NSString *filename, NSDictionary *dict)
{
    NSString *errMsg;
    NSData *d=[NSPropertyListSerialization dataFromPropertyList:dict
        format:NSPropertyListXMLFormat_v1_0 errorDescription:&errMsg];
    [d writeToFile:filename atomically:YES];
    return CVGetFileTimeStamp(filename);
}

@implementation CVConfig
- (CVConfig*)initWithFile:(NSString *)f defaultData:(NSDictionary *)d
{
    if (self = [super init]) {
        filename = [[NSString alloc] initWithString: [f stringByStandardizingPath]];
        dict = [NSMutableDictionary new];
        backup = [NSMutableDictionary new];
        
        stamp = CVGetFileTimeStamp(filename);
        NSDictionary *p = CVReadPropertyList(filename);
        if (!p)
			p = d;
        
        if (p) {
            // read default value first, then overwrite
            // if (d) [dict addEntriesFromDictionary:d];            
            [dict addEntriesFromDictionary:p];
            [backup release];
            backup = [[NSMutableDictionary alloc] initWithDictionary:dict copyItems:YES];
        }
    }
    return self;
}
- (void)dealloc
{
    [filename release];
    [dict release];
    [backup release];
    [super dealloc];
}
- (NSString *)description
{
    return [dict description];
}
- (CVTimeTag)timeStamp
{
    return stamp;
}
- (CVTimeTag)sync
{
    // check timestamp
    CVTimeTag newstamp = CVGetFileTimeStamp(filename);
	
    if (!CVTimeTagEqual(&newstamp, &stamp)) {
        NSDictionary *p = CVReadPropertyList(filename);
        if (p) {
            [dict removeAllObjects];
            [dict addEntriesFromDictionary: p];
            [backup release];
            backup = [[NSMutableDictionary alloc] initWithDictionary:dict copyItems:YES];
        }
        return stamp = newstamp;
    }

    if ([dict isEqualToDictionary:backup])
		return stamp;
	
    stamp = CVWritePropertyList(filename, dict);
    [backup release];
    backup = [[NSMutableDictionary alloc] initWithDictionary:dict copyItems:YES];
    return stamp;
}
- (NSMutableDictionary *)dictionary
{
    return dict;
}
- (BOOL)needSync
{
    // first we check if there's any need to read
    CVTimeTag newstamp = CVGetFileTimeStamp(filename);
    if (!CVTimeTagEqual(&newstamp, &stamp))
		return YES;
    
    // now check if there's any need to write
    if (![dict isEqualToDictionary:backup])
		return YES;
	
    return NO;
}
@end
