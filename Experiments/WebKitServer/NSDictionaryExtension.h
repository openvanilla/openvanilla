// NSDictionaryExtension.h
// Extension to NSDictionary class
// By lukhnos
// This piece of code is public domain

#ifndef __NSDictionaryExtension_h
#define __NSDictionaryExtension_h

#import <Cocoa/Cocoa.h>

/*! 

 @category NSDictionary(ValueForKeyDefault)
 Extends NSDictionary's functionalities
  
*/
@interface NSDictionary(ValueForKeyDefault) 
/*!

 @method valueForKey:default:
 Look for the corresponding value to the key, if not found, it returns the value specified in the default: parameter
*/
-(id)valueForKey:(NSString*)key default:(id)obj;
@end

/*! 

 @category NSMutableDictionary(ValueForKeyDefault)
 Extends NSMutableDictionary's functionalities
  
*/
@interface NSMutableDictionary (ValueForKeyDefault) 
/*!

 @method valueForKey:default:
 Look for the corresponding value to the key, if not found, it first inserts a new key-value pair in the dictionary, then returns the value specified in the default: parameter. Note the behavior is different from that of NSDictionary.
*/
-(id)valueForKey:(NSString*)key default:(id)obj;
@end

#endif
