//
//  NSBezierPathAdditions.h
//  NewDisplayServer
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSBezierPath (CVBezelPathAdditions)
+ (NSBezierPath*)fullBezelPathWithRect:(NSRect)boundingRect radius:(float)radius;
+ (NSBezierPath*)halfBezelPathWithRect:(NSRect)boundingRect radius:(float)radius;
@end
