// VXLoaderUtility.h

#ifndef __VXLoaderUtility_h
#define __VXLoaderUtility_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include "VXWrappers.h"

// returns an array of all loaded modules, which are milked from loaded libraries,
// which are in turn loaded from the paths indicated in the paths array
NSArray* VXLoadEverything(NSArray *paths, OVDictionary *globalPref, OVService *srv);

VXModuleWrapper *VXFindModule(NSArray *modlist, NSString *identifier, NSString *type=NULL);

// this returns (and creates by the way) a "module user data" directory
// by appending the module identifer after the user space root
// e.g. ~/
NSString *VXGetUserDataPath(VXModuleWrapper *w, NSString *userspaceroot);


OVLoadedLibrary *VXLoadLibraryFromBundle(NSString *p);
OVLoadedLibrary *VXLoadLibraryFromDylib(NSString *p);

// this create an autoreleased NSArray object storing enumerated pathnames
NSArray *VXEnumeratePath(NSString *path, NSString *ext);

// we still need the full pathname of the loaded library because we need
// to extract the loaded path (and then append /Contents for .bundle!)
// that is required for the loaded modules in the initialization process;
// we also need to supply a dictionary to prevent module-id conflicts
NSString *VXGetRealLoadedPath(NSString *libName);

NSArray *VXMilkModulesFromLibrary(NSString *libName, OVLoadedLibrary *lib, 
    NSMutableDictionary *nameDict);    


#endif
