// CVConfig.h: configuration managmenet

#ifndef __CVConfig_h
#define __CVConfig_h

#include <Cocoa/Cocoa.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

typedef struct timespec CVTimeStamp;

#ifdef __cplusplus
    extern "C" BOOL CVTimeStampEqual(const CVTimeStamp *s1, const CVTimeStamp *s2);
    inline int operator==(const CVTimeStamp &s1, const CVTimeStamp &s2) {
        return CVTimeStampEqual(&s1, &s2);
    }
#endif

// note that sync method may discard local changes if the config file is newer
@interface CVConfig : NSObject {
    NSString *filename;
    NSMutableDictionary *dict;
    NSMutableDictionary *backup;
    CVTimeStamp stamp;
}
-(void)dealloc;
-(CVConfig*)initWithFile:(NSString*)f defaultData:(NSDictionary*)d;
-(CVTimeStamp)timeStamp;
-(CVTimeStamp)sync;
-(NSMutableDictionary*)dictionary;
-(BOOL)needSync;
@end

#endif
