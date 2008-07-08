//
//  OVDisplayController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVDisplayController.h"
#import "NSStringExtension.h"
#import "OVHotkeyField.h"

@implementation OVDisplayController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVDisplayController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void) dealloc
{
	[beepSound release];
	[super dealloc];
}

- (void)awakeFromNib
{
	[u_fontManager setDelegate:self];
	
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
	
	[d setValue:[NSString stringByColor:[NSColor whiteColor]] forKey:@"foreground"];
	[d setValue:[NSString stringByColor:[NSColor blueColor]] forKey:@"background"];
	[d setValue:@"TrebuchetMS" forKey:@"font"];
	[d setValue:[NSNumber numberWithFloat:14.0] forKey:@"size"];
	[d setValue:[NSNumber numberWithFloat:1.0] forKey:@"opacity"];
	[d setValue:@"default" forKey:@"notificationStyle"]; 	//default silent
	[d setValue:[NSNumber numberWithInt:1] forKey:@"useWindowAnimation"];

	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_foreground setColor:[[d valueForKey:@"foreground"] colorByString]];
	[u_background setColor:[[d valueForKey:@"background"] colorByString]];
	
	NSString *fontName = [d valueForKey:@"font"];
	CGFloat fontSize = [[d valueForKey:@"size"] floatValue];
	NSFont *font = [NSFont fontWithName:fontName size:14];
	
	[u_fontLabel setFont:font];
	[u_fontLabel setStringValue:[NSString stringWithFormat:@"%@ %.f", [font displayName], fontSize]];
	
	[u_useWindowAnimation setIntValue:[[d valueForKey:@"useWindowAnimation"] intValue]];
	NSString *style = [d valueForKey:@"notificationStyle"];
	if ([style isEqualToString:@"silent"]) {
		[u_notificationStyle setIntValue:0];
	}
	else {
		[d setValue:@"default" forKey:@"notificationStyle"];
		[u_notificationStyle setIntValue:1];
	}
	float opacity = [[d valueForKey:@"opacity"] floatValue];
	[u_opacityScroll setIntValue:(int)(opacity * 100)];
	[u_opacityLabel setStringValue:[NSString stringWithFormat:@"%d%%", (int)(opacity * 100)]];
	
	[self setDictionary:d];
	[u_preview changeConfig:[self dictionary]];
	[self update];
	
	[u_hotkeyField setModuleController:self];
}

- (IBAction)changeColor:(id)sender
{
	[self setValue:[NSString stringByColor:[u_foreground color]] forKey:@"foreground"];
	[self setValue:[NSString stringByColor:[u_background color]] forKey:@"background"];	
	[u_preview changeConfig:[self dictionary]];	
	[self updateAndWrite];
}
- (IBAction)changeOpacity:(id)sender
{
	[u_opacityLabel setStringValue:[NSString stringWithFormat:@"%d%%", [sender intValue]]];	
	CGFloat opacity = ([sender floatValue] / 100);
	[self setValue:[NSNumber numberWithFloat:opacity] forKey:@"opacity"];
	[self updateAndWrite];
}
- (IBAction)launchFontPanel:(id)sender
{
	NSString *fontName = [[self dictionary] valueForKey:@"font"];
	float fontSize = [[[self dictionary] valueForKey:@"size"] floatValue];
	NSFont *font = [NSFont fontWithName:fontName size:fontSize];
	
	[u_fontManager setSelectedFont:font isMultiple:NO];
	[[u_fontManager fontPanel:YES] orderFront:self];
}

- (void)changeFont:(id)sender
{
    NSFont *newfont = [sender convertFont:[[NSFontManager sharedFontManager] selectedFont]];
	float fontSize = [newfont pointSize];	
	[self setValue:[newfont fontName] forKey:@"font"];	
	[self setValue:[NSNumber numberWithFloat:fontSize] forKey:@"size"];
	
	[u_fontLabel setStringValue:[NSString stringWithFormat:@"%@ %.f", [newfont displayName], fontSize]];
	[u_preview changeConfig:[self dictionary]];	
	[self updateAndWrite];
}

- (IBAction)setDisplayServerSetting:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_useWindowAnimation intValue]] forKey:@"useWindowAnimation"];

	if ([u_notificationStyle intValue])
		[self setValue:@"default" forKey:@"notificationStyle"];
	else
		[self setValue:@"silent" forKey:@"notificationStyle"];
	[self updateAndWrite];

}
- (void)setShortcut:(NSString *)shortcut
{
	[super setShortcut:shortcut];
	if (![u_hotkeyField moduleController]) {
		[u_hotkeyField setModuleController:self];
	}
}
- (void)setShortcut:(NSString *)shortcut fromSender:(id)sender
{
	[super setShortcut:shortcut];
	[_delegate updateShortcut:shortcut forModule:@"fastIMSwitch"];
}
@end
