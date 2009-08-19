//
//  MainController+Toolbar.m
//  OVPreference

#import "MainController+Toolbar.h"

static NSString *toolbarIdentifier = @"Toolbar";
static NSString *loaderSettingToolbarItemIndentifier = @"Loader";
static NSString *modulesSettingToolbarItemIndentifier = @"Modules";

@implementation MainController(Toolbar)

- (void)initToolbar
{
	NSToolbar *toolbar = [[[NSToolbar alloc] initWithIdentifier:toolbarIdentifier] autorelease];
	[toolbar setDelegate:self];
	[toolbar setSelectedItemIdentifier:modulesSettingToolbarItemIndentifier];
	[[self window] setToolbar:toolbar];
}

- (void)setActiveView:(NSView *)view animate:(BOOL)flag
{	
	NSRect windowFrame = [[self window] frame];
	windowFrame.size.height = [view frame].size.height + WINDOW_TITLE_HEIGHT;
	windowFrame.size.width = [view frame].size.width;
	windowFrame.origin.y = NSMaxY([[self window] frame]) - ([view frame].size.height + WINDOW_TITLE_HEIGHT);
	
	NSView *mainView = [[self window] contentView];
	
	if ([[mainView subviews] count] != 0)
		[[[mainView subviews] objectAtIndex:0] removeFromSuperview];
	
	[[self window] setFrame:windowFrame display:YES animate:flag];		
	[mainView setFrame:[view frame]];
	[mainView addSubview:view];
}


- (void)toggleActivePreferenceView:(id)sender
{
	NSView *view;
	
//	if ([[sender itemIdentifier] isEqualToString:loaderSettingToolbarItemIndentifier])
//		view = [m_displayController view];
//	else if ([[sender itemIdentifier] isEqualToString:modulesSettingToolbarItemIndentifier])
//		view = [m_moduleListController view];
//	
//	[self setActiveView:view animate:YES];
//	[[self window] setTitle:MSG([sender itemIdentifier])];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,				
			nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar 
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,
			nil];
}

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:
//			loaderSettingToolbarItemIndentifier,
			modulesSettingToolbarItemIndentifier,
			nil];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)willBeInserted 
{
	NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
	if ([identifier isEqualToString:loaderSettingToolbarItemIndentifier]) {
		[item setLabel:NSLocalizedString(loaderSettingToolbarItemIndentifier, @"")];
		[item setImage:[NSImage imageNamed:@"General"]];	
		[item setTarget:self];
		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else if ([identifier isEqualToString:modulesSettingToolbarItemIndentifier]) {
		[item setLabel:NSLocalizedString(modulesSettingToolbarItemIndentifier, @"")];
		[item setImage:[NSImage imageNamed:@"Modules"]];
		[item setTarget:self];
		[item setAction:@selector(toggleActivePreferenceView:)];
	}
	else
		item = nil;
	return item; 
}

@end
