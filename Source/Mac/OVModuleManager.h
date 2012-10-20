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

@class OVToolTipWindowController;

namespace OpenVanilla {
    class OVLoaderServiceImpl;
    class OVCandidateServiceImpl;
    class OVInputMethod;
    typedef std::map<std::string, OVInputMethod *> OVInputMethodMap;
};

@interface OVModuleManager : NSObject
{
@private
    OpenVanilla::OVLoaderServiceImpl* _loaderService;
    OpenVanilla::OVCandidateServiceImpl* _candidateService;
    OVToolTipWindowController *_toolTipWindowController;

    NSMutableArray *_inputMethodIdentifiers;
    NSMutableDictionary *_customTableBasedInputMethodIdentifierTableNameMap;
    OpenVanilla::OVInputMethodMap* _inputMethodMap;

    OpenVanilla::OVInputMethod* _activeInputMethod;
    NSString *_currentLocale;
}

+ (OVModuleManager *)defaultManager;
- (void)selectInputMethod:(NSString *)identifier;
- (NSString *)localizedInputMethodName:(NSString *)identifier;
- (void)reload;
- (void)synchronizeActiveInputMethodSettings;

- (NSString *)alphanumericKeyboardLayoutForInputMethod:(NSString *)identifier;
- (BOOL)isTableBasedInputMethodIdentifier:(NSString *)identifier;
- (BOOL)canInstallCustomTableBasedInputMethodWithTablePath:(NSString *)path willOverrideBuiltInTable:(BOOL *)willOverride identifier:(NSString **)identifierIfInstalled localizedName:(NSString **)localizedNameIfInstalled error:(NSError **)error;
- (void)installCustomTableBasedInputMethodWithTablePath:(NSString *)path;
- (BOOL)isCustomTableBasedInputMethod:(NSString *)identifier;
- (BOOL)removeCustomTableBasedInputMethod:(NSString *)identifier error:(NSError **)error;

- (NSString *)filteredStringWithString:(NSString *)input;

@property (assign, readonly) OpenVanilla::OVLoaderServiceImpl* loaderService;
@property (assign, readonly) OpenVanilla::OVCandidateServiceImpl* candidateService;
@property (retain) OVToolTipWindowController *toolTipWindowController;

@property (assign, readonly) OpenVanilla::OVInputMethod* activeInputMethod;
@property (readonly) NSString *activeInputMethodIdentifier;
@property (readonly) NSArray *inputMethodIdentifiers;
@property (retain) NSString *currentLocale;

@property (assign) BOOL traditionalToSimplifiedChineseFilterEnabled;
@property (assign) BOOL simplifiedToTraditionalChineseFilterEnabled;

@property (copy) NSString *sharedAlphanumericKeyboardLayoutIdentifier;
@end

extern NSString *const OVModuleManagerDidReloadNotification;
extern NSString *const OVModuleManagerDidUpdateActiveInputMethodNotification;
