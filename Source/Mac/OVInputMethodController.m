//
// OVInputMethodController.m
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "OVInputMethodController.h"

#if DEBUG
    #define IMEDebug NSLog
#else
    #define IMEDebug(...)
#endif

@implementation OVInputMethodController
- (void)dealloc
{
    [super dealloc];
}

- (id)initWithServer:(IMKServer *)server delegate:(id)aDelegate client:(id)client
{
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
    
    self = [super initWithServer:server delegate:aDelegate client:client];
	if (self) {
	}
	
	return self;
}

- (NSMenu *)menu
{
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
    return nil;
}

#pragma mark IMKStateSetting protocol methods

- (void)activateServer:(id)client
{
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
}

- (void)deactivateServer:(id)client
{
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
}

- (void)commitComposition:(id)client 
{    
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
}

- (BOOL)handleEvent:(NSEvent *)inEvent client:(id)client
{
    IMEDebug(@"%s", __PRETTY_FUNCTION__);
    return NO;
}

@end
