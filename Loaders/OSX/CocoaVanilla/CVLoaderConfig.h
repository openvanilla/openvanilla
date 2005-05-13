// CVLoaderConfig.h

#ifndef __CVLoaderConfig_h
#define __CVLoaderConfig_h

#include <Cocoa/Cocoa.h>

#define CVLC_SVN                "$Rev$"

NSArray *CVGetModuleLoadPath();

#define CVLC_USERSPACE_PATH		@"~/Library/OpenVanilla/0.7.0/UserSpace/"
#define CVLC_USERCONFIG_PATH	@"~/Library/OpenVanilla/0.7.0/Preferences/"
#define CVLC_USERCONFIG_FILE	@"~/Library/OpenVanilla/0.7.0/Preferences/org.openvanilla.070.plist"

NSString *CVGetUserSpacePath();
NSString *CVGetUserConfigFilename();

// #define CVLC_USERSPACE_PATH     @"/tmp/"
// #define CVLC_USERCONFIG         @"/tmp/org.openvanilla.070.plist"
// void CVCheckDefaultConfiguration(NSMutableDictionary *d);
// void CVCreateIntermediateDirectory(NSString *p);

#endif
