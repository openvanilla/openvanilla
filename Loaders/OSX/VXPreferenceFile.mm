// VXPreference.mm

#include <Cocoa/Cocoa.h>
#include "VXPreferenceFile.h"
#include "VXConstants.h"

VXTimeStamp VXGetTimeStamp(const char *f)
{
    struct stat s;
    VXTimeStamp t;
    bzero(&t, sizeof(t));
    if (f && !stat(f, &s)) t=s.st_mtimespec;
    return t;
}

int operator==(const VXTimeStamp &s1, const VXTimeStamp &s2)
{
    return !memcmp(&(VXTimeStamp&)s1, &(VXTimeStamp&)s2, sizeof(VXTimeStamp));
}

VXPreferenceFile::VXPreferenceFile(const char *f)
{
    preffile=[[[NSString stringWithUTF8String:f] stringByStandardizingPath] retain];
    dictroot=[NSMutableDictionary new];
    
    VXSETUPDEFAULTPREFERENCE(dictroot);
}

VXPreferenceFile::~VXPreferenceFile()
{
    [preffile release];
    [dictroot release];
}

VXTimeStamp VXPreferenceFile::read()
{
    NSData *d=[NSData dataWithContentsOfFile:preffile];
    if (d)
    {        
        NSString *errMsg;
        NSPropertyListFormat fmt;
        id p=[NSPropertyListSerialization propertyListFromData:d
            mutabilityOption:NSPropertyListMutableContainersAndLeaves 
            format:&fmt errorDescription:&errMsg];

        if (p) 
        {
            if ([p isKindOfClass: [NSMutableDictionary class]])
                [dictroot addEntriesFromDictionary: p];
        }   
    }
    return timeStamp();
}

VXTimeStamp VXPreferenceFile::write()
{
    NSString *errMsg;
    NSData *d=[NSPropertyListSerialization dataFromPropertyList:dictroot
        format:NSPropertyListXMLFormat_v1_0 errorDescription:&errMsg];
    [d writeToFile:preffile atomically:TRUE];
    return timeStamp();
}

VXTimeStamp VXPreferenceFile::timeStamp()
{
    return VXGetTimeStamp([preffile UTF8String]);
}

NSMutableDictionary * VXPreferenceFile::getDictionary(NSString *name)
{
    NSMutableDictionary *md=[dictroot objectForKey:name];
    if (!md)
    {
        md=[NSMutableDictionary new];
        [dictroot setObject:md forKey:name];
        [md release];
    }
    return md;
}
