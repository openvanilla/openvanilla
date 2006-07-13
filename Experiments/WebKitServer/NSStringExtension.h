// NSStringExtension.h
// Extension to NSString class
// By lukhnos
// This piece of code is public domain

#include <Cocoa/Cocoa.h>

/*!
 
 @category NSString (SplitBySpaceWithQuote)
 
 @abstract Extends NSString's functionalities
 
 This category provides some extensions to NSString.
 
*/
@interface NSString (SplitBySpaceWithQuote) 
/*!
 creates an NSString object (autoreleased) by converting the NSColor parameter
*/
+ (NSString*)stringByColor:(NSColor*)c;
/*!
 creates an autoreleased NSString object by putting double-quote marks around the receiver
*/
- (NSString*)stringByQuoting;
/*!
 creates an autoreleased NSString by chomping the ending 0xa character of the receiver 
*/
- (NSString*)stringByChomping;
/*!
 returns an array of strings resulting from splitting the receiver. This method takes quoted substrings into consideration.
*/
- (NSArray*)splitBySpaceWithQuote;
/*!
 returns an autoreleased NSColor by converting the receiver, e.g. for the string "1.0 1.0 1.0" you will get the NSColor equivalent of [NSColor white]
*/
- (NSColor*)colorByString;
@end
