//
//  OVUpdateChecker.m
//  OpenVanilla
//
//  Created by Lukhnos Liu on 10/19/12.
//  Copyright (c) 2012 The OpenVanilla Project. All rights reserved.
//

#import "OVUpdateChecker.h"
#import "OVConstants.h"
#import "OVNonModalAlertWindowController.h"

NSString *const OVUpdateCheckerDidFinishCheckingNotification = @"OVUpdateCheckerDidFinishCheckingNotification";

@interface OVUpdateChecker () <OVNonModalAlertWindowControllerDelegate>
- (void)cleanUp;
- (void)checkForUpdateForced:(BOOL)forced;
- (void)showPlistError;
@property (retain) NSDate *nextUpdateCheckDate;
@end

@implementation OVUpdateChecker
+ (OVUpdateChecker *)sharedInstance
{
    static OVUpdateChecker *instance;
    @synchronized(self) {
        if (!instance) {
            instance = [[OVUpdateChecker alloc] init];
        }
    }

    return instance;
}

- (void)dealloc
{
    [_connection cancel];
    [_connection release];
    [_data release];
    [super dealloc];
}

- (void)checkForUpdate
{
    [self checkForUpdateForced:YES];
}

- (void)checkForUpdateIfNeeded
{
    [self checkForUpdateForced:NO];
}

#pragma mark - Private methods

- (void)cleanUp
{
    [_connection release];
    _connection = nil;
    [_data release];
    _data = nil;
}

- (void)checkForUpdateForced:(BOOL)forced
{
    if (self.busy) {
        return;
    }

    if (!forced) {
        NSDate *nextCheckDate = self.nextUpdateCheckDate;
        NSDate *now = [NSDate date];

        // time to check?
        if (nextCheckDate && [nextCheckDate compare:now] != NSOrderedAscending) {
            return;
        }
    }

    NSURL *url = [NSURL URLWithString:OVUpdateCheckInfoURLString];
    if (!url) {
        return;
    }

    _forcedCheck = forced;

    NSURLRequest *request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringLocalCacheData timeoutInterval:30.0];
    _connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
    [_connection start];
}

- (void)showPlistError
{
#if DEBUG
    NSLog(@"Update check: plist error, forced check: %d", _forcedCheck);
#endif

    if (!_forcedCheck) {
        return;
    }

    [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Update Check Failed", nil) content:NSLocalizedString(@"The version information returned by the server is not valid.", nil) confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
}

#pragma mark - Properties

- (BOOL)busy
{
    return _connection != nil;
}

- (NSDate *)lastUpdateCheckDate
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:OVLastUpdateCheckTimeKey];
    if ([object isKindOfClass:[NSDate class]]) {
        return object;
    }

    return nil;
}

- (void)setLastUpdateCheckDate:(NSDate *)lastUpdateCheckDate
{
    [[NSUserDefaults standardUserDefaults] setObject:lastUpdateCheckDate forKey:OVLastUpdateCheckTimeKey];
}

- (NSDate *)nextUpdateCheckDate
{
    id object = [[NSUserDefaults standardUserDefaults] objectForKey:OVNextUpdateCheckTimeKey];
    if ([object isKindOfClass:[NSDate class]]) {
        return object;
    }

    return nil;
}

- (void)setNextUpdateCheckDate:(NSDate *)nextUpdateCheckDate
{
    [[NSUserDefaults standardUserDefaults] setObject:nextUpdateCheckDate forKey:OVNextUpdateCheckTimeKey];
}

#pragma mark - NSURLConnectionDataDelegate methods

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
#if DEBUG
    NSLog(@"Update check: connection error, forced check: %d, error: %@", _forcedCheck, error);
#endif

    NSDate *now = [NSDate date];
    [self setLastUpdateCheckDate:now];
    [self setNextUpdateCheckDate:[NSDate dateWithTimeInterval:OVNextUpdateCheckRetryInterval sinceDate:now]];

    [self cleanUp];
    [[NSNotificationCenter defaultCenter] postNotificationName:OVUpdateCheckerDidFinishCheckingNotification object:self];

    if (_forcedCheck) {
        [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Update Check Failed", nil) content:[NSString stringWithFormat:NSLocalizedString(@"There may be no internet connection or the server failed to respond.\n\nError message: %@", nil), [error localizedDescription]] confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    if (!_data) {
        _data = [[NSMutableData alloc] init];
    }

    [_data appendData:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    NSString *errorDescription = nil;
    id plist = [NSPropertyListSerialization propertyListFromData:_data mutabilityOption:NSPropertyListImmutable format:NULL errorDescription:&errorDescription];

    NSDate *now = [NSDate date];
    [self setLastUpdateCheckDate:now];
    [self setNextUpdateCheckDate:[NSDate dateWithTimeInterval:OVNextUpdateCheckInterval sinceDate:now]];

    [self cleanUp];
    [[NSNotificationCenter defaultCenter] postNotificationName:OVUpdateCheckerDidFinishCheckingNotification object:self];
    
    if (!plist) {
        [self showPlistError];
        return;
    }

#if DEBUG
    NSLog(@"update check plist: %@",plist);
#endif

    NSString *remoteVersion = [plist objectForKey:(id)kCFBundleVersionKey];
    if (!remoteVersion) {
        [self showPlistError];
        return;
    }

    NSString *remoteShortVersion = [plist objectForKey:@"CFBundleShortVersionString"];
    if (!remoteShortVersion) {
        [self showPlistError];
        return;
    }

    NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString *currentVersion = [infoDict objectForKey:(id)kCFBundleVersionKey];
    NSString *currentShortVersion = [infoDict objectForKey:@"CFBundleShortVersionString"];
    NSComparisonResult result  = [currentVersion compare:remoteVersion options:NSNumericSearch];

    if (result != NSOrderedAscending) {
        if (_forcedCheck) {
            [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Check for Update Completed", nil) content:NSLocalizedString(@"You are already using the latest version of OpenVanilla.", nil) confirmButtonTitle:NSLocalizedString(@"OK", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
        }

        return;
    }

    NSDictionary *versionDescriptions = [plist objectForKey:@"Description"];
    NSString *versionDescription = @"";
    if ([versionDescriptions isKindOfClass:[NSDictionary class]]) {
        NSString *locale = @"en";
        NSArray *tags = [NSLocale preferredLanguages];
        if ([tags count]) {
            locale = [tags objectAtIndex:0];
        }
        versionDescription = [versionDescriptions objectForKey:locale];
        if (!versionDescription) {
            versionDescription = @"";
        }
        else {
            versionDescription = [@"\n\n" stringByAppendingString:versionDescription];
        }
    }

    NSString *content = [NSString stringWithFormat:NSLocalizedString(@"You're currently using OpenVanilla %@ (%@), a new version %@ (%@) is now available. Do you want to visit openvanilla.org to download the version?%@", nil), currentShortVersion, currentVersion, remoteShortVersion, remoteVersion, versionDescription];

    [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"New Version Available", nil) content:content confirmButtonTitle:NSLocalizedString(@"Visit Website", nil) cancelButtonTitle:NSLocalizedString(@"Not Now", nil) cancelAsDefault:NO delegate:self];
}

#pragma mark OVNonModalAlertWindowControllerDelegate methods

- (void)nonModalAlertWindowControllerDidConfirm:(OVNonModalAlertWindowController *)controller
{
    if (controller.delegate == self) {
        [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:OVUpdateDownloadURLString]];
    }
}

- (void)nonModalAlertWindowControllerDidCancel:(OVNonModalAlertWindowController *)controller
{
    if (controller.delegate == self) {
        NSDate *now = [NSDate date];
        [self setNextUpdateCheckDate:[NSDate dateWithTimeInterval:OVNextUpdateCheckRemindLaterInterval sinceDate:now]];
    }
}
@end
