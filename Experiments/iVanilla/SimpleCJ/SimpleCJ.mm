// This code is experimental and does not guarantee it will run.
// No build instruction is given. It might harm your system.
//
// Copyright (c) 2007-2008 Yao-Chung Chang and Lukhnos D. Liu
// Released under the same terms under which OpenVanilla is released 
// (The New BSD License)
//
// See LICENSE.TXT under OpenVanilla's source repository trunk/License/
// for the full text of the New BSD License.
//

#import <objc/objc-class.h>
#import <UIKit/UIKit.h>
#import <UIKit/UIDefaultKeyboardInput.h>
#import <UIKit/UIKBInputManager.h>

#include "MinimalLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <string>

static BOOL g_inputMethodEnabled;
static NSMutableString *g_autocorrectText;

extern "C" void _Unwind_SjLj_Resume()
{
    // this may cause SERIOUS PROBLEM if we ever use C++/ObjC exception handling, but we have to 
    // do this for the time being to make the linker happy
    
    // SjLj seems to mean ObjC++ exception unwinding
}

extern "C" void SimpleCJModuleInit();

void MethodSwizzle(Class aClass, SEL orig_sel, SEL alt_sel);

@interface UIKeyboardImpl (SimpleCJ)
- (void)replacedAddInputString:(NSString*)input;
@end

@implementation UIKeyboardImpl (SimpleCJ)
- (void)replacedNotifyShiftChanged
{
    [self replacedNotifyShiftChanged];
    if ([g_autocorrectText length]) [self updateAutocorrectPrompt:g_autocorrectText];
}
- (void)replacedNotifySublayoutChanged
{
    [self replacedNotifySublayoutChanged];
    if ([g_autocorrectText length]) [self updateAutocorrectPrompt:g_autocorrectText];
}
- (void)setAutocorrectText:(NSString*)text
{
    [g_autocorrectText setString:text];
    [self updateAutocorrectPrompt:g_autocorrectText];
}
- (void)replacedDeleteFromInput
{
    NSLog(@"delete from input");
    if (!g_inputMethodEnabled) {
        [self replacedDeleteFromInput];
        return;
    }
    
    // if not handled by OV, we pass it back to do the real delete
    if (!MLKey(ovkDelete))
        [self replacedDeleteFromInput];
    else {
    	if (!MLCandidatesString().length()) {
			NSLog(@"setting auto correction: %@", [NSString stringWithUTF8String:MLComposingBuffer().c_str()]);
            [self setAutocorrectText:[NSString stringWithUTF8String:MLComposingBuffer().c_str()]];

        }
        else {
            [g_autocorrectText setString:@""];
            [self fadeAutocorrectPrompt];
        }
    }
}
- (void)replacedAddInputString:(NSString*)input
{    
    NSLog(@"Received key string %@", [input description]);

    UInt16 ccode = [input characterAtIndex:0];
    if (ccode == '&' && g_inputMethodEnabled) {
        NSLog(@"composing buffer reset");
        [self setAutocorrectText:@"IME reset"];
        MLCancel();
        MLStart();        
        return;
    }
     
    if (ccode == '@') {
        if (g_inputMethodEnabled) {
            NSLog(@"IME now disabled");
            [self setAutocorrectText:@"IME disabled"];
            g_inputMethodEnabled = NO;
            [m_delegate insertText:input];
            return;
        }
        else {
            NSLog(@"IME now enabled");
            [self setAutocorrectText:[NSString stringWithUTF8String:"OpenVanilla啟動"]];
            g_inputMethodEnabled = YES;
            MLStart();            
            return;
        }
    }
    
    if (!g_inputMethodEnabled) {
        [self replacedAddInputString:input];
        return;
    }

	if (MLKey(ccode)) {
		if (!MLIsCommited()) {
            cerr << "[CURRENTLY COMPOSING] " << MLComposingBuffer() << "\n";
            
        	if (!MLCandidatesString().length()) {
                NSLog(@"setting auto correction");
                [self setAutocorrectText:[NSString stringWithUTF8String:MLComposingBuffer().c_str()]];
            }
		}
		else {
			cerr << "[OUTPUT STRING] " << MLCommitedString() << "\n";
			[m_delegate insertText:[NSString stringWithUTF8String:MLCommitedString().c_str()]];
			MLCancel();
			MLStart();
            [g_autocorrectText setString:@""];
            [self fadeAutocorrectPrompt];
		}		
	}	
	else {
        [m_delegate insertText:input];
    }
    
	if (MLCandidatesString().length() > 0) {
        cerr << "[CANDIDATES] " << MLCandidatesString() << "\n";
        [self setAutocorrectText:[NSString stringWithUTF8String:MLCandidatesString().c_str()]];
	}    	
}	
@end

// SimpleCJ module initializer
extern "C" void SimpleCJModuleInit()
{
	NSLog (@"SimpleCJ initialized");
    g_inputMethodEnabled = NO;
    g_autocorrectText = [[NSMutableString alloc] init];    
	MethodSwizzle ([UIKeyboardImpl class], @selector(addInputString:), @selector(replacedAddInputString:));
	MethodSwizzle ([UIKeyboardImpl class], @selector(deleteFromInput), @selector(replacedDeleteFromInput));
    MethodSwizzle ([UIKeyboardImpl class], @selector(notifySublayoutChanged), @selector(replacedNotifySublayoutChanged));
    MethodSwizzle ([UIKeyboardImpl class], @selector(notifyShiftChanged), @selector(replacedNotifyShiftChanged));
		
    NSLog(@"Initializing OpenVanilla MinimalLoader (with the preloaded IME module)");
	MLInitialize();
}

void MethodSwizzle(Class aClass, SEL orig_sel, SEL alt_sel)
{
    Method orig_method = nil;
    Method alt_method = nil;

	// First, look for the methods
	orig_method = class_getInstanceMethod(aClass, orig_sel);
	alt_method = class_getInstanceMethod(aClass, alt_sel);

	// If both are found, swizzle them
	if ((orig_method != nil) && (alt_method != nil))
	{
		char *temp1;
		IMP temp2;

		temp1 = orig_method->method_types;
		orig_method->method_types = alt_method->method_types;
		alt_method->method_types = temp1;

		temp2 = orig_method->method_imp;
		orig_method->method_imp = alt_method->method_imp;
		alt_method->method_imp = temp2;
	}
	else {
		NSLog (@"swizzle failed");
	}
}
