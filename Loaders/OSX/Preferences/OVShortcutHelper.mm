// OVShortcutHelper.mm : The helper for shortcut setting.
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "OVShortcutHelper.h"
#import "CVKeyCode.h"

@implementation OVShortcutHelper

unichar unicharForKeyCode( unsigned short aKeyCode )
{
	
	static UInt32			theState = 0;
	const void				* theKeyboardLayoutData;
	KeyboardLayoutRef		theCurrentKeyBoardLayout;
	UInt32					theChar = kNullCharCode;
	
	if( KLGetCurrentKeyboardLayout( &theCurrentKeyBoardLayout ) == noErr && KLGetKeyboardLayoutProperty( theCurrentKeyBoardLayout, kKLKCHRData, &theKeyboardLayoutData) == noErr ) {
		theChar = KeyTranslate (theKeyboardLayoutData, aKeyCode, &theState);
		
		switch(theChar) {
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
	NSString *keyString;

	#define KMAP(x, y) if (aCharacter == x) keyString = y;

	KMAP(ovkSpace, @"space")
	else KMAP(NSDeleteCharFunctionKey, @"delete")
	else KMAP(ovkBackspace, @"backspace")
	else KMAP(NSUpArrowFunctionKey, @"up")
	else KMAP(NSDownArrowFunctionKey, @"down")
	else KMAP(NSLeftArrowFunctionKey, @"left")
	else KMAP(NSRightArrowFunctionKey, @"right")
	else KMAP(NSHomeFunctionKey, @"home")
	else KMAP(NSEndFunctionKey, @"end")
	else KMAP(NSPageUpFunctionKey, @"pageup")
	else KMAP(NSPageDownFunctionKey, @"pagedown")
	else KMAP(ovkTab, @"tab")	
	else {
		keyString = [NSString stringWithCharacters:&aCharacter length:1];
	}
	
	if (!keyString || ![keyString length]) {
		return @"";
	}

	unsigned int aModifierFlags = [modifiers unsignedIntValue];
	NSMutableString *modifierString = [NSMutableString string];
	
	#define APPEND(x) [modifierString appendString:x];
	if (aModifierFlags & NSCommandKeyMask) APPEND(@"m")
	if (aModifierFlags & NSAlternateKeyMask) APPEND(@"o")
	if (aModifierFlags & NSControlKeyMask) APPEND(@"c")
	if (aModifierFlags & NSShiftKeyMask) APPEND(@"s")
	#undef APPEND
	
	NSString *s = [NSString stringWithFormat:@"%@ %@", keyString, modifierString];
	return s;
}


+ (NSString *)stringForModifiers: (unsigned int)aModifierFlags
{
	NSMutableString	*s = [NSMutableString string];
	unichar ch;
	
	#define APPEND(x)  {ch = x; [s appendString:[NSString stringWithCharacters:&ch length:1]];}
    if (aModifierFlags & NSCommandKeyMask) APPEND(kCommandUnicode)
    if (aModifierFlags & NSAlternateKeyMask) APPEND(kOptionUnicode)
    if (aModifierFlags & NSControlKeyMask) APPEND(kControlUnicode)
    if (aModifierFlags & NSShiftKeyMask) APPEND(kShiftUnicode)		
	#undef APPEND
	return s;
}

@end
