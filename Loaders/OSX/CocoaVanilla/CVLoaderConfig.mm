// CVLoaderConfig.mm

#include "CVLoaderConfig.h"
#include "NSDictionaryExtension.h"

NSArray *CVGetModuleLoadPath() {
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    [a addObject:@"/Library/OpenVanilla/0.7.0/Modules/"];
    [a addObject:@"~/Library/OpenVanilla/0.7.0/Modules/"];
    return a;
}

/* void CVCheckDefaultConfiguration(NSMutableDictionary *d) {
    [d valueForKey:@"primaryInputMethod" default:@""];
    [d valueForKey:@"outputFilters" default:[[NSMutableArray new] autorelease]];
} */
