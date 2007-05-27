//
//  ovwidgerplugin.m
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#import "CVEmbeddedLoader.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
#import "CVKeyCode.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import "CVPreviewView.h"

#import "ovwidgerplugin.h"

@implementation ovwidgerplugin

// init

-(id)initWithWebView:(WebView*)w {
	NSLog(@"I'm in!");		
	self = [super init];
	srand(time(NULL));
	return self;
}

-(void)dealloc {
	[super dealloc];
}


-(void)windowScriptObjectAvailable:(WebScriptObject*)wso {
	[wso setValue:self forKey:@"OVFilter"];
}

+(NSString*)webScriptNameForSelector:(SEL)aSel {
	NSString *retval = nil;
	
	if (aSel == @selector(genList)) {
		retval = @"genList";	
	} else if (aSel == @selector(myConvert:filter:)) {
		retval = @"myConvert";
	} else if (aSel == @selector(logMessage:)) {
		retval = @"logMessage";
	} else {
		NSLog(@"\tunknown selector");
	}
	
	return retval;
}

+(BOOL)isSelectorExcludedFromWebScript:(SEL)aSel {
	if (aSel == @selector(genList) || aSel == @selector(myConvert:filter:) || 
		aSel == @selector(logMessage:)) {
		return NO;
	}
	return YES;
}

+(BOOL)isKeyExcludedFromWebScript:(const char*)k {
	return YES;
}

- (NSString *) genList {
    loader=[[CVEmbeddedLoader alloc] init];
    const char *lc=[loader service]->locale();	
    if (!loader) {
		return @"";
    } else {
		NSString* s = @"";
		NSArray *l=CVGetModulesByType([loader moduleList], @"OVOutputFilter");
		for (unsigned int i=0; i<[l count]; i++) {
			CVModuleWrapper *w=[l objectAtIndex:i];
			OVModule *ovm=[w module];
			NSString *name=[NSString stringWithUTF8String:ovm->localizedName(lc)]; 			
			if([w identifier]) {
				s = [s stringByAppendingFormat: @"<option value=\"%@\">%@</option>", [w identifier], name];
			}
		}
		return s;
	}
}

- (NSString *) myConvert:(NSString *)str filter:(NSString *) currentFilter {
	if([currentFilter isEqualToString:@""]) return @"";		
    loader=[[CVEmbeddedLoader alloc] init];
	CVModuleWrapper *m=CVFindModule([loader moduleList], currentFilter, @"OVOutputFilter");
	
	if (!m) {return @"Module not found!, module: %@", currentFilter;}
	
    NSMutableDictionary *md=[[[loader config] dictionary] valueForKey:currentFilter default:[[NSMutableDictionary new] autorelease]];	
    CVDictionary mcd(md);
    BOOL r=[m initializeWithConfig:&mcd service:[loader service]];
	
	if (!r) { return @"Error happens!";}
	
	OVOutputFilter *of=(OVOutputFilter*)[m module];
	[loader clearNotification];
	NSString *output= @"";
	output=[NSString stringWithUTF8String:of->process([str UTF8String],[loader service])];
	return output;
}

- (void) logMessage:(NSString *)str {
	NSLog(@"JavaScript says: %@", str);
}

@end
