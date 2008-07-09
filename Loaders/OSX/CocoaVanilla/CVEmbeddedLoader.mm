// CVEmbeddedLoader.m: Embedded loader
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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
#import "CVLoaderConfig.h"
#import "CVLoaderUtility.h"
#import "OVDisplayServer.h"

@interface CVDumpDisplayServer : NSObject <OVDisplayServer> {
    NSMutableString *msg;
}

- (NSString*)notifyMessage;
@end

@implementation CVEmbeddedLoader 
- (void)dealloc
{
    delete srv;
    [dpsrv release];
    [cfg release];
    [modlist release];
    [loadhistory release];
    [super dealloc];
}
- (id)init
{
    self = [super init];
    if (!self)
		return nil;

    dpsrv = [CVDumpDisplayServer new];
    srv = new CVService(CVGetUserSpacePath(), dpsrv);
    srv->setNotificationPosition((Point){1,1});     // to prevent "hide ghost notification" feature
    cfg = [[CVConfig alloc] initWithFile:CVGetUserConfigFilename() defaultData:nil];
    modlist = [NSMutableArray new];
    loadhistory = [NSMutableDictionary new];
    
    // load everything!
    [modlist addObjectsFromArray: CVLoadEverything(CVGetModuleLoadPath(), srv, nil, nil, loadhistory, nil)];
    return self;
}
- (CVConfig*)config
{
    return cfg;
}
- (CVService*)service
{
    return srv;
}
- (NSArray *)moduleList
{
    return modlist;
}
- (NSDictionary *)loadHistory
{
    return loadhistory;
}
- (NSString *)notifyMessage
{
    return [dpsrv notifyMessage];
}
- (void)clearNotification
{
    [dpsrv notifyClose];
}
@end

@implementation CVDumpDisplayServer
- (id)init
{
    if (self = [super init]) {
        msg=[NSMutableString new];
    }
    return self;
}
- (void)dealloc
{
    [msg release];
    [super dealloc];
}
- (NSString *)notifyMessage
{
    return msg;
}
- (void)setConfig:(NSDictionary*)cfg {}
- (void)candidateShow {}
- (void)candidateHide {}
- (void)candidateUpdate:(bycopy NSString *)s position:(Point)p {}
- (void)notifyMessage:(bycopy NSString *)s position:(Point)p
{
    [msg setString:s];
}
- (void)notifyClose
{
    [msg setString:@""];
}
- (void)notifyFade {}
- (void)aboutDialog {}
- (BOOL)ping
{
	return TRUE;
}
- (void)sendCharacterToCurrentComposingBuffer:(NSString *)text {}
- (void)sendStringToCurrentComposingBuffer:(NSString *)text {}
- (int)nextAvailableRemoteID {}
- (void)setCurrentComposingBufferRemoteID:(int)remoteID {}
@end
