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

@end
