// VXLoader.h: OpenVanilla Loader for OS X interfaces

#ifndef __VXLoader_h
#define __VXLoader_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>

#define VXMODLIBPATH        "/Library/OpenVanilla/0.7.0-pre/Modules/"
#define VXUSRMODLIBPATH     "~/Library/OpenVanilla/0.7.0-pre/Modules/"
#define VXLIBUSRSPACEPREFIX "~/Library/OpenVanilla/0.7.0-pre/ModuleData/"

// implemented in VXLibraryLoader.mm
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

// returns an array of all loaded modules, which are milked from loaded libraries,
// which are in turn loaded from the paths indicated in the paths array
NSArray* VXLoadEverything(NSArray *paths, OVDictionary *globalPref, OVService *srv);

// NSString *VXGetUserDataPath(VXModuleWrapper *w, NSString *userspaceroot);

@interface VXModuleWrapper : NSObject
{
    OVModule *mod;
    BOOL inited;
    NSString *loadedPath;
    
    // BOOL canUse;
}
-(void)dealloc;
-(id)initWithModule:(OVModule*)m libraryPath:(NSString*)p;
-(NSString*)description;
-(NSString*)moduleType;
-(NSString*)identifier;
-(OVModule*)module;
-(OVModule*)setModule:(OVModule*)m libraryPath:(NSString*)p;
-(BOOL)initialize:(OVDictionary*)globalPref localPref:(OVDictionary*)pref
 service:(OVService*)s userSpaceRoot:(NSString*)u;
@end


#endif

/*

struct VXLoaderData
{
    MenuRef menu;
    VXConfig *cfg;
    
    VXInfoBoxController *ibctrl;
    NSWindow *ib;
};

*/