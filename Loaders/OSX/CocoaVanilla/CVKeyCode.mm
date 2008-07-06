// CVKeyCode.mm: CocoaVanilla implementation of OVKeyCode
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#include <ctype.h>
#include "CVKeyCode.h"
#include "NSStringExtension.h"

CVKeyCode::CVKeyCode(char charcode, UInt32 modifiers)
{
	m = modifiers;
	c = charcode;
	
	// translate keycode
	switch (c) {
		case 3:		// Mac's very confusing "enter" key
			c = ovkReturn;
			break;
	}
}

CVKeyCode::CVKeyCode(const char *charcode, const char *modifiers)
{
	init (charcode, modifiers);
}

CVKeyCode::CVKeyCode(NSString *s)
{
	NSArray *a = [s splitBySpaceWithQuote];
	if ([a count] < 2) {
		init([s UTF8String], "");
	}
	else {
		init([[a objectAtIndex:0] UTF8String], [[a objectAtIndex:1] UTF8String]);
	}
}

void CVKeyCode::init(const char *charcode, const char *modifiers) {
    c = 0;
    if (strlen(charcode) == 1) {
        c = toupper(*charcode);
    }
    else {
        #define KMAP(x,y)  if(!strcasecmp(charcode, x)) c = y
        KMAP("esc", ovkEsc);
        else KMAP("space", ovkSpace);
        else KMAP("delete", ovkDelete);
        else KMAP("backspace", ovkBackspace);
        else KMAP("up", ovkUp);
        else KMAP("down", ovkDown);
        else KMAP("left", ovkLeft);
        else KMAP("right", ovkRight);
        else KMAP("home", ovkHome);
        else KMAP("end", ovkEnd);
        else KMAP("pageup", ovkPageUp);
        else KMAP("pagedown", ovkPageDown);
        else KMAP("tab", ovkTab);
        #undef KMAP
    }
	m = 0;
	
	for (size_t i = 0; i < strlen(modifiers); i++) {
		switch (toupper(modifiers[i])) {
			case 'M': m |= cmdKey; break;
			case 'O': m |= optionKey; break;
			case 'C': m |= controlKey; break;
			case 'S': m |= shiftKey; break;
		}
	}
}

BOOL CVKeyCode::equalToKey(CVKeyCode* k, BOOL ignorecase)
{
	if (ignorecase) {
		if (toupper(code()) != toupper(k->code())) 
			return NO;
	}
	else {
		if (code() != k->code())
			return NO;
	}

	if ((isShift() == k->isShift()) &&
	    (isCtrl() == k->isCtrl()) &&
		(isOpt() == k->isOpt()) &&
		(isCommand() == k->isCommand()))
		return YES;

	return NO;
}

UInt8 CVKeyCode::convertToMenuModifier()
{
	UInt8 mm = 0;
	
	if (isShift()) mm |= kMenuShiftModifier;
	if (isCtrl()) mm |= kMenuControlModifier;
	if (isOpt()) mm |= kMenuOptionModifier;
	if (!isCommand()) mm |= kMenuNoCommandModifier;
	return mm;
}

bool CVKeyCode::isShift() 
{
    if (m & (shiftKey | rightShiftKey))
		return true;
    return false;
}

bool CVKeyCode::isCtrl()
{
    if (m & (controlKey | rightControlKey))
		return true;
    return false;
}

bool CVKeyCode::isAlt()
{
    if (m & (optionKey | rightOptionKey))
		return true;
    return false;
}

bool CVKeyCode::isCommand()
{
    if (m & cmdKey)
		return true;
    return false;
}

bool CVKeyCode::isCapslock()
{
    if (m & (alphaLock | kEventKeyModifierNumLockMask))
		return true; 
//  if (m & alphaLock) return 1; 
    return false;
}

NSArray *CVKeyCode::getKeyList()
{
    NSMutableArray *ma = [[NSMutableArray new] autorelease];
    #define KMAP(x)     [ma addObject:x]
    KMAP(@"esc");
    KMAP(@"space");
    KMAP(@"delete");
    KMAP(@"backspace");
    KMAP(@"up");
    KMAP(@"down");
    KMAP(@"left");
    KMAP(@"right");
    KMAP(@"home");
    KMAP(@"end");
    KMAP(@"pageup");
    KMAP(@"pagedown");
    KMAP(@"tab");
    #undef KMAP
    char *s = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;< = >?@[\\]^_`";
    char buf[2];
    buf[1] = 0;
    for (size_t i = 0; i<strlen(s); i++) {
        buf[0] = s[i];
        [ma addObject:[NSString stringWithUTF8String:buf]];
    }

    return ma;
}

NSString *CVKeyCode::getKeyCodeString()
{
    char buf[32];
    bzero(buf, 32);
    buf[0] = code();
    #define KMAP(x,y)  if(c == y) strcpy(buf, x)
    KMAP("esc", ovkEsc);
    else KMAP("space", ovkSpace);
    else KMAP("delete", ovkDelete);
    else KMAP("backspace", ovkBackspace);
    else KMAP("up", ovkUp);
    else KMAP("down", ovkDown);
    else KMAP("left", ovkLeft);
    else KMAP("right", ovkRight);
    else KMAP("home", ovkHome);
    else KMAP("end", ovkEnd);
    else KMAP("pageup", ovkPageUp);
    else KMAP("pagedown", ovkPageDown);
    else KMAP("tab", ovkTab);
    #undef KMAP
    return [NSString stringWithUTF8String:buf];
}

NSString *CVKeyCode::getKeyIconString()
{
	NSString *string = getKeyCodeString();
	if ([string isEqualToString:@"space"]) {
		return [NSString stringWithUTF8String:"Space"];
	}
	else if ([string isEqualToString:@"delete"]) {
		return [NSString stringWithUTF8String:"⌦"];
	}
	else if ([string isEqualToString:@"backspace"]) {
		return [NSString stringWithUTF8String:"⌫"];
	}
	else if ([string isEqualToString:@"up"]) {
		return [NSString stringWithUTF8String:"↑"];
	}
	else if ([string isEqualToString:@"down"]) {
		return [NSString stringWithUTF8String:"↓"];
	}
	else if ([string isEqualToString:@"left"]) {
		return [NSString stringWithUTF8String:"←"];
	}
	else if ([string isEqualToString:@"right"]) {
		return [NSString stringWithUTF8String:"→"];
	}
	else if ([string isEqualToString:@"home"]) {
		return [NSString stringWithUTF8String:"↖"];
	}	
	else if ([string isEqualToString:@"end"]) {
		return [NSString stringWithUTF8String:"↘"];
	}
	else if ([string isEqualToString:@"pageup"]) {
		return [NSString stringWithUTF8String:"⇞"];
	}
	else if ([string isEqualToString:@"pagedown"]) {
		return [NSString stringWithUTF8String:"⇟"];
	}
	else if ([string isEqualToString:@"tab"]) {
		return [NSString stringWithUTF8String:"⇥"];
	}
	
	
	return string;
}

NSString *CVKeyCode::getModifierIconString()
{ 
    char func[32];
    bzero(func, 32);
    if (isCommand()) 
		strcat(func, "⌘");
    if (isOpt()) 
		strcat(func, "⌥");
    if (isCtrl()) 
		strcat(func, "^");
    if (isShift()) 
		strcat(func, "⇧");
    return [NSString stringWithUTF8String:func];
}

NSString *CVKeyCode::getModifierString()
{
    char func[32];
    bzero(func, 32);
    if (isCommand()) 
		strcat(func, "m");
    if (isOpt()) 
		strcat(func, "o");
    if (isCtrl()) 
		strcat(func, "c");
    if (isShift()) 
		strcat(func, "s");
    return [NSString stringWithUTF8String:func];
}