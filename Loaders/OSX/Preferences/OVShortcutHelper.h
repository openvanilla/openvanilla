//
//  OVShortcutHelper.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/6.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OVShortcutHelper : NSObject {

}

+ (NSString *)readableShortCut:(NSString *)shortcut;
+ (NSString *)shortcutFromDictionary: (NSDictionary *)dictionary;
+ (NSString *)stringForModifiers: (unsigned int)aModifierFlags;
@end
