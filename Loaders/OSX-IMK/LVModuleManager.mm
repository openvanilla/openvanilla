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

#import "LVConfig.h"
#import "LVModuleManager.h"
#include <OpenVanilla/OVWildcard.h>

using namespace OpenVanilla;

NSString *LVPrimaryInputMethodKey = @"primaryInputMethod";
NSString *LVModuleConfigChangedNotification = @"LVModuleConfigChangedNotification";

@implementation LVModule
- (void)dealloc
{
	delete _module;
	[_moduleDataPath release];
	[super dealloc];
}
- (id)initWithModule:(OVModule *)module moduleDataPath:(NSString *)dataPath
{
	if (self = [super init]) {
		_module = module;
		_moduleDataPath = [dataPath copy];
	}
	return self;
}
+ (LVModule *)moduleWithModuleObject:(OVModule*)module moduleDataPath:(NSString *)dataPath
{
	return [[[LVModule alloc] initWithModule:module moduleDataPath:(NSString *)dataPath] autorelease];
}
- (OVModule *)moduleObject
{
	return _module;
}
- (NSString *)description
{
	return [NSString stringWithFormat:@"OVModule (type '%s', identifier '%s', en name '%s')", _module->moduleType(), _module->identifier(), _module->localizedName("en")];
}
- (NSString *)moduleIdentifier
{
	const char *i = _module->identifier();
	return [NSString stringWithUTF8String:_module->identifier()];
}
- (BOOL)lazyInitWithLoaderService:(LVService*)service configDictionary:(NSMutableDictionary *)configDict
{
	if (_initialized) {
		return _usable;
	}
	
	
	LVDictionary cd(configDict);
	
	_initialized = YES;
	_usable = !!_module->initialize(&cd, service, [_moduleDataPath UTF8String]);
		
	return _usable;
}
- (BOOL)isInitialized
{
	return _initialized;
}
- (BOOL)isUsable
{
	if (!_initialized) {
		return YES;
	}
	return _usable;
}
@end


@implementation LVModuleManager : NSObject
- (NSString *)_configFilePath
{
	NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSAssert([dirs count], @"NSSearchPathForDirectoriesInDomains");		
	NSString *prefPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:@"Preferences"];	
	return [prefPath stringByAppendingPathComponent:OPENVANILLA_LOADER_PLIST_FILENAME];
}
- (void)_unloadEverything
{
	[_loadedModuleDictionary removeAllObjects];
	
	NSEnumerator *keyEnum = [_loadedModulePackageBundleDictionary keyEnumerator];	
	NSString *key;
	while (key = [keyEnum nextObject]) {
		CFBundleRef bundle = (CFBundleRef)[_loadedModulePackageBundleDictionary objectForKey:key];
		CFBundleUnloadExecutable(bundle);
	}
	[_loadedModulePackageBundleDictionary removeAllObjects];
}
- (void)_writeConfigurationFile
{
	NSData *data = [NSPropertyListSerialization dataFromPropertyList:_configDictionary format:NSPropertyListXMLFormat_v1_0 errorDescription:nil];
	if (data) {
		[data writeToFile:[self _configFilePath] atomically:YES];
		
		NSDictionary *attr = [[NSFileManager defaultManager] fileAttributesAtPath:[self _configFilePath] traverseLink:YES];
		_configTimestamp = [[attr objectForKey:NSFileModificationDate] timeIntervalSince1970];
	}
}

- (void)_validateAndWriteConfig
{
	// validate the config
	if (![_primaryInputMethodModuleID length]) {
		NSArray *allInputMethods = [self inputMethodTitlesAndModuleIDs];
		if ([allInputMethods count]) {
			[_primaryInputMethodModuleID autorelease];
			_primaryInputMethodModuleID = [[[allInputMethods objectAtIndex:0] objectAtIndex:1] retain];
		}
	}
	
	if ([_primaryInputMethodModuleID length]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:_primaryInputMethodModuleID];
		
		BOOL valid = NO;
		if (module) {
			if (OVWildcard::Match([module moduleObject]->moduleType(), "OVInputMethod")) {
				valid = YES;
			}
		}
		
		if (!valid) {
			[_primaryInputMethodModuleID autorelease];
			_primaryInputMethodModuleID = nil;
		}
	}
	
	[_configDictionary setObject:([_primaryInputMethodModuleID length] ? _primaryInputMethodModuleID : @"") forKey:LVPrimaryInputMethodKey];
	[self _writeConfigurationFile];	
}

- (void)_syncConfigurationWithNotification:(BOOL)sendNotification
{
	NSDictionary *oldConfigDict = [[_configDictionary copy] autorelease];
	
	// get a list of all keys
	NSDictionary *attr = [[NSFileManager defaultManager] fileAttributesAtPath:[self _configFilePath] traverseLink:YES];
	NSTimeInterval nowTimestamp = [[attr objectForKey:NSFileModificationDate] timeIntervalSince1970];
	
	if (_configTimestamp == nowTimestamp) {
		return;
	}
	
	_configTimestamp = nowTimestamp;
	
	NSData *configData = [NSData dataWithContentsOfFile:[self _configFilePath]];
	if (configData) {
		NSPropertyListFormat format;
		id plist = [NSPropertyListSerialization propertyListFromData:configData mutabilityOption:NSPropertyListMutableContainersAndLeaves format:&format errorDescription:nil];
		if ([plist isKindOfClass:[NSDictionary class]]) {
			[_configDictionary removeAllObjects];
			[_configDictionary addEntriesFromDictionary:plist];
		}
	}
		
	_primaryInputMethodModuleID = [[_configDictionary objectForKey:LVPrimaryInputMethodKey] retain];
		
	if (![oldConfigDict isEqualToDictionary:_configDictionary]) {
		// tell every usable module that some configuration has been changed
		
		NSEnumerator *keyEnum = [_loadedModuleDictionary keyEnumerator];
		NSString *moduleIdentifier;
		while (moduleIdentifier = [keyEnum nextObject]) {
			LVModule *module = [_loadedModuleDictionary objectForKey:moduleIdentifier];
			if ([module isInitialized]  && [module isUsable]) {
				OVModule *moduleObject = [module moduleObject];
				NSMutableDictionary *configDict = [_configDictionary objectForKey:[module moduleIdentifier]];
				if (!configDict) {
					configDict = [NSMutableDictionary dictionary];
					[_configDictionary setObject:configDict forKey:[module moduleIdentifier]];
				}		
				
				LVDictionary dict(configDict);
				moduleObject->update(&dict, _loaderService);
			}
		}
		
		if (sendNotification) {
			[[NSNotificationCenter defaultCenter] postNotificationName:LVModuleConfigChangedNotification object:self];
		}
	}

	[self _writeConfigurationFile];
}

- (void)delloc
{
    [self _unloadEverything];
	[_loadedModuleDictionary release];
    [_loadedModulePackageBundleDictionary release];
    [_modulePackageBundlePaths release];	
	[_configDictionary release];
	[_primaryInputMethodModuleID release];
	delete _loaderService;
	
    [super dealloc];
}
- (id)init
{
    if (self = [super init]) {
		_configDictionary = [NSMutableDictionary new];
        _modulePackageBundlePaths = [NSMutableArray new];
        _loadedModulePackageBundleDictionary = [NSMutableDictionary new];
		_loadedModuleDictionary = [NSMutableDictionary new];
		
		_loaderService = new LVService;
    }
    return self;
}
+ (LVModuleManager *)sharedManager
{
    static LVModuleManager *sharedInstance = nil;
    if (!sharedInstance) {
        sharedInstance = [LVModuleManager new];
    }
    return sharedInstance;
}
- (void)setModulePackageBundlePaths:(NSArray *)array
{
    [_modulePackageBundlePaths setArray:array];
}
- (void)loadModulePackageBundles
{
    [self _unloadEverything];
    
	NSMutableArray *_bundlePathArray = [NSMutableArray array];
	
    NSEnumerator *mpbpEnum = [_modulePackageBundlePaths objectEnumerator];
    NSString *path;
    while (path = [mpbpEnum nextObject]) {
        if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
            continue;
        }
        
        NSDirectoryEnumerator *dirEnum = [[NSFileManager defaultManager] enumeratorAtPath:path];
        NSString *target;
        while (target = [dirEnum nextObject]) {
            if ([[target pathExtension] isEqualToString:@"bundle"]) {
                [dirEnum skipDescendents];
                [_bundlePathArray addObject:[path stringByAppendingPathComponent:target]];
            }
        }
    }
	
	// now try to load everything
	NSEnumerator *bpaEnum = [_bundlePathArray objectEnumerator];
	while (path = [bpaEnum nextObject]) {
		NSLog(@"Attempting to load: %@", path);
		CFBundleRef bundle = CFBundleCreate(NULL, (CFURLRef)[NSURL URLWithString:path]);
		BOOL loaded = NO;
		if (bundle) {
			if (CFBundleLoadExecutable(bundle)) {
				// see if this is what we want...
				_OVGetLibraryVersion_t *getVersion = (_OVGetLibraryVersion_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVGetLibraryVersion"));
				_OVInitializeLibrary_t *initLib = (_OVInitializeLibrary_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVInitializeLibrary"));
				_OVGetModuleFromLibrary_t *getModule = (_OVGetModuleFromLibrary_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVGetModuleFromLibrary"));
				
				if (getVersion && initLib && getModule) {
					if (getVersion() == OV_VERSION) {
						NSString *resourceDir = [path stringByAppendingPathComponent:@"Resources"];
						if (initLib(_loaderService, [resourceDir UTF8String])) {
							size_t moduleIterator = 0;
							OVModule *module;
							while (module = getModule(moduleIterator)) {
								LVModule *loadedModule = [LVModule moduleWithModuleObject:module moduleDataPath:resourceDir];
								[_loadedModuleDictionary setObject:loadedModule forKey:[loadedModule moduleIdentifier]];
								moduleIterator++;
							}
							
							if (moduleIterator)
								loaded = YES;
						}
					}
				}
			}				
		}
	
		if (loaded) {
			[_loadedModulePackageBundleDictionary setObject:(id)bundle forKey:path];
		}
		else {
			if (bundle) {
				CFBundleUnloadExecutable(bundle);
			}
		}
		
		if (bundle) {
			CFRelease(bundle);
		}
	}
	
	[self _syncConfigurationWithNotification:NO];
}
- (LVService*)loaderService
{
	return _loaderService;
}
- (LVContextSandwich *)createContextSandwich
{
	OVInputMethodContext* inputMethodContext = 0;

	if ([_primaryInputMethodModuleID length]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:_primaryInputMethodModuleID];

		NSDictionary *oldConfigDict = [[_configDictionary copy] autorelease];		
		NSMutableDictionary *configDict = [_configDictionary objectForKey:_primaryInputMethodModuleID];
		if (!configDict) {
			configDict = [NSMutableDictionary dictionary];
			[_configDictionary setObject:configDict forKey:_primaryInputMethodModuleID];
		}
		if ([module lazyInitWithLoaderService:_loaderService configDictionary:configDict]) {
			inputMethodContext = ((OVInputMethod*)[module moduleObject])->newContext();
			
			if (![oldConfigDict isEqualTo:_configDictionary]) {
				[self _writeConfigurationFile];
			}
		}
	}
	
	LVContextSandwich* sandwich = new LVContextSandwich(inputMethodContext);
	return sandwich;
}
- (void)syncConfiguration
{
	[self _syncConfigurationWithNotification:YES];
}

- (LVModule *)moduleForIdentifier:(NSString *)identifier
{
	return [_loadedModuleDictionary objectForKey:identifier];
}

- (void)setPrimaryInputMethodModuleID:(NSString *)moduleID
{
	NSString *tmp = _primaryInputMethodModuleID;
	_primaryInputMethodModuleID = [moduleID copy];
	[tmp release];
	[self _validateAndWriteConfig];
}
- (NSString *)primaryInputMethodModuleID
{
	return _primaryInputMethodModuleID;
}
- (NSArray *)inputMethodTitlesAndModuleIDs
{
	NSMutableArray *result = [NSMutableArray array];
	NSEnumerator *keyEnum = [_loadedModuleDictionary keyEnumerator];
	NSString *key;
	
	while (key = [keyEnum nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:key];		
		if (OVWildcard::Match([module moduleObject]->moduleType(), "OVInputMethod")) {		
			NSString *localizedName = [NSString stringWithUTF8String:[module moduleObject]->localizedName(_loaderService->locale())];
			[result addObject:[NSArray arrayWithObjects:localizedName, key, nil]];
		}
	}
	
	return result;
}

@end

@implementation LVModuleManager (ProtectedMethods)
- (NSString *)userDataPathForModuleID:(NSString *)moduleID
{
    NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSAssert([dirs count], @"NSSearchPathForDirectoriesInDomains");	

	NSString *userPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:OPENVANILLA_NAME];
	BOOL isDir = YES;
	if (![[NSFileManager defaultManager] fileExistsAtPath:userPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:userPath attributes:nil];
	}
	
	NSAssert1(isDir, @"%@ must be a directory", userPath);	
	
    return [[userPath stringByAppendingPathComponent:moduleID] stringByAppendingString:@"/"];
}
@end
