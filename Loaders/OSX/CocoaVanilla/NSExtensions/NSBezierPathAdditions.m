//
//  NSBezierPathAdditions.m
//  NewDisplayServer
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "NSBezierPathAdditions.h"


@implementation NSBezierPath (CVBezelPathAdditions)
+ (NSBezierPath*)bezelPathWithRect:(NSRect)boundingRect radius:(float)radius
{
	NSBezierPath *path = [NSBezierPath bezierPath];
	float x1 = NSMinX(boundingRect);
	float y1 = NSMinY(boundingRect);
	float x2 = NSMaxX(boundingRect);
	float y2 = NSMaxY(boundingRect);
	
	[path moveToPoint:NSMakePoint(x1 + radius, y1)];
	[path lineToPoint:NSMakePoint(x2 - radius, y1)];
	[path appendBezierPathWithArcFromPoint:NSMakePoint(x2, y1) toPoint:NSMakePoint(x2, y1 + radius) radius:radius];
	[path lineToPoint:NSMakePoint(x2, y2 - radius)];
	[path appendBezierPathWithArcFromPoint:NSMakePoint(x2, y2) toPoint:NSMakePoint(x2 - radius , y2) radius:radius];
	[path lineToPoint:NSMakePoint(x1 + radius, y2)];
	[path appendBezierPathWithArcFromPoint:NSMakePoint(x1, y2) toPoint:NSMakePoint(x1 , y2 - radius) radius:radius];
	[path lineToPoint:NSMakePoint(x1, y1 + radius)];
	[path appendBezierPathWithArcFromPoint:NSMakePoint(x1, y1) toPoint:NSMakePoint(x1 + radius, y1) radius:radius];			
	[path closePath];

	return path;
}
@end
