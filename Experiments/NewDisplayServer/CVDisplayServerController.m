//
//  CVDisplayServerController.m
//  NewDisplayServer
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "CVBubbleTextWindow.h"
#import "CVDisplayServerController.h"

#define CVNotificationFadeWait	0.5		// fade after 0.5 sec
#define CVNotificationInterval	0.05	// 0.05 sec/frame
#define CVNotificationFadeStep	0.025	// alpha decreases by 0.025 per frame

NSPoint CVFixWindowOriginWithTSMCoordinate(NSRect windowRect, Point oldStylePoint);
NSColor *CVColorFromRGBValueString(NSString *string);

@implementation CVDisplayServerController
- (void)awakeFromNib
{
	NSConnection *connection = [NSConnection defaultConnection];
	[connection setRootObject:self];
	
	if (![connection registerName:CVDisplayServerName]) {
		[[NSApplication sharedApplication] terminate:self];
		return;
	}
	
	_fadeAlpha = 1.0;
	_fadeTimer = nil;
}
- (IBAction)updateTextAction:(id)sender
{
	[_candidateWindow orderFront:self];
	[(CVBubbleTextWindow*)_candidateWindow setText:[_userText stringValue]];
}
- (void)setConfig:(bycopy NSDictionary*)cfg
{
	NSString *background = [cfg objectForKey:@"background"];
	if (!background) background = @"0.0 0.0 0.5";
	
	NSString *foreground = [cfg objectForKey:@"foreground"];
	if (!foreground) background = @"1.0 1.0 1.0";
	
	NSString *opacity = [cfg objectForKey:@"opacity"];
	if (!opacity) opacity = @"1.0";
	
	NSString *fontName = [cfg objectForKey:@"font"];
	if (!fontName) fontName = @"LucidaGrande";
	
	NSString *fontSize = [cfg objectForKey:@"size"];
	if (!fontSize) fontSize = @"24";
	
	NSFont *font = [NSFont fontWithName:fontName size:[fontSize floatValue]];
	
	[(CVBubbleTextWindow*)_candidateWindow setFont:font];
	[(CVBubbleTextWindow*)_candidateWindow setBubbleBackgroundColor:CVColorFromRGBValueString(background)];
	[(CVBubbleTextWindow*)_candidateWindow setTextColor:CVColorFromRGBValueString(foreground)];
	[_candidateWindow setAlphaValue:[opacity floatValue]];
	
	[(CVBubbleTextWindow*)_notificationWindow setFont:font];
	[(CVBubbleTextWindow*)_notificationWindow setBubbleBackgroundColor:CVColorFromRGBValueString(background)];
	[(CVBubbleTextWindow*)_notificationWindow setTextColor:CVColorFromRGBValueString(foreground)];	
	[_notificationWindow setAlphaValue:1.0];
}
- (void)candidateShow
{
	[_candidateWindow orderFront:self];
}
- (void)candidateHide
{
	[_candidateWindow orderOut:self];
}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p
{
	[(CVBubbleTextWindow*)_candidateWindow setText:s];
	[(CVBubbleTextWindow*)_candidateWindow updateWindowSizeWithAnimation];
	NSRect frame = [_candidateWindow frame];
	
	Point point;
	point.h = p.h;
	point.v = p.v + (short)frame.size.height;
	
	[_candidateWindow setFrameOrigin:CVFixWindowOriginWithTSMCoordinate(frame, point)];
}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p
{
	[(CVBubbleTextWindow*)_notificationWindow setText:s];
	[_notificationWindow updateWindowSizeWithAnimation];
	
	NSRect frame = [_notificationWindow frame];	
	Point point;
	point.h = p.h;
	point.v = p.v + (short)frame.size.height;
	
	if ([_candidateWindow isVisible]) {
		// pad the window down a bit
		point.v += (short)[_candidateWindow frame].size.height + 2;
	}
	
	[self stopTimer];
	[_notificationWindow setFrameOrigin:CVFixWindowOriginWithTSMCoordinate(frame, point)];	
	[_notificationWindow orderFront:self];
}
- (void)notifyClose
{
	[_notificationWindow orderOut:self];
}
- (void)notifyFade
{
	if (_fadeTimer)
		[self stopTimer];

	_fadeTimer = [NSTimer scheduledTimerWithTimeInterval:CVNotificationFadeWait target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
}
- (void)aboutDialog
{
    [_aboutWindow setLevel:NSScreenSaverWindowLevel];
	[_aboutWindow orderFront:self];
}
- (BOOL)ping
{
	return YES;
}
- (void)stopTimer {
	if (_fadeTimer)
		[_fadeTimer invalidate];
		
	_fadeTimer = nil;
	[_notificationWindow setAlphaValue:1.0];
}
- (void)fadeStart {
	// _fadeTimer is now invalidated

	_fadeAlpha = 1.0;
	_fadeTimer = [NSTimer scheduledTimerWithTimeInterval:CVNotificationInterval target:self selector:@selector(fadeWork) userInfo:nil repeats:YES];
}
- (void)fadeWork {
	_fadeAlpha -= CVNotificationFadeStep;

	if (_fadeAlpha < 0.0) {
		[_notificationWindow orderOut:self];
        [self stopTimer];
    }
	else {
		[_notificationWindow setAlphaValue:_fadeAlpha];
	}
}
@end

NSPoint CVFixWindowOriginWithTSMCoordinate(NSRect windowRect, Point oldStylePoint)
{
	NSPoint point = NSMakePoint((float)oldStylePoint.h, (float)oldStylePoint.v);
	
	// let's find where the point falls in first
	NSRect frame = [[NSScreen mainScreen] frame];
	BOOL hasFocus = NO;
	
	NSArray *screens = [NSScreen screens];
	int i, c = [screens count];
	for (i = 0; i < c; i++) {
		NSScreen *screen = [screens objectAtIndex:i];
		NSRect screenFrame = [screen frame];

		if (!hasFocus && point.x >= NSMinX(screenFrame) && point.x <= NSMaxX(screenFrame)) {
			frame = screenFrame;
			hasFocus = YES;
		}
	}
	
	if (hasFocus) {
		point.y = [[NSScreen mainScreen] frame].size.height - point.y;
		
		// we don't want the window to be out of screen, so...
		if (point.x > NSMaxX(frame) - windowRect.size.width) point.x = NSMaxX(frame) - windowRect.size.width;
		if (point.y > NSMaxY(frame) - windowRect.size.height) point.y = NSMaxY(frame) - windowRect.size.height;
		if (point.x < NSMinX(frame)) point.x = NSMinX(frame);
		if (point.y < NSMinY(frame)) point.y = NSMinY(frame);
	}
	else {
		// can't find a proper focus (ie. a screen where the point falls in), so we set it to center
		point.x = frame.origin.x + (frame.size.width - windowRect.size.width) / 2.0;
		point.y = frame.origin.y + (frame.size.height - windowRect.size.height) / 2.0;
	}
	
	return point;
}

NSColor *CVColorFromRGBValueString(NSString *string)
{
	NSArray *components = [string componentsSeparatedByString:@" "];
	NSString *red = [components objectAtIndex:0];
	NSString *green = [components objectAtIndex:1];
	NSString *blue = [components objectAtIndex:2];
	
	if (!red) red = @"0.0";
	if (!green) green = @"0.0";
	if (!blue) blue = @"0.0";
	
	return [NSColor colorWithCalibratedRed:[red floatValue] green:[green floatValue] blue:[blue floatValue] alpha:1.0];
}
