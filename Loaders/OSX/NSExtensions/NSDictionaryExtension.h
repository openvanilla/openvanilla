// NSDictionaryExtension.h

#ifndef __NSDictionaryExtension_h
#define __NSDictionaryExtension_h

#import <Cocoa/Cocoa.h>

@interface NSMutableDictionary (ValueForKeyDefault) 
-(id)valueForKey:(NSString*)key default:(id)obj;
@end

#endif
