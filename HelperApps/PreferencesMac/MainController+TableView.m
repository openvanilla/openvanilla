//
//  MainController+TableView.m
//  OVPreference

#import "MainController+TableView.h"
#import "OVViewController.h"

@implementation MainController(TableView)

- (void)setActiveModuleSettingView:(NSView *)view
{
	NSArray *subViews = [NSArray arrayWithArray:[_moduleContentView subviews]];
	NSEnumerator *enumerator = [subViews objectEnumerator];
	NSView *subView = nil;
	while (subView = [enumerator nextObject]) {
		[subView removeFromSuperview];
	}
	[view setFrame:[_moduleContentView bounds]];
	[_moduleContentView addSubview:view];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [_controllersArray count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	NSString *identifier = [aTableColumn identifier];
	OVViewController *controller = [_controllersArray objectAtIndex:rowIndex];
	if ([identifier isEqualToString:@"localizedName"]) {
		return [controller localizedName];
	}
	return nil;
}
- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
	NSTableView *tableview = [aNotification object];
	NSInteger index = [tableview selectedRow];
	if (index >= 0 && index < [_controllersArray count]) {
		OVViewController *controller = [_controllersArray objectAtIndex:index];
		NSView *view = [controller view];
		[self setActiveModuleSettingView:view];
		[_titleLabel setStringValue:[controller localizedName]];
	}
}

@end
