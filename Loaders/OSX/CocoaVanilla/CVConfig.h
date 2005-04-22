// CVConfig.h: configuration managmenet

#ifndef __CVConfig_h
#define __CVConfig_h

#include <Cocoa/Cocoa.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>

typedef struct timespec CVTimeTag;

#ifdef __cplusplus
    extern "C" BOOL CVTimeTagEqual(const CVTimeTag *s1, const CVTimeTag *s2);
    inline int operator==(const CVTimeTag &s1, const CVTimeTag &s2) {
        return CVTimeTagEqual(&s1, &s2);
    }
#endif

// note that sync method may discard local changes if the config file is newer
@interface CVConfig : NSObject {
    NSString *filename;
    NSMutableDictionary *dict;
    NSMutableDictionary *backup;
    CVTimeTag stamp;
}
-(void)dealloc;
-(CVConfig*)initWithFile:(NSString*)f defaultData:(NSDictionary*)d;
-(CVTimeTag)timeStamp;
-(CVTimeTag)sync;
-(NSMutableDictionary*)dictionary;
-(BOOL)needSync;
@end

#endif
