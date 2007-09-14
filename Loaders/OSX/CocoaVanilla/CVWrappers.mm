// CVWrappers.mm: Objective-C wrapper for OVModule and OVInputMethodContext
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

#include "CVWrappers.h"

@implementation CVModuleWrapper
- (id)initWithModule:(OVModule*)m loadedPath:(NSString*)p fromLibrary:(NSString*)l {
    self=[super init];
    if (self) {
        inited=NO;
        canuse=NO;
        mod=m;
        loadedpath=[[NSString alloc] initWithString:p];
        fromlibrary=[[NSString alloc] initWithString:l];
    }
    return self;
}
- (void)dealloc {
    [loadedpath release];
    [super dealloc];
}
- (BOOL)isEqual:(id)obj {
    return [[self identifier] isEqualToString:[obj identifier]];
}
- (NSString*)description {
    if (!mod) return @"";
    return [NSString stringWithFormat: 
        @"(module id=%s, type=%s, module path=%@, initialized=%s, usable=%s)",
        mod->identifier(), mod->moduleType(), loadedpath,
        inited ? "YES" : "NO", canuse ? "YES" : "NO"];
}
- (NSString*)moduleType {
    return mod ? [NSString stringWithUTF8String:mod->moduleType()] : @"";
}
- (NSString*)fromLibrary {
    return fromlibrary;
}
- (NSString*)identifier {
    return mod ? [NSString stringWithUTF8String:mod->identifier()] : @"";
}
- (OVModule*)module {
    return mod;
}
- (BOOL)usable {
    return canuse;
}
- (BOOL)initializeWithConfig:(OVDictionary*)cfg service:(OVService*)s {
    if (!mod) return NO;
    if (inited) return canuse;
    inited=YES;
    if (mod->initialize(cfg, s, [loadedpath UTF8String])) return canuse=YES;
    return canuse=NO;
}
@end

@implementation CVContextWrapper
-(id)initWithContext:(OVInputMethodContext*)c {
    cntx=c;
    return self;
}
-(void)dealloc {
    if (cntx) {
        delete cntx;
    }
	[super dealloc];
}
-(OVInputMethodContext*)context {
    return cntx;
}
@end
