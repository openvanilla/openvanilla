#include <Cocoa/Cocoa.h>

@interface NSString (SplitBySpaceWithQuote) 
-(NSString*)stringByChomping;
-(NSArray*)splitBySpaceWithQuote;
@end
