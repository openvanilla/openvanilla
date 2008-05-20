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
#import <UIKit/UIKBInputManager.h>
#import <UIKit/UIDefaultKeyboardInput.h>
#import <UIKit/UIAutocorrectInlinePrompt.h>
#import <UIKit/UIKeyboardSublayout.h>
#import <UIKit/UIKeyboardLayout.h>
#import "POBox.h"

#include "MinimalLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <string>

BOOL g_inputMethodEnabled = YES;

extern "C" void _Unwind_SjLj_Resume()
{
    // this may cause SERIOUS PROBLEM if we ever use C++/ObjC exception handling, but we have to 
    // do this for the time being to make the linker happy
    
    // SjLj seems to mean ObjC++ exception unwinding
}

extern "C" void SimpleBPMFModuleInit();

void MethodSwizzle(Class aClass, SEL orig_sel, SEL alt_sel);


@interface POBox (SimpleBPMF)
- (id)myGetOneCandidate:(id) word;
- (id)myGetCandidates:(id) word fixPat:(id) pat;
- (int)myAddCandidate:(char *) w1 yomi:(char *) w2 inConnection:(int)n1 outConnection:(int)n2 nCands:(int)n3;
- (void)myAddConnectedCandidate:(char *)w1 yomi:(char *)w2 inConnection:(int)n1 outConnection:(int)n2 withLevel:(int)n3;
- (void)myGenerateCand0:(int)n1 matchedLen:(int)n2 level:(int)n3 matchedStr:(char *)w1 matchedYomi:(char *)w2;
- (void)myRegist:(id) w1 withYomi:(id) w2 inConnection:(int) n1 outConnection:(int *) n2  outConnections:(int) n3;

- (void) testMode;

@end

@implementation POBox (SimpleBPMF)
- (void) testMode
{

}

- (void)myRegist:(id) w1 withYomi:(id) w2 inConnection:(int) n1 outConnection:(int *) n2  outConnections:(int) n3
{
	//NSLog (@"regist %@ %@ %d %d %d", w1, w2, n1, n2, n3);
	//return [self myRegist: w1 withYomi: w2 inConnection: n1 outConnection: n2 outConnections:n3];
}

- (int)myAddCandidate:(char *) w1 yomi:(char *) w2 inConnection:(int)n1 outConnection:(int)n2 nCands:(int)n3
{
	//NSLog (@"addCandidate %@ %@ %d %d %d", [NSString stringWithUTF8String:w1], [NSString stringWithUTF8String:w2], n1, n2, n3);
	//return  [self myAddCandidate: w1 yomi: w2 inConnection: n1 outConnection: n2 nCands:n3];
}

/* This method is called to generate the candidate */

- (void)myAddConnectedCandidate:(char *)w1 yomi:(char *)w2 inConnection:(int)n1 outConnection:(int)n2 withLevel:(int)n3
{
	//NSLog (@"addConnectedCandidate %@ %@ %d %d %d", [NSString stringWithUTF8String:w1], [NSString stringWithUTF8String:w2], n1, n2, n3);
	//[self myAddConnectedCandidate: w1 yomi: w2 inConnection: n1 outConnection: n2 withLevel:n3]; 
}
- (void)myGenerateCand0:(int)n1 matchedLen:(int)n2 level:(int)n3 matchedStr:(char *)w1 matchedYomi:(char *)w2
{
	//NSLog (@"addGenerateCandidate0 %@ %@ %d %d %d", [NSString stringWithUTF8String:w1], [NSString stringWithUTF8String:w2], n1, n2, n3);
	//[self myGenerateCand0: n1 matchedLen: n2 level: n3 matchedStr: w1 matchedYomi: w2];
}

- (id)myGetOneCandidate:(id) word
{
	//id obj = [self myGetOneCandidate: word];

	//NSLog (@"one candicate %@", [word description]);
	//NSLog (@"one candicate %@", [obj description]);

	//return obj;
	return nil;
}


/* Manually add a test candidate into the last of list */

- (id)myGetCandidates:(id) word fixPat:(id) pat
{
	id w;

	NSLog(@"Received key string %@", [word description]);
	//[self myGetCandidates:word fixPat: pat];


	_candidates = [[[NSMutableArray alloc] init] autorelease];
	[_candidates removeAllObjects];
	
	if ([word length] > 0) {
		id w = [[WordInfo alloc] initWithWord: word withYomi: word inConnection:0 outConnection: 0];
		[_candidates addObject: w];
	}

	int len = [word length];

	UInt16 ccode = [word characterAtIndex: len -1];
	printf ("ccode %c\n", ccode);
	if (ccode == '-' || len == 0) {
		NSLog(@"composing buffer reset");
		MLCancel();
		MLStart();
		return _candidates;
	} 

	for (int i = 0; i < len; i++) {
		UInt16 ccode = [word characterAtIndex: i];

		if (MLKey(ccode)) {
			if (!MLIsCommited()) {
				cerr << "[CURRENTLY COMPOSING] " << MLComposingBuffer() << "\n";
			}
			else {
				cerr << "[OUTPUT STRING] " << MLCommitedString() << "\n";
				MLCancel();
				MLStart();
			}

		}	

		if (MLCandidatesString().length() > 0) {
			cerr << "[CANDIDATES] " << MLCandidatesString() << "\n";
			//char *str = MLCandidatesString().c_str();

			id w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:MLCandidatesString().c_str()] withYomi: @"fuck" inConnection:0 outConnection: 0];
			[_candidates addObject: w];
		}   

	}
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"中"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"文"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"測"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"試"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"香"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"草"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"輸"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"入"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"法"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"testtesttest"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"leopard"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"apple"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"testtesttesttest"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	w = [[WordInfo alloc] initWithWord: [NSString stringWithUTF8String:"法"] withYomi: @"fuck" inConnection:0 outConnection: 0];
	[_candidates addObject: w];
	return _candidates;
}	
@end

@interface UIKBInputManager (SimpleBPMF)
- (id)myGenerateSuggestions;
- (id)myGenerateSuggestions:(BOOL) flag;
- (id)myInputString;
- (id)myAutocorrection;
- (id)myShadowTyping;
@end

@implementation UIKBInputManager (SimpleBPMF)
- (id)myShadowTyping
{
	id obj = [self myShadowTyping];
	//NSLog (@"##%@", [obj description]);
	return obj;
}
- (id)myGenerateSuggestions
{
	id obj = [self myGenerateSuggestions];
	//NSLog (@"->%@", [obj description]);
	return obj;
}
- (id)myGenerateSuggestions:(BOOL) flag
{
	id obj = [self myGenerateSuggestions: flag];
	//NSLog (@"->%@", [obj description]);
	return obj;
}
- (id)myInputString
{
	id obj = [self myInputString];
	//NSLog (@"#%@", [obj description]);
	return obj;

}
- (id)myAutocorrection
{
	id obj = [self myAutocorrection];
	//NSLog (@"-->%@", [obj description]);
	return obj;
}
@end


@interface UIKeyboardImpl (SimpleBPMF)
- (void)replacedAddInputString:(NSString*)input;
- (void)replacedDeleteFromInput;
- (id)replacedInitWithFrame:(struct CGRect) rect;
@end


@implementation UIKeyboardImpl (SimpleBPMF)
- (id)replacedInitWithFrame:(struct CGRect) rect 
{
	//FontLayer *background = [[[FontLayer alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 320.0f, 216.0f)] autorelease];
	//[background setImage: [UIImage imageAtPath:@"/tmp/test.png"]];

	//[[UIImage imageAtPath:@"/tmp/test.png"] draw1PartImageInRect:CGRectMake(0.0f, 100.0f, 320.0f, 480.0f)];
	//self = [self replacedInitWithFrame: rect];
	//[m_languageIndicator setBackgroundImage: [UIImage imageAtPath:@"/tmp/test.png"]];
	//[self addSubview: background];
	//[background addTarget: self action:@selector(mouseDown:) forEvents:1];
	self = [self replacedInitWithFrame: rect];
	//[self addSubview: background];
	//[self becomeFirstResponder];

	return self;
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
    	
    // it seems that delete key  will fade the prompt byitself 		
    //[self updateAutocorrectPrompt:[NSString stringWithUTF8String:MLComposingBuffer().c_str()]]; 
}
- (void)replacedAddInputString:(NSString*)input
{    
    //[self clearLayouts];
    //[self setPreferredKeyboardType:2];
    //[self showPreferredLayout];
    //NSLog (@"keyboard %@", [[self keyboardsPreference] description]);
    //NSLog (@"delegate %@", [[self delegate] description]);
    //NSLog (@"LayoutKeyForPreferenceKeyboard  %@", [[[self layoutKeyForPreferredKeyboardType] class] description]);
   // NSLog (@"preferredLayoutClass %@", [[self preferredLayoutClass] description]);

    NSLog(@"Received key string %@", [input description]);
    NSLog(@"visibility = %d, visible = %d, suggestions = %@",
    [self suggestionBarVisibility],
    [self suggestionBarVisible],
    [[self suggestions] description]);
    
    NSLog(@"text suggestion delegate = %@", [[m_delegate textSuggestionDelegate] description]);
    NSLog(@"text traits = %@", [m_defaultTextTraits description]);
    NSLog(@"text traits delegate = %@", [[[m_defaultTextTraits class] defaultTextSuggestionDelegate] description]);

    UInt16 ccode = [input characterAtIndex:0];
    if (ccode == '&' && g_inputMethodEnabled) {
        NSLog(@"composing buffer reset");
        MLCancel();
        MLStart();

        [m_autocorrectPrompt dismiss];
        
        return;
    }
     
    if (ccode == '@') {
        if (g_inputMethodEnabled) {
            NSLog(@"IME now disabled");
            g_inputMethodEnabled = NO;
            [m_delegate insertText:input];
            return;
        }
        else {
            NSLog(@"IME now enabled");
            g_inputMethodEnabled = YES;
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
//            [self setSuggestions:[NSArray arrayWithObjects:@"blah", [NSString stringWithFormat:@"%d %d", MLComposingBuffer().length(), MLCandidatesString().length()], nil]];
/*            [self showSuggestionBar];
            [self updateSuggestionsDisplay];
            [self updateSuggestionsForCurrentInput];
            [self updateSuggestionBarVisibility:YES]; */
            // NSLog(@"text suggest delegate = %@", [m_textSuggestionDelegate description]);
            
        	if (!MLCandidatesString().length()) {
                NSLog(@"setting auto correction");
                [self updateAutocorrectPrompt:[NSString stringWithUTF8String:MLComposingBuffer().c_str()]];
            }
		}
		else {
			cerr << "[OUTPUT STRING] " << MLCommitedString() << "\n";
			[m_delegate insertText:[NSString stringWithUTF8String:MLCommitedString().c_str()]];
			[self fadeAutocorrectPrompt];
			MLCancel();
			MLStart();
		}
		
	}	
	else {
        [m_delegate insertText:input];
    }
    
	if (MLCandidatesString().length() > 0) {
        cerr << "[CANDIDATES] " << MLCandidatesString() << "\n";
        [self updateAutocorrectPrompt:[NSString stringWithUTF8String:MLCandidatesString().c_str()]];
	}    
	
	
}	
@end

// SimpleBPMF module initializer
extern "C" void SimpleBPMFModuleInit()
{
	NSLog (@"SimpleBPMF initialized");
    g_inputMethodEnabled = NO;

	/* POBox stuff */

    	MethodSwizzle ([POBox class], @selector(myRegist:withYomi:inConnection:outConnection:outConnections:), @selector(regist:withYomi:inConnection:outConnection:outConnections:));
    	MethodSwizzle ([POBox class], @selector(myAddCandidate:yomi:inConnection:outConnection:nCands:), @selector(addCandidate:yomi:inConnection:outConnection:nCands:));
    	MethodSwizzle ([POBox class], @selector(myAddConnectedCandidate:yomi:inConnection:outConnection:withLevel:), @selector(addConnectedCandidate:yomi:inConnection:outConnection:withLevel:));
    	MethodSwizzle ([POBox class], @selector(myGenerateCand0:matchedLen:level:matchedStr:matchedYomi:), @selector(generateCand0:matchedLen:level:matchedStr:matchedYomi:));
	MethodSwizzle ([POBox class], @selector(getCandidates:fixPat:), @selector(myGetCandidates:fixPat:));
	MethodSwizzle ([POBox class], @selector(getOneCandidate:), @selector(myGetOneCandidate:));

	/* end of POBox stuff */

	MethodSwizzle ([UIKBInputManager class], @selector(shadowTyping), @selector(myShadowTyping));
	MethodSwizzle ([UIKBInputManager class], @selector(autocorrection), @selector(myAutocorrection));
	//MethodSwizzle ([UIKBInputManager class], @selector(inputString), @selector(myInputString));
	MethodSwizzle ([UIKBInputManager class], @selector(generateSuggestions), @selector(myGenerateSuggestions));
	MethodSwizzle ([UIKBInputManager class], @selector(generateSuggestions), @selector(myGenerateSuggestions:));
	MethodSwizzle ([UIKeyboardImpl class], @selector(initWithFrame:), @selector(replacedInitWithFrame:));
	//MethodSwizzle ([UIKeyboardImpl class], @selector(addInputString:), @selector(replacedAddInputString:));
	//MethodSwizzle ([UIKeyboardImpl class], @selector(deleteFromInput), @selector(replacedDeleteFromInput));
	
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
