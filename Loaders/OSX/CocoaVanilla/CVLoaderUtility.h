// CVLoaderUtility.h: CocoaVanilla loader utilities
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __CVLoaderUtility_h
#define __CVLoaderUtility_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include "CVWrappers.h"

// returns an array of all loaded modules, which are milked from loaded libraries,
// which are in turn loaded from the paths indicated in the paths array
NSArray* CVLoadEverything(NSArray *paths, OVService *srv, NSArray *libexcludelist = nil, NSArray *modexcludelist = nil, NSMutableDictionary *history = nil, NSString *atomic = nil);

CVModuleWrapper* CVFindModule(NSArray *modlist, NSString *identifier, NSString *type = nil);
NSArray *CVFindModules(NSArray *modlist, NSArray *idlist, NSString *type = nil);
NSArray *CVGetModulesByType(NSArray *modlist, NSString *type);

// atomic init related
void CVAtomicInitStart(NSString *f, NSString *libname);
void CVAtomicInitEnd(NSString *f);

// utility functions
BOOL CVIfPathExists(NSString *p);
BOOL CVStringIsInArray(NSString *s, NSArray *a);
NSArray *CVEnumeratePath(NSString *path, NSString *ext);
void CVRemoveStringFromArray(NSString *s, NSMutableArray *ma);

#endif
