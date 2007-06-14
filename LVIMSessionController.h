//
//	File:		InputMethodSessionController.h of «PROJECTNAME»
//
//  Created by «FULLUSERNAME» on «DATE».
//
//  Copyright «ORGANIZATIONNAME» «YEAR». All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>

@interface LVIMSessionController : IMKInputController {
	NSMutableString *composed;
}
- (int)tscbGetX;
- (int)tscbGetY;
- (void)tscbClear;
- (void)tscbSend;
- (void)tscbUpdate:(BOOL)send withCursor:(int)cursor from:(int)markFrom to:(int)markTo;
- (void)tscbAppend:(NSString*)s;
- (BOOL)tscbIsEmpty;
- (NSString*)tscbGetContent;
@end
