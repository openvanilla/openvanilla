// CVLoaderUtility.h

#ifndef __CVLoaderUtility_h
#define __CVLoaderUtility_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include "CVWrappers.h"

// returns an array of all loaded modules, which are milked from loaded libraries,
// which are in turn loaded from the paths indicated in the paths array
NSArray* CVLoadEverything(NSArray *paths, OVService *srv, NSArray *libexcludelist=nil, NSArray *modexcludelist=nil, NSMutableDictionary *history=nil, NSString *atomic=nil);

CVModuleWrapper *CVFindModule(NSArray *modlist, NSString *identifier, NSString *type=nil);
NSArray *CVFindModules(NSArray *modlist, NSArray *idlist, NSString *type=nil);
NSArray *CVGetModulesByType(NSArray *modlist, NSString *type);

// atomic init related
void CVAtomicInitStart(NSString *f, NSString *libname);
void CVAtomicInitEnd(NSString *f);

// utility functions
BOOL CVIsPathExist(NSString *p);
BOOL CVStringIsInArray(NSString *s, NSArray *a);

#endif
