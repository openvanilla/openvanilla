// VXPreferenceFile.h

#ifndef __VXPreferenceFile_h
#define __VXPreferenceFile_h

#include <Cocoa/Cocoa.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

typedef struct timespec VXTimeStamp;

VXTimeStamp VXGetTimeStamp(const char *f);
int operator==(const VXTimeStamp &s1, const VXTimeStamp &s2);

class VXPreferenceFile
{
public:
    VXPreferenceFile(const char *f);
    ~VXPreferenceFile();
    VXTimeStamp read();
    VXTimeStamp write();
    VXTimeStamp timeStamp();
    NSMutableDictionary *getDictionary(NSString *name);
    
protected:
    NSString *preffile;
    NSMutableDictionary *dictroot;
};

#endif
