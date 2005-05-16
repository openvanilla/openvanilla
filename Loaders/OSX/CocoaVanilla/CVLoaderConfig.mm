// CVLoaderConfig.mm

#include "CVLoaderConfig.h"
#include "CVLoaderUtility.h"
#include "NSDictionaryExtension.h"

void CVPreparePath() {
	NSString *up=[CVLC_USERSPACE_PATH stringByStandardizingPath];
	NSString *cp=[CVLC_USERCONFIG_PATH stringByStandardizingPath];
	
    if (!CVIsPathExist(up)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", up]);
		system([[NSString stringWithFormat:@"mkdir -p %@", up] UTF8String]);
	}
    if (!CVIsPathExist(cp)) {
		NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", cp]);
		system([[NSString stringWithFormat:@"mkdir -p %@", cp] UTF8String]);
	}
}

NSArray *CVGetModuleLoadPath() {
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    [a addObject:@"/Library/OpenVanilla/0.7.0/Modules/"];
    [a addObject:@"~/Library/OpenVanilla/0.7.0/Modules/"];
    return a;
}

NSString *CVGetUserSpacePath() {
	CVPreparePath();
	// NSLog([CVLC_USERSPACE_PATH stringByStandardizingPath]);
	return [CVLC_USERSPACE_PATH stringByStandardizingPath];
}

NSString *CVGetAtomicInitLockFilename() {
    return [CVGetUserSpacePath() stringByAppendingPathComponent:CVLC_ATOMIC_INIT_FILE];
}

NSString *CVGetUserConfigFilename() {
	CVPreparePath();
	// NSLog([CVLC_USERCONFIG_FILE stringByStandardizingPath]);
	return [CVLC_USERCONFIG_FILE stringByStandardizingPath];
}

NSDictionary *CVGetDisplayServerConfig() {
	NSMutableDictionary *d=[[[NSMutableDictionary alloc] init] autorelease];
	[d setValue:@"" forKey:@"backgroundImage"];
	[d setValue:@"1.0 1.0 1.0" forKey:@"foreground"];
	[d setValue:@"0.15 0.15 0.15" forKey:@"background"];
	[d setValue:@"0.9" forKey:@"opacity"];
	[d setValue:@"Lucida Grande" forKey:@"font"];
	[d setValue:@"20" forKey:@"size"];
    [d setValue:@"default" forKey:@"notificationStyle"];
	return d;
}

NSString *CVGetAtomicInitErrorMessageFilename() {
    return CVLC_ATOIMC_ERROR_FILE;
}
