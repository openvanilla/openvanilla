//
//  OVUpdateChecker.h
//  OpenVanilla
//
//  Created by Lukhnos Liu on 10/19/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface OVUpdateChecker : NSObject
{
    BOOL _forcedCheck;
    NSURLConnection *_connection;
    NSMutableData *_data;
}

+ (OVUpdateChecker *)sharedInstance;
- (void)checkForUpdate;
- (void)checkForUpdateIfNeeded;

@property (weak, readonly) NSDate *lastUpdateCheckDate;
@property (readonly) BOOL busy;
@end

extern NSString *const OVUpdateCheckerDidFinishCheckingNotification;
