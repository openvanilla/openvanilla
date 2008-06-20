// CVWrappers.h: Objective-C wrapper for OVModule and OVInputMethodContext
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

#ifndef __CVWrappers_h
#define __CVWrappers_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>

@interface CVModuleWrapper : NSObject
{
    OVModule* mod;
    NSString *loadedpath;
    NSString *fromlibrary;
    BOOL inited;
    BOOL canuse;
}
- (void)dealloc;
- (BOOL)isEqual:(id)obj;
- (id)initWithModule:(OVModule*)m loadedPath:(NSString *)p fromLibrary:(NSString *)l;
- (NSString *)description;
- (NSString *)moduleType;
- (NSString *)fromLibrary;
- (NSString *)identifier;
- (OVModule*)module;
- (BOOL)usable;
- (BOOL)initializeWithConfig:(OVDictionary*)cfg service:(OVService*)s;
@end

@interface CVContextWrapper : NSObject
{
    OVInputMethodContext* cntx;
}
- (id)initWithContext:(OVInputMethodContext*)c;
- (void)dealloc;
- (OVInputMethodContext*)context;
@end

#endif
