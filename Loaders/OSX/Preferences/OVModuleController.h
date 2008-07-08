// OVModuleController.h : The controller for settings of OpenVanilla modules.
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

#import <Cocoa/Cocoa.h>
#import "NSObjectUpdateConfig.h"

@interface OVModuleController : NSObject {
	IBOutlet NSView *view;
	NSString *_identifier;
	NSString *_localizedName;
	NSString *_shortcut;
	NSMutableDictionary *_dictionary;
	BOOL _enabled;
	id _delegate;
}

- (id)copyWithZone:(NSZone *)zone;
- (id)initWithIdentifier:(NSString *)identifier localizedName:(NSString *)localizedName dictionary:(NSDictionary *)dictionary enabled:(BOOL)enabled delegate:(id)delegate shortcut:(NSString *)shortcut;
- (void)loadNib;
- (NSString *)moduleIdentifer;
- (NSString *)localizedName;
- (NSDictionary *)dictionary;
- (void)setDictionary:(NSDictionary *)dictionary;
- (BOOL)isEnabled;
- (void)setEnabled:(BOOL)enabled;
- (NSString *)shortcut;
- (void)setShortcut:(NSString *)shortcut;
- (void)setShortcut:(NSString *)shortcut fromSender:(id)sender;
- (id)delegate;
- (void)setDelegate:(id)delegate;
- (void)setValue:(id)value forKey:(NSString *)key;
- (void)update;
- (void)updateAndWrite;
- (NSView *)view;

@end
