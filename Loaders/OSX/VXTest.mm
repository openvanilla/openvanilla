#include <Cocoa/Cocoa.h>
#include "VXLoader.h"

#define OVDEBUG
#include <OpenVanilla/OVUtility.h>
#include "VXOpenVanilla.h"

int main()
{
    [NSAutoreleasePool new];
    
    NSMutableArray *a=[[NSMutableArray new] autorelease];
    [a addObject: VXSTR("/Users/lukhnos/logiciels/openvanilla/Modules/IMExample")];
    
    NSMutableDictionary *dict=[[NSMutableDictionary new] autorelease];
    VXDictionary *vxd=new VXDictionary(dict);
    VXService *srv=new VXService;
    NSArray *modlist=VXLoadEverything(a, vxd, srv);
    
    murmur([[modlist description] UTF8String]);
    
    NSEnumerator *enm=[modlist objectEnumerator];
    VXModuleWrapper *m;
    
    while (m=[enm nextObject])
    {
        murmur("%s: ", [[m identifier] UTF8String]);
        if ([m initialize:vxd localPref:vxd service:srv userSpaceRoot:@"/tmp"])
            murmur ("passed");
        else
            murmur ("failed");
    }
    
    return 0;
}

void testEnumerate()
{
    NSArray *a=VXEnumeratePath(@"/Users/lukhnos/logiciels/openvanilla/Modules/IMExample", @".dylib");
//    NSArray *a=VXEnumeratePath(@"/usr/include", @".h");
    murmur([[a description] UTF8String]);
}

void testLibLoad()
{
//    OVLoadedLibrary *l=VXLoadLibraryFromDylib(@"/Users/lukhnos/logiciels/openvanilla/Modules/IMExample/IMExample.dylib");

    OVLoadedLibrary *l=VXLoadLibraryFromBundle(@"/Users/lukhnos/logiciels/openvanilla/Modules/IMExample/IMExample.bundle");
    
    OVModule *m=l->getModule(0);
    
    murmur("identifier: %s, type: %s", m->identifier(), m->moduleType());
}

void testMilk()
{
    NSMutableDictionary *dict=[NSMutableDictionary new];
    
    NSString *p;
    OVLoadedLibrary *l;
    NSMutableArray *a=[NSMutableArray new];

    p=@"/Users/lukhnos/logiciels/openvanilla/Modules/IMExample/IMExample.dylib";
    l=VXLoadLibraryFromDylib(p);
    [a addObjectsFromArray: VXMilkModulesFromLibrary(p, l, dict)];    
    murmur("currently in array:\n%s\n", [[a description] UTF8String]); 
    
    p=[NSString stringWithUTF8String: "/Users/lukhnos/logiciels/openvanilla/Modules/IMExample/IM簡單.app"];
    l=VXLoadLibraryFromBundle(p);
    if (!l) abort();
    
    [a addObjectsFromArray: VXMilkModulesFromLibrary(p, l, dict)];    
    murmur("currently in array:\n%s\n", [[a description] UTF8String]);
}