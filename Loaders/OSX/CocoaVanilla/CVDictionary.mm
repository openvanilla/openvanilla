// CVDictionary.mm: CocoaVanilla implementation of OVDictionary
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

#import "CVDictionary.h"

CVDictionary::CVDictionary(NSMutableDictionary *dict) {
    d=dict;
    [d retain];
}

CVDictionary::~CVDictionary() {
    [d release];
}

int CVDictionary::keyExist(const char *key) {
    return [d objectForKey:[NSString stringWithUTF8String:key]] ? 1 : 0;
}

int CVDictionary::getInteger(const char *key) {
    NSString *s;
    return (s=[d objectForKey: [NSString stringWithUTF8String:key]]) ? [s intValue] : 0;
}

int CVDictionary::setInteger(const char *key, int value) {
    [d setObject: [NSString stringWithFormat: @"%d", value] forKey: [NSString stringWithUTF8String:key]];
    return value;
}

const char* CVDictionary::getString(const char *key) {
    NSString *s;
    return (s=[d objectForKey: [NSString stringWithUTF8String:key]]) ? [s UTF8String] : "";
}

const char* CVDictionary::setString(const char *key, const char *value) {
    [d setObject:[NSString stringWithUTF8String:value] forKey:[NSString stringWithUTF8String:key]];
    return value;
}