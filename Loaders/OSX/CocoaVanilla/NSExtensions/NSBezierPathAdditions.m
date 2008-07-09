// NSBezierPathAdditions.m
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
