//
//  LFHotkeyView.h
//  ShortCut
//
//  Created by Weizhong Yang on 12/26/07.
//  Copyright 2007 Lithoglyph Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OVHotkeyField : NSView {
	IBOutlet id u_outlineView;
	NSTextField *u_displayTextView;
	NSButton *u_setButton;
	id _moduleController;
	@private
	NSString *m_shortcut;	
	NSDictionary *hotKey;
}
- (IBAction)set:(id)sender;
- (NSDictionary *)hotKey;
- (void)setHotKey:(NSDictionary *)newHotKey;
- (void)updateStringForHotKey;
- (void)absorbEvents;
- (void)setModuleController: (id)controller;
- (id)moduleController;
@end
