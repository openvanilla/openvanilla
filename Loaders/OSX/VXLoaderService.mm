// VXLoaderService.mm

#include "VXLoader.h"
#include <dlfcn.h>

#define OVDEBUG
#include <OpenVanilla/OVUtility.h>

#include "VXUtility.h"

OVLoadedLibrary *VXLoadLibraryFromBundle(NSString *p)
{
    const char *func="VXLoadLibraryFromOSXBundle";
    murmur("%s: loading library (OS X bundle fashion) %s", func, [p UTF8String]);
    
    NSURL *url=[NSURL fileURLWithPath: [p stringByExpandingTildeInPath]];
    if (!url) return NULL;

    CFBundleRef libref=CFBundleCreate(NULL, (CFURLRef)url);
    if (!libref)
    {
        murmur("%s: failed loading library %s", func, [p UTF8String]);
        return NULL;
    }

    _OVGetLibraryVersion_t *g;
    _OVInitializeLibrary_t *i;
    _OVGetModuleFromLibrary_t *m;
    
    #define GETPOINTER(x) CFBundleGetFunctionPointerForName(libref, CFSTR(x))
    if (!(g=(_OVGetLibraryVersion_t*)GETPOINTER("OVGetLibraryVersion")) ||
        !(i=(_OVInitializeLibrary_t*)GETPOINTER("OVInitializeLibrary")) ||
        !(m=(_OVGetModuleFromLibrary_t*)GETPOINTER("OVGetModuleFromLibrary")))
    {
        murmur("%s: incompatible interface (library %s)", func, [p UTF8String]);
        return NULL;
    }
    #undef GETPOINTER
    
    // check if the loaded library's version is older than ours
    if (g() < OVVERSION)
    {
        murmur("%s: version too old (library %s)", func, [p UTF8String]);
        return NULL;
    }
    
    OVLoadedLibrary *l=new OVLoadedLibrary(i, m);
    // we don't release that CFBundleRef as we can't, and we don't retain it
    // (in OVLoadedLibrary) as it's meaningless (it's impossible to unload it
    // and reload it anyway)
    return l;
}

// this is actually platform-independent
OVLoadedLibrary *VXLoadLibraryFromDylib(NSString *p)
{
    const char *func="VXLoadLibraryFromDylib";
    murmur("%s: loading library (.dylib fashion) %s", func, [p UTF8String]);
    
    void *libh=dlopen([[p stringByExpandingTildeInPath] UTF8String], RTLD_LAZY);
    if (!libh)
    {
        murmur("%s: failed loading library %s", func, [p UTF8String]);
        return NULL;
    }

    _OVGetLibraryVersion_t *g;
    _OVInitializeLibrary_t *i;
    _OVGetModuleFromLibrary_t *m;
    
    g=(_OVGetLibraryVersion_t*)dlsym(libh, "OVGetLibraryVersion");
    i=(_OVInitializeLibrary_t*)dlsym(libh, "OVInitializeLibrary");
    m=(_OVGetModuleFromLibrary_t*)dlsym(libh, "OVGetModuleFromLibrary");
    
    if (!g || !i || !m) return NULL;

    if (g() < OVVERSION)
    {
        murmur("%s: version too old (library %s)", func, [p UTF8String]);
        return NULL;
    }
    
    OVLoadedLibrary *l=new OVLoadedLibrary(i, m);
    return l;
}

NSArray *VXEnumeratePath(NSString *path, NSString *ext)
{
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    NSDirectoryEnumerator *direnum = [[NSFileManager defaultManager]
        enumeratorAtPath:path];
    while (NSString *pname = [direnum nextObject])
    {
        if ([pname hasSuffix: ext]) 
            [a addObject: [path stringByAppendingPathComponent: pname]];
        // tell the enumerator not to descend into a possible path
        [direnum skipDescendents];
    }
    return a;
}

NSString *VXGetRealLoadedPath(NSString *libName)
{
    if ([libName hasSuffix: @".bundle"] || [libName hasSuffix: @".app"])
        return [[libName stringByAppendingPathComponent: @"Contents"] 
            stringByAppendingString: @"/"];
    return [[libName stringByDeletingLastPathComponent] 
        stringByAppendingString: @"/"];
}

NSArray *VXMilkModulesFromLibrary(NSString *libName, OVLoadedLibrary *lib, 
    NSMutableDictionary *nameDict)
{
    const char *func="VXMilkModulesFromLibrary";
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    
    NSString *shortName=[libName lastPathComponent];
    NSString *realPath=VXGetRealLoadedPath(libName);

//    murmur("%s: milking modules from library %s, path supplied to module=%s",
//        func, [shortName UTF8String], [realPath UTF8String]);
    
    for(int idx=0; OVModule *m=lib->getModule(idx); idx++)
    {
        murmur("%s: loading module idx %d (module id=%s) from library %s", 
            func, idx, m->identifier(), [shortName UTF8String]);
            
        NSString *i=VXSTR(m->identifier());
        if ([nameDict objectForKey: i])
        {
            murmur("%s: module '%s' already exists!", func, m->identifier());
        }
        else
        {
            [nameDict setObject: @"1" forKey: i];
            [a addObject: [[[VXModuleWrapper alloc] 
                initWithModule:m libraryPath:realPath] autorelease]];
        }
    }
    return a;
}

NSArray* VXLoadEverything(NSArray *paths, OVDictionary *globalPref, OVService *srv)
{
    const char *func="VXLoadEveryThing";
    
    // (add statically linked modules)
    NSMutableArray *libList=[[NSMutableArray new] autorelease];
    NSMutableArray *modList=[[NSMutableArray new] autorelease];
    NSMutableDictionary *dict=[[NSMutableDictionary new] autorelease];
    
    NSEnumerator *enm=[paths objectEnumerator];
    NSString *i;
    while (i=[enm nextObject])
    {
        [libList addObjectsFromArray:VXEnumeratePath(i, @".bundle")];
        [libList addObjectsFromArray:VXEnumeratePath(i, @".app")];
        [libList addObjectsFromArray:VXEnumeratePath(i, @".dylib")];
    }    

    // load everything
    enm=[libList objectEnumerator];
    while (i=[enm nextObject])
    {
        OVLoadedLibrary *l;
        NSString *rlp=VXGetRealLoadedPath(i);
        if ([i hasSuffix: @".dylib"]) l=VXLoadLibraryFromDylib(i);
            else l=VXLoadLibraryFromBundle(i);    
        if (!l) continue;   // err message already supplied
        
        if (!l->initialize(globalPref, srv, [rlp UTF8String], "/"))
        {
            murmur("%s: library initialization failed (%s), module milking ignored",
                func, [i UTF8String]);
            continue;
        }
        
        [modList addObjectsFromArray: VXMilkModulesFromLibrary(i, l, dict)];   
    }
        
    return modList;
}

@implementation VXModuleWrapper
-(id)initWithModule:(OVModule*)m libraryPath:(NSString*)p
{
    self=[super init];
    if (self)
    {
        inited=FALSE;
        [self setModule:m libraryPath:p];
    }
    return self;
}
-(void)dealloc
{
    if (loadedPath) [loadedPath release];
    [super dealloc];
}
-(NSString*)description
{
    if (!mod) return @"";
    return [NSString stringWithFormat: 
        @"(module id=%@, type=%s, module path=%@, initialized=%s)",
        VXSTR(mod->identifier()), mod->moduleType(), 
        loadedPath, inited ? "TRUE" : "FALSE"];
}
-(NSString*)moduleType
{
    return mod ? VXSTR(mod->moduleType()) : @"";
}
-(NSString*)identifier
{
    return mod ? VXSTR(mod->identifier()) : @"";
}
-(OVModule*)module
{
    return mod;
}
-(OVModule*)setModule:(OVModule*)m libraryPath:(NSString*)p
{
    mod=m;
    if (loadedPath) [loadedPath release];
    loadedPath=p;
    [loadedPath retain];
    return mod;
}
-(BOOL)initialize:(OVDictionary*)globalPref localPref:(OVDictionary*)pref
 service:(OVService*)s userSpaceRoot:(NSString*)u
{
    if (inited) return FALSE;
    inited=TRUE;
    
    // we append the identifer to the userSpaceRoot parameter
    NSString *up=
        [[u stringByAppendingPathComponent: VXSTR(mod->identifier())]
            stringByAppendingString: @"/"];
    system([[NSString stringWithFormat: @"mkdir %@", up] UTF8String]);
    return (BOOL)mod->initialize(globalPref, pref, s, [loadedPath UTF8String], 
        [up UTF8String], "/");
}
@end
