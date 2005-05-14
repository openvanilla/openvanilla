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
	c=toupper(*charcode);
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
