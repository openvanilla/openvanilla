// CVLoaderConfig.h

#ifndef __CVLoaderConfig_h
#define __CVLoaderConfig_h

#include <Cocoa/Cocoa.h>

#define CVLC_SVN                "$Rev$"
#define CVLC_USERSPACE_PATH		@"~/Library/OpenVanilla/0.7.0/UserSpace/"
#define CVLC_USERCONFIG_PATH	@"~/Library/OpenVanilla/0.7.0/Preferences/"
#define CVLC_USERCONFIG_FILE	@"~/Library/OpenVanilla/0.7.0/Preferences/org.openvanilla.070.plist"
#define CVLC_DISPLAYSERVER		@"/Library/OpenVanilla/0.7.0/OVLoader/OVDisplayServer.app"
#define CVLC_ATOMIC_INIT_FILE   @"atomic-init-lock"
#define CVLC_ATOIMC_ERROR_FILE  @"/tmp/_openvanilla-atomicinit-error.html"
#define CVLC_FACTORY_DEFAULTS   @"/Library/OpenVanilla/0.7.0/OVLoader/OVLoader.bundle/Contents/Resources/org.openvanilla.OVLoader.FactorySettings.plist"
#define CVLC_PREFERENCEUTIL     @"/Library/OpenVanilla/0.7.0/OVLoader/OVPreferences.app"

NSArray *CVGetModuleLoadPath();
NSString *CVGetUserSpacePath();
NSString *CVGetUserConfigFilename();
NSString *CVGetAtomicInitLockFilename();
NSString *CVGetAtomicInitErrorMessageFilename();
NSDictionary *CVGetDisplayServerConfig();

// #define CVLC_USERSPACE_PATH     @"/tmp/"
// #define CVLC_USERCONFIG         @"/tmp/org.openvanilla.070.plist"
// void CVCheckDefaultConfiguration(NSMutableDictionary *d);
// void CVCreateIntermediateDirectory(NSString *p);

#endif
