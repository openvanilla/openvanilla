//
//  LFHotkeyView.h
//  ShortCut
//
//  Created by Weizhong Yang on 12/26/07.
//  Copyright 2007 Lithoglyph Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface OVHotkeyField : NSView {
	NSTextField *u_displayTextView;
	NSButton *u_setButton;
	NSString *m_shortcut;
	@private
	NSDictionary *hotKey;
}
- (IBAction)set:(id)sender;
- (NSDictionary *)hotKey;
- (void)setHotKey:(NSDictionary *)newHotKey;
- (void)updateStringForHotKey;
- (void)absorbEvents;
@end
