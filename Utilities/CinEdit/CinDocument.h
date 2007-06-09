// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#import <Cocoa/Cocoa.h>
#import "keyTable.h"
#import "charTable.h"


@interface CinDocument : NSDocument
{
    IBOutlet id listChardef;
    IBOutlet id listKeyname;
    IBOutlet id txtCname;
    IBOutlet id txtEname;
    IBOutlet id txtTcname;
    IBOutlet id txtScname;
    IBOutlet id txtSelkey;
    IBOutlet id msgIndicator;
    IBOutlet id msgText;	
    IBOutlet id msgWindow;	

	NSString *_ename;
	NSString *_cname;
	NSString *_tcname;
	NSString *_scname;
	NSString *_selkey;
	NSString *_endkey;
	
	keyTable * k;
	charTable * c;	
	NSString *currentPath;
	NSArray *ka;
	NSArray *ca;	
	BOOL isEdited;
}
- (IBAction)chardefAdd:(id)sender;
- (IBAction)chardefRemove:(id)sender;
- (IBAction)keynameAdd:(id)sender;
- (IBAction)keynameRemove:(id)sender;
@end
