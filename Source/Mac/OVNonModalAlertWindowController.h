//
//  OVNonModalAlertWindowController.h
//  OpenVanilla
//
//  Created by Lukhnos Liu on 10/17/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class OVNonModalAlertWindowController;

@protocol OVNonModalAlertWindowControllerDelegate <NSObject>
- (void)nonModalAlertWindowControllerDidConfirm:(OVNonModalAlertWindowController *)controller;

@optional
- (void)nonModalAlertWindowControllerDidCancel:(OVNonModalAlertWindowController *)controller;
@end

@interface OVNonModalAlertWindowController : NSWindowController

+ (OVNonModalAlertWindowController *)sharedInstance;
- (void)showWithTitle:(NSString *)title content:(NSString *)content confirmButtonTitle:(NSString *)confirmTitle cancelButtonTitle:(NSString *)cancelButtonTitle cancelAsDefault:(BOOL)cancelAsDefault delegate:(id<OVNonModalAlertWindowControllerDelegate>)delegate;
- (IBAction)confirmButtonAction:(id)sender;
- (IBAction)cancelButtonAction:(id)sender;

@property (weak) IBOutlet NSTextField *titleTextField;
@property (weak) IBOutlet NSTextField *contentTextField;
@property (weak) IBOutlet NSButton *confirmButton;
@property (weak) IBOutlet NSButton *cancelButton;
@property (weak) id<OVNonModalAlertWindowControllerDelegate> delegate;
@end
