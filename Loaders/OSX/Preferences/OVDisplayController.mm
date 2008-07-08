//
//  OVDisplayController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVDisplayController.h"


@implementation OVDisplayController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVDisplayController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}


@end
