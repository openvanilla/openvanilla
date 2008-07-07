#import <Cocoa/Cocoa.h>

@interface OVHotkeyField : NSView {
	IBOutlet id u_outlineView;
	NSTextField *u_displayTextView;
	NSButton *u_setButton;
	NSButton *u_clearButton;	
	id m_moduleController;
	@private
	NSString *m_shortcut;	
	NSDictionary *hotKey;
}
- (IBAction)set:(id)sender;
- (NSDictionary *)hotKey;
- (void)setHotKey:(NSDictionary *)newHotKey;
- (void)updateStringForHotKey;
- (void)absorbEvents;
- (void)setModuleController:(id)controller;
- (id)moduleController;
@end
