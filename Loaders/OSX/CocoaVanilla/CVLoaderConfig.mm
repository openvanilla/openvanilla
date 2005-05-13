// CVLoaderConfig.mm

#include "CVLoaderConfig.h"
#include "NSDictionaryExtension.h"
#include <sys/stat.h>

void CVPreparePath() {
	NSString *up=[CVLC_USERSPACE_PATH stringByStandardizingPath];
	NSString *cp=[CVLC_USERCONFIG_PATH stringByStandardizingPath];
	
	struct stat st;
	if (stat([up UTF8String], &st)) {
		// NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", up]);
		system([[NSString stringWithFormat:@"mkdir -p %@", up] UTF8String]);
	}
	if (stat([cp UTF8String], &st)) {
		// NSLog([NSString stringWithFormat:@"path %@ doesn't exist, creating", cp]);
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

NSString *CVGetUserConfigFilename() {
	CVPreparePath();
	// NSLog([CVLC_USERCONFIG_FILE stringByStandardizingPath]);
	return [CVLC_USERCONFIG_FILE stringByStandardizingPath];
}


/* void CVCheckDefaultConfiguration(NSMutableDictionary *d) {
    [d valueForKey:@"primaryInputMethod" default:@""];
    [d valueForKey:@"outputFilters" default:[[NSMutableArray new] autorelease]];
} */
