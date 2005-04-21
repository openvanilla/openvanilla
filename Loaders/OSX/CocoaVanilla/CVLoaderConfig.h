// CVLoaderConfig.h

#ifndef __CVLoaderConfig_h
#define __CVLoaderConfig_h

#include <Cocoa/Cocoa.h>

#define CVLC_SVN                "$Rev$"

NSArray *CVGetModuleLoadPath();

#define CVLC_USERSPACE_PATH     @"/tmp/"
#define CVLC_USERCONFIG         @"/tmp/org.openvanilla.070.plist"

// void CVCheckDefaultConfiguration(NSMutableDictionary *d);
// void CVCreateIntermediateDirectory(NSString *p);

#endif
