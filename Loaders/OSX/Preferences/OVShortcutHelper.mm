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

unichar unicharForKeyCode( unsigned short aKeyCode )
{
	
	static UInt32			theState = 0;
	const void				* theKeyboardLayoutData;
	KeyboardLayoutRef		theCurrentKeyBoardLayout;
	UInt32					theChar = kNullCharCode;
	
	if( KLGetCurrentKeyboardLayout( &theCurrentKeyBoardLayout ) == noErr && KLGetKeyboardLayoutProperty( theCurrentKeyBoardLayout, kKLKCHRData, &theKeyboardLayoutData) == noErr )
	{
		theChar = KeyTranslate ( theKeyboardLayoutData, aKeyCode, &theState );
		
		switch( theChar )
		{
			case kHomeCharCode: theChar = NSHomeFunctionKey; break;
				//			case kEnterCharCode: theChar = ; break;
			case kEndCharCode: theChar = NSEndFunctionKey; break;
			case kHelpCharCode: theChar = NSHelpFunctionKey; break;
				//			case kBellCharCode: theChar = ; break;
				//			case kBackspaceCharCode: theChar = ; break;
				//			case kTabCharCode: theChar = ; break;
				//			case kLineFeedCharCode: theChar = ; break;
			case kPageUpCharCode: theChar = NSPageUpFunctionKey; break;
			case kPageDownCharCode: theChar = NSPageDownFunctionKey; break;
				//			case kReturnCharCode: theChar = ; break;
				//case kFunctionKeyCharCode: theChar = unicodeForFunctionKey( aKeyCode ); break;
				//			case kCommandCharCode: theChar = ; break;
				//			case kCheckCharCode: theChar = ; break;
				//			case kDiamondCharCode : theChar = ; break;
				//			case kAppleLogoCharCode: theChar = ; break;
				//			case kEscapeCharCode: theChar = ; break;
			case kClearCharCode:
				theChar = (aKeyCode==0x47) ? NSInsertFunctionKey : theChar;
				break;
			case kLeftArrowCharCode: theChar = NSLeftArrowFunctionKey; break;
			case kRightArrowCharCode: theChar = NSRightArrowFunctionKey; break;
			case kUpArrowCharCode: theChar = NSUpArrowFunctionKey; break;
			case kDownArrowCharCode: theChar = NSDownArrowFunctionKey; break;
				//			case kSpaceCharCode: theChar = ; break;
			case kDeleteCharCode: theChar = NSDeleteCharFunctionKey; break;
				//			case kBulletCharCode: theChar = ; break;
				//			case kNonBreakingSpaceCharCode: theChar = ; break;
		}
	}
	
	return theChar;
}

+ (NSString *)readableShortCut:(NSString *)shortcut
{
	if (!shortcut)
		return @"";
	
	CVKeyCode cvkc(shortcut);
	NSString *code = cvkc.getKeyIconString();
	NSString *modifier = cvkc.getModifierIconString();
	return [NSString stringWithFormat:@"%@%@", modifier, code];
}

+ (NSString *)shortcutFromDictionary: (NSDictionary *)dictionary
{
	NSNumber *keyCode = [dictionary objectForKey:@"keyCode"];
	NSNumber *modifiers = [dictionary objectForKey:@"modifiers"];
	
	if (!keyCode) {
		return @"";
	}
	if (!modifiers) {
		return @"";
	}
	unsigned short aCode = [keyCode shortValue];
	unichar aCharacter = unicharForKeyCode(aCode);
	NSString *keyString = [NSString stringWithCharacters:&aCharacter length:1];

	unsigned int aModifierFlags = [modifiers unsignedIntValue];
	NSMutableString *modifierString = [NSMutableString string];
	
	if (aModifierFlags & NSCommandKeyMask) {
		[modifierString appendString:@"m"];
	}	
	if (aModifierFlags & NSAlternateKeyMask) {
		[modifierString appendString:@"o"];
	}	
	if (aModifierFlags & NSControlKeyMask) {
		[modifierString appendString:@"c"];
	}	
	if (aModifierFlags & NSShiftKeyMask) {
		[modifierString appendString:@"s"];		
	}
	
	NSString *s = [NSString stringWithFormat:@"%@ %@", keyString, modifierString];
	NSLog(s);
	
	return s;
}

@end
