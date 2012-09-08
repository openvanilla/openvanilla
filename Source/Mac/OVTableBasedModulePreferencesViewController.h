//
//  OVTableBasedModulePreferencesViewController.h
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/7/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVBasePreferencesViewController.h"

@interface OVTableBasedModulePreferencesViewController : OVBasePreferencesViewController
{
    NSPopUpButton *_fieldMaximumRadicalLength;
    NSButton *_fieldClearReadingBufferAtCompositionError;
    NSButton *_fieldComposeWhileTyping;
    NSButton *_fieldSendFirstCandidateWithSpaceWithOnePageList;
    NSButton *_fieldShouldCommitAtMaximumRadicalLength;
    NSButton *_fieldUseSpaceAsFirstCandidateSelectionKey;
}

- (IBAction)updateField:(id)sender;
@property (assign, nonatomic) IBOutlet NSPopUpButton *fieldMaximumRadicalLength;
@property (assign, nonatomic) IBOutlet NSButton *fieldClearReadingBufferAtCompositionError;
@property (assign, nonatomic) IBOutlet NSButton *fieldComposeWhileTyping;
@property (assign, nonatomic) IBOutlet NSButton *fieldSendFirstCandidateWithSpaceWithOnePageList;
@property (assign, nonatomic) IBOutlet NSButton *fieldShouldCommitAtMaximumRadicalLength;
@property (assign, nonatomic) IBOutlet NSButton *fieldUseSpaceAsFirstCandidateSelectionKey;
@end
