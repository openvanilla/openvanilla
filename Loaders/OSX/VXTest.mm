#include <Cocoa/Cocoa.h>
// #include "VXLoader.h"

#define OVDEBUG
#include <OpenVanilla/OVUtility.h>
// #include "VXOpenVanilla.h"
#include "VXUtility.h"
#include "VXPreferenceFile.h"

int main()
{
    [NSAutoreleasePool new];
    
    VXPreferenceFile pref("/tmp/pref.plist");
    
    VXTimeStamp s1=pref.read();
    
    printf ("soundEffect=%s\n",
        [[pref.getDictionary("OVLoader") objectForKey:@"soundEffect"] UTF8String]);
    
    NSMutableArray *a=[NSMutableArray new];
    [pref.getDictionary("OVLoader") setObject:a forKey:@"array"];

    a=[NSMutableArray new];
    [a addObject: [NSString stringWithUTF8String: "中文字串"]];
    [a addObject: @"456"];
    [a addObject: @"789"];    
    [pref.getDictionary("OVLoader") setObject:a forKey:@"array2"];
    
    [pref.getDictionary("test") setObject:a forKey:@"arr"];
    
    VXTimeStamp s2=pref.write();
    
    if (s1==s2)
    {
        printf ("equal!\n");
    }
    else
    {
        printf ("not equal!\n");
    }

    
    
/*    NSData *dd=[NSData dataWithContentsOfFile: @"/tmp/test"];
    if (dd) printf ("dd exists"); else printf ("dd null!");
    return 1;
    
    system("echo tt > /tmp/test");
    VXTimeStamp s=VXGetTimeStamp("/tmp/test");    

    system("echo tt > /tmp/test");
    VXTimeStamp s1=VXGetTimeStamp("/tmp/test");
    
    if (s==s1)
    {
        printf ("equal!\n");
    }
    else
    {
        printf ("not equal!\n");
    }
        
    */
      
/*    NSMutableDictionary *md=[NSMutableDictionary new];
    
    NSMutableDictionary *loader=[NSMutableDictionary new];
    [loader setObject:@"1" forKey: @"sound"];
    [loader setObject:[NSString stringWithUTF8String: "中文字串"] 
        forKey: @"chnString"];
    [md setObject:loader forKey: @"ovloader"];
  
    NSString *errMsg;
    NSData *d=[NSPropertyListSerialization dataFromPropertyList:md 
        format:NSPropertyListXMLFormat_v1_0 errorDescription:&errMsg];
        
    [d writeToFile: @"/tmp/test.plist" atomically: TRUE]; */
    
/*    NSData *d=[NSData dataWithContentsOfFile: @"/tmp/test.plist"];
    
    
    NSString *errMsg;
    NSPropertyListFormat fmt;
    id p=[NSPropertyListSerialization propertyListFromData:d
        mutabilityOption:NSPropertyListMutableContainersAndLeaves 
        format:&fmt  // NSPropertyListXMLFormat_v1_0
        errorDescription:&errMsg];
    
    NSMutableDictionary *mm=[p objectForKey: @"ovloader"];
      
    [p setObject: @"test" forKey: @"testKey"];
    printf ("%s", [[mm objectForKey: @"chnString"] UTF8String]);
    
    printf ("%s", [[p description] UTF8String]); */
        
//    printf ("%s", [[[p class] description] UTF8String]);
    
    
//    printf ("%s", [[md description] UTF8String]);
    
    return 0;
}

/*
void testLoadEverything()
{
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
        if ([m initialize:vxd localPref:vxd service:srv userSpace:@"/tmp"])
            murmur ("passed");
        else
            murmur ("failed");
    }
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
*/
