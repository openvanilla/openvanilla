// VXConstants.h

#ifndef __VXConstants_h
#define __VXConstants_h

#define VXVERSION           "0.7.0"
#define VXMODLIBPATH         "/Library/OpenVanilla/" VXVERSION "/Modules/"
#define VXUSRMODLIBPATH     "~/Library/OpenVanilla/" VXVERSION "/Modules/"
#define VXLIBUSRSPACEPREFIX "~/Library/OpenVanilla/" VXVERSION "/UserData/"
// #define VXPREFPATH          "~/Library/OpenVanilla/" VXVERSION "/OpenVanilla.plist"

#define VXPREFPATH          "/tmp/test.plist"

#define VXLOADERPREFKEY     "OVLoader"
#define VXSETUPDEFAULTPREFERENCE(x) \
    NSMutableDictionary *_d=[NSMutableDictionary new]; \
    [_d setObject:@"1" forKey:@"soundEffect"]; \
    NSMutableArray *_a=[NSMutableArray new]; \
    [_a addObject:@"OVIMExample-simple"]; \
    [_d setObject:_a forKey:@"IMArray"]; \
    [x setObject:_d forKey:@VXLOADERPREFKEY];   \
    [_d release];

#define VXUSINGOFARRAYKEY   "OFArray"
#define VXUSINGIMARRAYKEY   "IMArray"

#endif
