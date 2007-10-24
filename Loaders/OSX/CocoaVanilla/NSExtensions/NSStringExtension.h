#include <Cocoa/Cocoa.h>

@interface NSString (SplitBySpaceWithQuote) 
+ (NSString*)stringByColor:(NSColor*)c;
- (NSString*)stringByQuoting;
- (NSString*)stringByChomping;
- (NSArray*)splitBySpaceWithQuote;
- (NSColor*)colorByString;
@end
