// CVConfig.h: configuration managmenet
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __CVConfig_h
#define __CVConfig_h

#include <Cocoa/Cocoa.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

typedef struct timespec CVTimeTag;

#ifdef __cplusplus
    extern "C" BOOL CVTimeTagEqual(const CVTimeTag *s1, const CVTimeTag *s2);
    inline int operator==(const CVTimeTag &s1, const CVTimeTag &s2) {
        return CVTimeTagEqual(&s1, &s2);
    }
#endif

// note that sync method may discard local changes if the config file is newer
@interface CVConfig : NSObject {
    NSString *filename;
    NSMutableDictionary *dict;
    NSMutableDictionary *backup;
    CVTimeTag stamp;
}
- (void)dealloc;
- (NSString*)description;
- (CVConfig*)initWithFile:(NSString*)f defaultData:(NSDictionary*)d;
- (CVTimeTag)timeStamp;
- (CVTimeTag)sync;
- (NSMutableDictionary*)dictionary;
- (BOOL)needSync;
@end

#endif
