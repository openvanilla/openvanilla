//
// LVUIController.mm
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
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

#import "LVUIController.h"

@implementation LVUIController

- (void)awakeFromNib
{
	_fontHeight = 20.0;
}
- (void)setFontHeight:(CGFloat)fontHeight
{
	_fontHeight = fontHeight;
}
- (NSPoint)estimatePoint:(NSPoint)point rect:(NSRect)rect
{
	NSRect frameRect = [_candidateWindow frame];
	
	if (point.x < NSMinX(rect))
		point.x = NSMinX(rect);
	if (point.x + frameRect.size.width> NSMaxX(rect))
		point.x = NSMaxX(rect) - frameRect.size.width;
	
	if (point.y < NSMinY(rect))
		point.y = NSMinY(rect) - frameRect.size.height;
	else if ((point.y - frameRect.size.height) < NSMinY(rect))
		point.y += _fontHeight + 5;
	else
		point.y -= frameRect.size.height;
	
	return point;
}
- (void)showCandidateWindow
{
	if (![_candidateWindow isVisible]) {
		[_candidateWindow orderFront:self];
	}
}
- (void)hideCandidateWindow
{
	if ([_candidateWindow isVisible]) {
		[_candidateWindow orderOut:self];
	}
}
- (void)setCandidateWindowOrigin:(NSPoint)origin
{
	[_candidateWindow setFrameOrigin:origin];
}
- (void)updateCandidateText:(NSString *)text
{
	[_candidateTextView setSimpleText:text];	
	[_candidateTextView setNeedsDisplay:YES];
	[_candidateWindow setContentSize:[_candidateTextView boundingRectForText].size];
}
- (void)showTooltipWithText:(NSString *)text atPoint:(NSPoint)point
{
	[_tooltipTextView setSimpleText:text];	
	[_tooltipTextView setNeedsDisplay:YES];
	[_tooltipWindow setContentSize:[_tooltipTextView boundingRectForText].size];	

	NSRect frameRect = [_tooltipWindow frame];
    point.y -= (frameRect.size.height + 5.0);
	[_tooltipWindow setFrameOrigin:point];	
	
	if (![_tooltipWindow isVisible]) {
		[_tooltipWindow orderFront:self];
	}
	
}
- (void)hideTooltip
{
	if ([_tooltipWindow isVisible]) {
		[_tooltipWindow orderOut:self];
	}
}
@end
