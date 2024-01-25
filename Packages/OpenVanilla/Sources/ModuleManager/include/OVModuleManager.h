//
// OVModuleManager.h
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import <Foundation/Foundation.h>
#import <map>

NS_ASSUME_NONNULL_BEGIN

@protocol ONCanInstallCheckResult <NSObject>
@end

@interface OVCanInstall : NSObject <ONCanInstallCheckResult>
@property (readonly, assign) BOOL willOverrideBuiltInTable;
@property (readonly, strong) NSString *identifierIfInstalled;
@property (readonly, strong) NSString *localizedNameIfInstalled;
@end

@interface OVCanNotInstall : NSObject <ONCanInstallCheckResult>
@property (readonly, strong) NSError *error;
@end

@class OVToolTipWindowController;

namespace OpenVanilla {
    class OVLoaderServiceImpl;
    class OVCandidateServiceImpl;
    class OVInputMethod;
    class OVAroundFilter;
    typedef std::map<std::string, OVInputMethod *> OVInputMethodMap;
};

@interface OVModuleManager : NSObject

- (void)selectInputMethod:(NSString *)identifier;
- (NSString *)localizedInputMethodName:(NSString *)identifier;
- (void)reload;
- (void)synchronizeActiveInputMethodSettings;
- (void)writeOutActiveInputMethodSettings;

- (NSString *)alphanumericKeyboardLayoutForInputMethod:(NSString *)identifier;
- (BOOL)isTableBasedInputMethodIdentifier:(NSString *)identifier;
- (id<ONCanInstallCheckResult>)canInstallCustomTableBasedInputMethodWithTablePath:(NSString *)path;
- (void)installCustomTableBasedInputMethodWithTablePath:(NSString *)path;
- (BOOL)isCustomTableBasedInputMethod:(NSString *)identifier;
- (BOOL)removeCustomTableBasedInputMethod:(NSString *)identifier error:(NSError **)error;

- (NSString *)filteredStringWithString:(NSString *)input;

@property (class, readonly) OVModuleManager *defaultManager;

@property (assign, readonly) OpenVanilla::OVLoaderServiceImpl* loaderService;
@property (assign, readonly) OpenVanilla::OVCandidateServiceImpl* candidateService;
@property (strong) OVToolTipWindowController *toolTipWindowController;

@property (assign, readonly) OpenVanilla::OVInputMethod* activeInputMethod;
@property (weak, readonly) NSString *activeInputMethodIdentifier;
@property (readonly, nonnull) NSArray <NSString *>  *inputMethodIdentifiers;
@property (strong) NSString *currentLocale;

@property (assign) BOOL traditionalToSimplifiedChineseFilterEnabled;
@property (assign) BOOL simplifiedToTraditionalChineseFilterEnabled;
@property (assign) BOOL associatedPhrasesAroundFilterEnabled;

@property (copy) NSString *sharedAlphanumericKeyboardLayoutIdentifier;

@property (assign, readonly) OpenVanilla::OVAroundFilter* associatedPhrasesModule;
@end

extern NSString *const OVModuleManagerDidReloadNotification;
extern NSString *const OVModuleManagerDidUpdateActiveInputMethodNotification;

NS_ASSUME_NONNULL_END
