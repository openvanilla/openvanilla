// CVLoaderService.mm

#define OV_DEBUG
#include "CVLoaderUtility.h"
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <OpenVanilla/OVUtility.h>

OVLoadedLibrary *CVLoadLibraryFromBundle(NSString *p);
OVLoadedLibrary *CVLoadLibraryFromDylib(NSString *p);

// this equals "find /path/*.ext", returns a 
NSArray *CVEnumeratePath(NSString *path, NSString *ext);

// a utility function
BOOL CVStringIsInArray(NSString *s, NSArray *a);

// we still need the full pathname of the loaded library because we need
// to extract the loaded path (and then append /Contents for .bundle!)
// that is required for the loaded modules in the initialization process;
// we also need to supply a dictionary to prevent module-id conflicts
NSString *CVGetRealLoadedPath(NSString *libname);
NSArray *CVMilkModulesFromLibrary(NSString *libname, OVLoadedLibrary *lib, NSMutableDictionary *namedict, NSMutableArray *history);

NSArray* CVLoadEverything(NSArray *paths, OVService *srv, NSArray *libexcludelist, NSArray *modexcludelist, NSMutableDictionary *history, NSString *atomic)
{
    const char *func="CVLoadEveryThing";
    
    // (add statically linked modules)
    NSMutableArray *libList=[[NSMutableArray new] autorelease];
    NSMutableArray *modList=[[NSMutableArray new] autorelease];
    NSMutableDictionary *dict=[[NSMutableDictionary new] autorelease];
    NSMutableDictionary *histdict=[[NSMutableDictionary new] autorelease];

    NSEnumerator *enm=[paths objectEnumerator];
    NSString *i;
    while (i=[enm nextObject]) {
        [libList addObjectsFromArray:CVEnumeratePath(i, @".bundle")];
        [libList addObjectsFromArray:CVEnumeratePath(i, @".app")];
        [libList addObjectsFromArray:CVEnumeratePath(i, @".dylib")];
    }    

    // add module-exclusion list into the named dict
    if (modexcludelist) {
        int c=[modexcludelist count];
        for (int i=0; i<c; i++) [dict setValue:@"1" forKey:[modexcludelist objectAtIndex:i]];
    }

    // load everything
    enm=[libList objectEnumerator];
    while (i=[enm nextObject]) {
        if (CVStringIsInArray([i lastPathComponent], libexcludelist)) {
            NSLog(@"library %@ in exclude list, not loaded", [i lastPathComponent]);
            continue;
        }

        CVAtomicInitStart(atomic, i);
        OVLoadedLibrary *l;
        NSString *rlp=CVGetRealLoadedPath(i);
        if ([i hasSuffix: @".dylib"]) l=CVLoadLibraryFromDylib(i); else l=CVLoadLibraryFromBundle(i);    
        if (!l) continue;   // err message already supplied
        
        if (!l->initialize(srv, [rlp UTF8String])) {
            murmur("%s: library initialization failed (%s), module milking ignored", func, [i UTF8String]);
            continue;
        }

        NSMutableArray *ha=[[NSMutableArray new] autorelease];
        [modList addObjectsFromArray: CVMilkModulesFromLibrary(i, l, dict, ha)];
        [histdict setValue:ha forKey:i];
        CVAtomicInitEnd(atomic);
    }
        
    if (history) [history addEntriesFromDictionary:histdict];
    return modList;
}

void CVAtomicInitStart(NSString *f, NSString *libname) {
    if (!f) return;
    // NSError *err;
    // NSLog(@"writing atomicinit file %@ for lib %@", f, libname);

    // this won't work in OS X 10.3
    // [libname writeToFile:f atomically:NO encoding:NSUTF8StringEncoding error:&err];

    [libname writeToFile:f atomically:NO];
}

void CVAtomicInitEnd(NSString *f) {
    if (!f) return;
    if (CVIsPathExist(f)) {
        // NSLog(@"unlinking existing atomicinit file: %@", f);
        unlink([f UTF8String]);
    }
}

BOOL CVStringIsInArray(NSString *s, NSArray *a) {
    if (!a) return NO;
    int c=[a count];
    for (int i=0; i<c; i++) if ([s isEqualToString:[a objectAtIndex: i]]) return YES;
    return NO;
}

CVModuleWrapper *CVFindModule(NSArray *modlist, NSString *identifier, 
    NSString *type)
{
    NSEnumerator *enm=[modlist objectEnumerator];
    while (CVModuleWrapper *m=[enm nextObject]) {
        if ([[m identifier] isEqualToString:identifier]) {
            if (!type) return m;
            if (type) {
                if ([[m moduleType] isEqualToString:type]) return m;
            }
        }
    }
    return NULL;
}

NSArray *CVFindModules(NSArray *modlist, NSArray *idlist, NSString *type)
{
    NSMutableArray *ma=[[NSMutableArray new] autorelease];
    NSEnumerator *e=[idlist objectEnumerator];
    id o;
    while (o=[e nextObject]) {
        CVModuleWrapper *w=CVFindModule(modlist, (NSString*)o, type);
        if (w) [ma addObject:w];
    }
    return ma;
}

NSArray *CVGetModulesByType(NSArray *modlist, NSString *type)
{
    NSMutableArray *ma=[[NSMutableArray new] autorelease];
    NSEnumerator *enm=[modlist objectEnumerator];
    while (CVModuleWrapper *m=[enm nextObject]) {
        if ([[m moduleType] isEqualToString:type]) [ma addObject: m];
    }
    return ma;
}

OVLoadedLibrary *CVLoadLibraryFromBundle(NSString *p)
{
    const char *func="CVLoadLibraryFromOSXBundle";
    // murmur("%s: loading library (OS X bundle fashion) %s", func, [p UTF8String]);
    
    NSURL *url=[NSURL fileURLWithPath: [p stringByExpandingTildeInPath]];
    if (!url) return NULL;

    CFBundleRef libref=CFBundleCreate(NULL, (CFURLRef)url);
    if (!libref) {
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
    if (g() < OV_VERSION) {
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
OVLoadedLibrary *CVLoadLibraryFromDylib(NSString *p)
{
    const char *func="CVLoadLibraryFromDylib";
    // murmur("%s: loading library (.dylib fashion) %s", func, [p UTF8String]);
    
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

    if (g() < OV_VERSION)
    {
        murmur("%s: version too old (library %s)", func, [p UTF8String]);
        return NULL;
    }
    
    OVLoadedLibrary *l=new OVLoadedLibrary(i, m);
    return l;
}

NSArray *CVEnumeratePath(NSString *path, NSString *ext)
{
    NSString *stdpath=[path stringByStandardizingPath];
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    NSDirectoryEnumerator *direnum = [[NSFileManager defaultManager]
        enumeratorAtPath:stdpath];
    while (NSString *pname = [direnum nextObject]) {
        if ([pname hasSuffix: ext]) 
            [a addObject: [stdpath stringByAppendingPathComponent: pname]];
        // tell the enumerator not to descend into a possible path
        [direnum skipDescendents];
    }
    return a;
}

NSString *CVGetRealLoadedPath(NSString *libname)
{
    if ([libname hasSuffix: @".bundle"] || [libname hasSuffix: @".app"])
        return [[libname stringByAppendingPathComponent: @"Contents/Resources"] 
            stringByAppendingString: @"/"];
    return [[libname stringByDeletingLastPathComponent] 
        stringByAppendingString: @"/"];
}

NSArray *CVMilkModulesFromLibrary(NSString *libname, OVLoadedLibrary *lib, NSMutableDictionary *namedict, NSMutableArray *history)
{
    const char *func="CVMilkModulesFromLibrary";
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    
    // NSString *shortName=[libname lastPathComponent];
    NSString *realPath=CVGetRealLoadedPath(libname);

    for(int idx=0; OVModule *m=lib->getModule(idx); idx++)
    {
        // murmur("%s: loading module idx %d (module id=%s) from library %s", func, idx, m->identifier(), [shortName UTF8String]);
            
        NSString *i=[NSString stringWithUTF8String:m->identifier()];
        if ([namedict objectForKey: i]) {
            murmur("%s: module '%s' already exists or in exclude-list!", func, m->identifier());
        }
        else {
            [namedict setObject: @"1" forKey: i];
            [a addObject: [[[CVModuleWrapper alloc] initWithModule:m loadedPath:realPath fromLibrary:libname] autorelease]];
            if (history) [history addObject:i];
        }
    }
    return a;
}

BOOL CVIsPathExist(NSString *p) {
    struct stat st;
    if (stat([[p stringByStandardizingPath] UTF8String], &st)) return NO;
    return YES;
}
