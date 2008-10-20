//
// LVModuleManager.h
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
#import <OpenVanilla/OpenVanilla.h>
#import <OpenVanilla/OVLibrary.h>
#import "LVOVImplementations.h"

class LVContextSandwich : public OVBase {
public:
	LVContextSandwich(OVInputMethodContext *inputMethodContext)
	{
		_inputMethodContext = inputMethodContext;
	}
	
	~LVContextSandwich()
	{
		if (_inputMethodContext) {
			delete _inputMethodContext;
		}
	}
	
    virtual void start(OVBuffer* buffer, OVCandidate* candidate, OVService* service)
	{
		if (_inputMethodContext) {
			_inputMethodContext->start(buffer, candidate, service);
		}
	}
	
    virtual void clear()
	{
		if (_inputMethodContext) {
			_inputMethodContext->clear();
		}
	}
	
    virtual void end()
	{
		if (_inputMethodContext) {
			_inputMethodContext->end();
		}
	}
	
    virtual bool keyEvent(OVKeyCode* keyCode, OVBuffer* buffer, OVCandidate* candidate, OVService* service)
	{
		bool handled = false;
		if (_inputMethodContext) {
			handled = !!_inputMethodContext->keyEvent(keyCode, buffer, candidate, service);
		}
		
		return handled;
	}
	
protected:
	OVInputMethodContext *_inputMethodContext;
};
		

@interface LVModule : NSObject
{
	BOOL _initialized;
	BOOL _usable;
	OVModule *_module;
	NSString *_moduleDataPath;
}
+ (LVModule *)moduleWithModuleObject:(OVModule*)module moduleDataPath:(NSString *)dataPath;
- (OVModule *)moduleObject;
- (NSString *)description;
- (NSString *)moduleIdentifier;
- (BOOL)lazyInitWithLoaderService:(LVService*)service configDictionary:(NSMutableDictionary *)configDict;
- (BOOL)isInitialized;
- (BOOL)isUsable;
@end

@interface LVModuleManager : NSObject
{
	NSMutableDictionary *_configDictionary;
	NSTimeInterval _configTimestamp;
	
    NSMutableArray *_modulePackageBundlePaths;
    NSMutableDictionary *_loadedModulePackageBundleDictionary;	
	NSMutableDictionary *_loadedModuleDictionary;
	LVService *_loaderService;
	
	NSString *_primaryInputMethodModuleID;
}
+ (LVModuleManager *)sharedManager;
- (void)setModulePackageBundlePaths:(NSArray *)array;
- (void)loadModulePackageBundles;
- (LVService*)loaderService;
- (LVContextSandwich*)createContextSandwich;
- (void)syncConfiguration;

- (LVModule *)moduleForIdentifier:(NSString *)identifier;
- (void)setPrimaryInputMethodModuleID:(NSString *)moduleID;
- (NSString *)primaryInputMethodModuleID;
- (NSArray *)inputMethodTitlesAndModuleIDs;
@end

@interface LVModuleManager (ProtectedMethods)
- (NSString *)userDataPathForModuleID:(NSString *)moduleID;
@end

extern NSString *LVModuleConfigChangedNotification;
