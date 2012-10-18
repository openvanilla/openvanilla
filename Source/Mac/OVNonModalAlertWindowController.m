//
//  OVNonModalAlertWindowController.m
//  OpenVanilla
//
//  Created by Lukhnos Liu on 10/17/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import "OVNonModalAlertWindowController.h"

@implementation OVNonModalAlertWindowController
@synthesize titleTextField = _titleTextField;
@synthesize contentTextField = _contentTextField;
@synthesize confirmButton = _confirmButton;
@synthesize cancelButton = _cancelButton;

+ (OVNonModalAlertWindowController *)sharedInstance
{
    static OVNonModalAlertWindowController *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[OVNonModalAlertWindowController alloc] initWithWindowNibName:@"OVNonModalAlertWindowController"];
        [instance window];
    });
    return instance;
}

- (void)showWithTitle:(NSString *)title content:(NSString *)content confirmButtonTitle:(NSString *)confirmTitle cancelButtonTitle:(NSString *)cancelButtonTitle cancelAsDefault:(BOOL)cancelAsDefault delegate:(id<OVNonModalAlertWindowControllerDelegate>)delegate;
{
    NSRect oldFrame = [self.confirmButton frame];
    [self.confirmButton setTitle:confirmTitle];
    [self.confirmButton sizeToFit];

    NSRect newFrame = [self.confirmButton frame];

    newFrame.size.width = MAX(90.0, newFrame.size.width);
    newFrame.origin.x += (oldFrame.size.width - newFrame.size.width);
    [self.confirmButton setFrame:newFrame];

    if (cancelButtonTitle) {
        [self.cancelButton setTitle:cancelButtonTitle];
        [self.cancelButton sizeToFit];
        NSRect adjustedFrame = [self.cancelButton frame];
        adjustedFrame.size.width = MAX(90.0, adjustedFrame.size.width);
        adjustedFrame.origin.x = newFrame.origin.x - adjustedFrame.size.width;
        [self.cancelButton setFrame:adjustedFrame];
        [self.cancelButton setHidden:NO];
    }
    else {
        [self.cancelButton setHidden:YES];
    }

    [self.cancelButton setNextKeyView:self.confirmButton];
    [self.confirmButton setNextKeyView:self.cancelButton];

    if (cancelButtonTitle) {
        if (cancelAsDefault) {
            [[self window] setDefaultButtonCell:[self.cancelButton cell]];
        }
        else {
            [self.cancelButton setKeyEquivalent:@" "];
            [[self window] setDefaultButtonCell:[self.confirmButton cell]];
        }
    }
    else {
        [[self window] setDefaultButtonCell:[self.confirmButton cell]];
    }

    [self.titleTextField setStringValue:title];

    oldFrame = [self.contentTextField frame];
    [self.contentTextField setStringValue:content];

    NSRect infiniteHeightFrame = oldFrame;
    infiniteHeightFrame.size.height = 10240;
    NSSize newSize = [[self.contentTextField cell] cellSizeForBounds:infiniteHeightFrame];
    newFrame = [content boundingRectWithSize:infiniteHeightFrame.size options:0 attributes:[NSDictionary dictionaryWithObjectsAndKeys:[self.contentTextField font], NSFontAttributeName, nil]];
    newFrame.origin = oldFrame.origin;
    newFrame.size = newSize;
    newFrame.origin.y -= (newSize.height - oldFrame.size.height);
    [self.contentTextField setFrame:newFrame];

    NSRect windowFrame = [[self window] frame];
    windowFrame.size.height += (newFrame.size.height - oldFrame.size.height);

    [[self window] setLevel:CGShieldingWindowLevel() + 1];
    [[self window] setFrame:windowFrame display:YES];
    [[self window] center];
    [[self window] makeKeyAndOrderFront:self];
}

- (IBAction)confirmButtonAction:(id)sender
{
    [_delegate nonModalAlertWindowControllerDidConfirm:self];
    [[self window] orderOut:self];
}

- (IBAction)cancelButtonAction:(id)sender
{
    [self cancel:sender];
}

- (void)cancel:(id)sender
{
    if ([_delegate respondsToSelector:@selector(nonModalAlertWindowControllerDidCancel:)]) {
        [_delegate nonModalAlertWindowControllerDidCancel:self];
    }

    _delegate = nil;

    [[self window] orderOut:self];
}

@end
