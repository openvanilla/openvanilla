//
//  OVTableBasedModulePreferencesViewController.m
//  OpenVanilla
//
//  Created by Lukhnos D. Liu on 9/7/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import "OVTableBasedModulePreferencesViewController.h"

@implementation OVTableBasedModulePreferencesViewController

@synthesize fieldMaximumRadicalLength = _fieldMaximumRadicalLength;
@synthesize fieldClearReadingBufferAtCompositionError = _fieldClearReadingBufferAtCompositionError;
@synthesize fieldComposeWhileTyping = _fieldComposeWhileTyping;
@synthesize fieldSendFirstCandidateWithSpaceWithOnePageList = _fieldSendFirstCandidateWithSpaceWithOnePageList;
@synthesize fieldShouldComposeAtMaximumRadicalLength = _fieldShouldComposeAtMaximumRadicalLength;
@synthesize fieldUseSpaceAsFirstCandidateSelectionKey = _fieldUseSpaceAsFirstCandidateSelectionKey;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)setStateForButton:(NSButton *)button forKey:(NSString *)key
{
    [button setState:([self boolValueForKey:key] ? NSOnState : NSOffState)];
}

- (void)synchronize
{
    [super synchronize];
    [self setStateForButton:self.fieldClearReadingBufferAtCompositionError forKey:@"ClearReadingBufferAtCompositionError"];
    [self setStateForButton:self.fieldComposeWhileTyping forKey:@"ComposeWhileTyping"];
    [self setStateForButton:self.fieldShouldComposeAtMaximumRadicalLength forKey:@"ShouldComposeAtMaximumRadicalLength"];

    if ([self boolValueForKey:@"UseSpaceAsFirstCandidateSelectionKey"]) {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOnState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
    }
    else if ([self boolValueForKey:@"SendFirstCandidateWithSpaceWithOnePageList"]) {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOnState];
    }
    else {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
    }

    NSUInteger length = [self unsignedIntegerValueForKey:@"MaximumRadicalLength"];
    if (!length || length > 100) {
        length = 5;
    }

    [self.fieldMaximumRadicalLength selectItemWithTitle:@"5"];

    for (NSMenuItem *item in [[self.fieldMaximumRadicalLength menu] itemArray]) {
        if ([[item title] integerValue] == length) {
            [self.fieldMaximumRadicalLength selectItem:item];
            break;
        }
    }
}

- (IBAction)updateField:(id)sender
{
    if (sender == self.fieldUseSpaceAsFirstCandidateSelectionKey) {
        if ([self.fieldUseSpaceAsFirstCandidateSelectionKey state] == NSOnState) {
            [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
        }
    } else if (sender == self.fieldSendFirstCandidateWithSpaceWithOnePageList) {
        if ([self.fieldSendFirstCandidateWithSpaceWithOnePageList state] == NSOnState) {
            [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        }
    }

    [self setBoolValue:([self.fieldClearReadingBufferAtCompositionError state] == NSOnState) forKey:@"ClearReadingBufferAtCompositionError"];
    [self setBoolValue:([self.fieldComposeWhileTyping state] == NSOnState) forKey:@"ComposeWhileTyping"];
    [self setBoolValue:([self.fieldShouldComposeAtMaximumRadicalLength state] == NSOnState) forKey:@"ShouldComposeAtMaximumRadicalLength"];
    [self setBoolValue:([self.fieldUseSpaceAsFirstCandidateSelectionKey state] == NSOnState) forKey:@"UseSpaceAsFirstCandidateSelectionKey"];
    [self setBoolValue:([self.fieldSendFirstCandidateWithSpaceWithOnePageList state] == NSOnState) forKey:@"SendFirstCandidateWithSpaceWithOnePageList"];
}
@end
