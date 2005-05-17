// CVKeyCode.mm

#include <ctype.h>
#include "CVKeyCode.h"
#include "NSStringExtension.h"

CVKeyCode::CVKeyCode(char charcode, UInt32 modifiers) {
	m=modifiers;
	c=charcode;
	
	// translate keycode
	switch (c) {
		case 3:		// Mac's very confusing "enter" key
			c=ovkReturn;
			break;
	}
}

CVKeyCode::CVKeyCode(const char *charcode, const char *modifiers) {
	init (charcode, modifiers);
}

CVKeyCode::CVKeyCode(NSString *s) {
	NSArray *a=[s splitBySpaceWithQuote];
	if ([a count] < 2) {
		init([s UTF8String], "");
	}
	else {
		init([[a objectAtIndex:0] UTF8String], [[a objectAtIndex:1] UTF8String]);
	}
}

void CVKeyCode::init(const char *charcode, const char *modifiers) {
    c=0;
    if (strlen(charcode)==1) {
        c=toupper(*charcode);
    }
    else {
        #define KMAP(x,y)  if(!strcasecmp(charcode, x)) c=y
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
	m=0;
	
	for (size_t i=0; i<strlen(modifiers); i++) {
		switch(toupper(modifiers[i])) {
			case 'M': m |= cmdKey; break;
			case 'O': m |= optionKey; break;
			case 'C': m |= controlKey; break;
			case 'S': m |= shiftKey; break;
		}
	}
}

BOOL CVKeyCode::equalToKey(CVKeyCode *k, BOOL ignorecase) {
	if (ignorecase) {
		if (toupper(code()) != toupper(k->code())) return NO;
	}
	else {
		if (code() != k->code()) return NO;
	}

	if ((isShift() == k->isShift()) &&
	    (isCtrl() == k->isCtrl()) &&
		(isOpt() == k->isOpt()) &&
		(isCommand() == k->isCommand())) return YES;

	return NO;
}

UInt8 CVKeyCode::convertToMenuModifier() {
	UInt8 mm=0;
	
	if (isShift()) mm |= kMenuShiftModifier;
	if (isCtrl()) mm |= kMenuControlModifier;
	if (isOpt()) mm |= kMenuOptionModifier;
	if (!isCommand()) mm |= kMenuNoCommandModifier;
	return mm;
}

int CVKeyCode::isShift() {
    if (m & (shiftKey | rightShiftKey)) return 1;
    return 0;
}

int CVKeyCode::isCtrl() {
    if (m & (controlKey | rightControlKey)) return 1;
    return 0;
}

int CVKeyCode::isAlt() {
    if (m & (optionKey | rightOptionKey)) return 1;
    return 0;
}

int CVKeyCode::isCommand() {
    if (m & cmdKey) return 1;
    return 0;
}

int CVKeyCode::isCapslock() {
    if (m&(alphaLock|kEventKeyModifierNumLockMask)) return 1; 
    return 0;
}

NSArray *CVKeyCode::getKeyList() {
    NSMutableArray *ma=[[NSMutableArray new] autorelease];
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
    char *s="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`a";
    char buf[2];
    buf[1]=0;
    for (size_t i=0; i<strlen(s); i++) {
        buf[0]=s[i];
        [ma addObject:[NSString stringWithUTF8String:buf]];
    }

    return ma;
}

NSString *CVKeyCode::getKeyCodeString() {
    char buf[32];
    bzero(buf, 32);
    buf[0]=code();
    #define KMAP(x,y)  if(c==y) strcpy(buf, x)
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

NSString *CVKeyCode::getModifierString() {    
    char func[32];
    bzero(func, 32);
    if (isCommand()) strcat(func, "⌘");
    if (isOpt()) strcat(func, "⌥");
    if (isCtrl()) strcat(func, "^");
    if (isShift()) strcat(func, "⇧");
    return [NSString stringWithUTF8String:func];
}

NSString *CVKeyCode::getModifierIconString() {
    char func[32];
    bzero(func, 32);
    if (isCommand()) strcat(func, "m");
    if (isOpt()) strcat(func, "o");
    if (isCtrl()) strcat(func, "c");
    if (isShift()) strcat(func, "s");
    return [NSString stringWithUTF8String:func];
}