//
//  OVShortcutHelper.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/6.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVShortcutHelper.h"
#import "CVKeyCode.h"

@implementation OVShortcutHelper

+ (NSString *)readableShortCut:(NSString *)shortcut
{
	if (!shortcut)
		return nil;
	
	CVKeyCode cvkc(shortcut);
	NSString *code = cvkc.getKeyCodeString();
	NSString *modifier = cvkc.getModifierIconString();
	return [NSString stringWithFormat:@"%@ %@", modifier, code];
}

@end
