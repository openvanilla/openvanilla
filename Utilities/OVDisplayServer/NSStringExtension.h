#include <Cocoa/Cocoa.h>

@interface NSString (SplitBySpaceWithQuote) 
-(NSString*)stringByQuoting;
-(NSString*)stringByChomping;
-(NSArray*)splitBySpaceWithQuote;
@end
