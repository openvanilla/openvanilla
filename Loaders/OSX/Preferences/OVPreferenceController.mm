//
//  OVPreferenceController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/4.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVPreferenceController.h"


@implementation OVPreferenceController

- (void)setUpModules
{
	NSArray *moduleLists = [_loader moduleList];
	
	const char *locale = [_loader service]->locale();	
	NSEnumerator *e = [moduleLists objectEnumerator];
	CVModuleWrapper *w;
	
	while (w = [e nextObject]) {
        OVModule *ovm = [w module];
        NSString *name = [NSString stringWithUTF8String:ovm->localizedName(locale)];
		NSDictionary *dictionary = [_config valueForKey:[w identifier]];
		if (!dictionary) {
			dictionary = [NSDictionary dictionary];
		}
		//      NSString *shortcut=[menucfg valueForKey:mid default:@""];	
		
		if ([[w moduleType] isEqualToString:@"OVInputMethod"]) {
			if ([[w identifier] hasPrefix:@"OVIMGeneric-"]) {
				OVIMGenericController *moduleCotroller = [[OVIMGenericController alloc] initWithIdentifier:[w identifier] localizedName:name dictionary:dictionary delegate:self];
				[m_moduleListController addInputMethod:moduleCotroller];				
			}
			else if ([dictionary count]) {
				OVTableModuleController *moduleCotroller = [[OVTableModuleController alloc] initWithIdentifier:[w identifier] localizedName:name dictionary:dictionary delegate:self];
				[m_moduleListController addInputMethod:moduleCotroller];
			}
			else {
				OVModuleController *moduleCotroller = [[OVModuleController alloc] initWithIdentifier:[w identifier] localizedName:name dictionary:dictionary delegate:self];
				[m_moduleListController addInputMethod:moduleCotroller];
			}
		}
		else if ([[w moduleType] isEqualToString:@"OVOutputFilter"]) {
			OVModuleController *moduleCotroller = [[OVModuleController alloc] initWithIdentifier:[w identifier] localizedName:name dictionary:nil delegate:self];
			[m_moduleListController addOutputFilter:moduleCotroller];
		}
		
	}
	[m_moduleListController reload];
}

- (void)awakeFromNib
{	
	[[self window] setDelegate:self];
	
	_loader = [CVEmbeddedLoader new];
    if (_loader) {
    }
    else {
    }
	
    _config = [[NSMutableDictionary dictionaryWithDictionary:[[_loader config] dictionary]] retain];
	[self setUpModules];
	
	NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier:@""];
	[toolbar setDelegate:self];
	[toolbar autorelease];
	[[self window] setToolbar:toolbar];
	[[self window] center];	
	[self setActiveView:[m_moduleListController view] animate:NO];
}

- (void) dealloc
{
	[_loader release];
	[_config release];
	[super dealloc];
}

- (BOOL)updateConfigWithIdentifer:(NSString *)identifier dictionary:(NSDictionary *)dictionary
{
	if (!identifier || ![identifier length])
		return NO;
	if (!dictionary)
		return NO;
	
	[_config setValue:dictionary forKey:identifier];
	NSLog(@"update");
	NSLog([dictionary description]);
	
	return YES;
}
- (void)writeConfigWithIdentifer:(NSString *)identifier dictionary:(NSDictionary *)dictionary
{
	if ([self updateConfigWithIdentifer:identifier dictionary:dictionary])
		[self writeConfig];
}
- (void)writeConfig
{
	[[_loader config] sync];
	[[[_loader config] dictionary] removeAllObjects];
	[[[_loader config] dictionary] addEntriesFromDictionary:_config];
	[[_loader config] sync];
}
- (void)setActiveView:(NSView *)view animate:(BOOL)flag
{	
	NSRect windowFrame = [[self window] frame];
	windowFrame.size.height = [view frame].size.height + WINDOW_TITLE_HEIGHT;
	windowFrame.size.width = [view frame].size.width;
	windowFrame.origin.y = NSMaxY([[self window] frame]) - ([view frame].size.height + WINDOW_TITLE_HEIGHT);

	if ([[u_mainView subviews] count] != 0)
		[[[u_mainView subviews] objectAtIndex:0] removeFromSuperview];
	
	[[self window] setFrame:windowFrame display:YES animate:flag];		
	[u_mainView setFrame:[view frame]];
	[u_mainView addSubview:view];
}

- (void)windowWillClose:(NSNotification *)notification
{
	[NSApp terminate:self];
}

@end
