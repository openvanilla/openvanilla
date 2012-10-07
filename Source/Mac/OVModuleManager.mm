//
// OVModuleManager.mm
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

#import "OVModuleManager.h"
#import "OVConstants.h"
#import "OVLoaderServiceImpl.h"
#import "OVCandidateServiceImpl.h"
#import "OVIMTableBased.h"
#import "OVIMArray.h"
#import "OVPlistBackedKeyValueMapImpl.h"
#import <map>

extern NSString *const OVModuleManagerDidUpdateActiveInputMethodNotification = @"OVModuleManagerDidUpdateActiveInputMethodNotification";

using namespace OpenVanilla;

typedef map<string, OVInputMethod *> OVInputMethodMap;

static string InputMethodConfigIdentifier(const string& identifier)
{
    if (identifier.find(".") != string::npos) {
        return identifier;
    }
    
    return string("org.openvanilla.module.") + identifier;
}

@interface OVModuleManager ()
{
    NSMutableArray *_inputMethodIdentifiers;
}
- (BOOL)canSelectInputMethod:(NSString *)identifier;
- (void)handleLocaleChangeNotification:(NSNotification *)aNotification;
@property (retain) NSString *cachedLocale;
@property (assign) OVInputMethodMap* inputMethodMap;
@end

@implementation OVModuleManager
@dynamic activeInputMethodIdentifier;
@dynamic sharedAlphanumericKeyboardLayoutIdentifier;
@synthesize loaderService = _loaderService;
@synthesize candidateService = _candidateService;
@synthesize activeInputMethod = _activeInputMethod;
@synthesize inputMethodMap = _inputMethodMap;
@synthesize cachedLocale = _cachedLocale;

+ (OVModuleManager *)defaultManager
{
    static OVModuleManager *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[OVModuleManager alloc] init];
    });

    return instance;
}

- (id)init
{
    self = [super init];
    if (self) {
        _loaderService = new OVLoaderServiceImpl;
        _candidateService = new OVCandidateServiceImpl(_loaderService);
        _inputMethodMap = new OVInputMethodMap;
        _inputMethodIdentifiers = [[NSMutableArray alloc] init];
        _cachedLocale = [@"en" retain];

        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleLocaleChangeNotification:) name:NSCurrentLocaleDidChangeNotification object:nil];

        // populate default settings; input method identifier and font name can take care of themselves

        NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
        if (![userDefaults objectForKey:OVCandidateListStyleNameKey]) {
            [userDefaults setObject:OVDefaultCandidateListStyleName forKey:OVCandidateListStyleNameKey];
        }

        if (![userDefaults objectForKey:OVCandidateListTextSizeKey]) {
            [userDefaults setInteger:OVDefaultCandidateListTextSize forKey:OVCandidateListTextSizeKey];
        }

        if (![userDefaults objectForKey:OVMakeSoundFeedbackOnInputErrorKey]) {
            [userDefaults setBool:YES forKey:OVMakeSoundFeedbackOnInputErrorKey];
        }

        if (![userDefaults objectForKey:OVCheckForUpdateKey]) {
            [userDefaults setBool:YES forKey:OVCheckForUpdateKey];
        }
    }
    return self;
}

- (void)dealloc
{
    delete _loaderService;
    delete _candidateService;
    for (OVInputMethodMap::iterator i = _inputMethodMap->begin(), e = _inputMethodMap->end(); i != e; ++i) {
        delete (*i).second;
    }
    delete _inputMethodMap;
    [_inputMethodIdentifiers release];
    [_cachedLocale release];
    [super dealloc];
}

- (void)selectInputMethod:(NSString *)identifier
{
    OVInputMethod *chosenInputMethod = 0;
    if ([identifier length]) {
        OVInputMethodMap::const_iterator f = _inputMethodMap->find([identifier UTF8String]);
        if (f != _inputMethodMap->end()) {
            chosenInputMethod = (*f).second;
        }
    }

    [[NSUserDefaults standardUserDefaults] synchronize];
    if (!chosenInputMethod) {
        _activeInputMethod = 0;
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:OVActiveInputMethodIdentifierKey];
        [[NSNotificationCenter defaultCenter] postNotificationName:OVModuleManagerDidUpdateActiveInputMethodNotification object:self];
    }
    else {
        OVPlistBackedKeyValueMapImpl kvmi((CFStringRef)identifier);
        OVKeyValueMap kvm(&kvmi);
        chosenInputMethod->loadConfig(&kvm, _loaderService);
        chosenInputMethod->saveConfig(&kvm, _loaderService);
        if (chosenInputMethod != _activeInputMethod) {
            _activeInputMethod = chosenInputMethod;
            [[NSUserDefaults standardUserDefaults] setObject:identifier forKey:OVActiveInputMethodIdentifierKey];
            [[NSNotificationCenter defaultCenter] postNotificationName:OVModuleManagerDidUpdateActiveInputMethodNotification object:self];
        }
    }
}

- (NSString *)localizedInputMethodName:(NSString *)identifier
{
    OVInputMethodMap::const_iterator f = _inputMethodMap->find([identifier UTF8String]);
    if (f != _inputMethodMap->end()) {
        OVInputMethod *inputMethod = (*f).second;
        string locale = [self.cachedLocale UTF8String];
        string localizedName = inputMethod->localizedName(locale);
        NSString *result = [NSString stringWithUTF8String:localizedName.c_str()];
        return result;
    }
    else {
        return nil;
    }    
}

- (void)reload
{
    [_inputMethodIdentifiers removeAllObjects];

    for (OVInputMethodMap::iterator i = _inputMethodMap->begin(), e = _inputMethodMap->end(); i != e; ++i) {
        delete (*i).second;
    }

    _activeInputMethod = 0;
    _inputMethodMap->clear();

    // TODO: Load user tables
    NSArray *basicTables = [NSArray arrayWithObjects:@"cj-ext.cin", @"simplex-ext.cin", @"dayi3-patched.cin", @"ehq-symbols.cin", nil];
    NSString *tableRoot = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables/TableBased"];

    vector<OVInputMethod*> inputMethods;
    for (NSString *tableName in basicTables) {
        NSString *tablePath = [tableRoot stringByAppendingPathComponent:tableName];

        OVInputMethod *inputMethod = new OVIMTableBased([tablePath UTF8String]);
        inputMethods.push_back(inputMethod);
    }

    // TODO: Array always use US keyboard layout
    NSString *arrayTableRoot = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables/Array"];
    OVInputMethod *inputMethod = new OVIMArray([arrayTableRoot UTF8String]);
    inputMethods.push_back(inputMethod);

    for (vector<OVInputMethod*>::iterator i = inputMethods.begin(), e = inputMethods.end(); i != e; ++i) {
        OVInputMethod* inputMethod = *i;
        OVPathInfo info;
        bool result = inputMethod->initialize(&info, self.loaderService);
        if (!result) {
            delete inputMethod;
        }
        else {
            string identifier = InputMethodConfigIdentifier(inputMethod->identifier());
            _inputMethodMap->operator[](identifier) = inputMethod;
            [_inputMethodIdentifiers addObject:[NSString stringWithUTF8String:identifier.c_str()]];
        }
    }

    [self handleLocaleChangeNotification:nil];
    [self synchronizeActiveInputMethodSettings];
}

- (void)synchronizeActiveInputMethodSettings
{
    [[NSUserDefaults standardUserDefaults] synchronize];
    NSString *activeInputMethodIdentifier = [[NSUserDefaults standardUserDefaults] stringForKey:OVActiveInputMethodIdentifierKey];
    
    if ([self canSelectInputMethod:activeInputMethodIdentifier]) {
        [self selectInputMethod:activeInputMethodIdentifier];
    }
    else {
        // default input method
        if ([self canSelectInputMethod:OVDefaultInputMethodIdentifier]) {
            [self selectInputMethod:OVDefaultInputMethodIdentifier];
        }
        else {
            [[NSUserDefaults standardUserDefaults] removeObjectForKey:OVActiveInputMethodIdentifierKey];
        }
    }

    NSString *candidateFontName = [[NSUserDefaults standardUserDefaults] stringForKey:OVCandidateTextFontNameKey];
    NSInteger candidateFontSize = [[NSUserDefaults standardUserDefaults] integerForKey:OVCandidateListTextSizeKey];

    if (candidateFontSize < OVMinCandidateListTextSize) {
        candidateFontSize = OVMinCandidateListTextSize;
    }
    else if (candidateFontSize > OVMaxCandidateListTextSize) {
        candidateFontSize = OVMaxCandidateListTextSize;
    }

    _candidateService->applyFontSettings(candidateFontName, candidateFontSize);

    NSString *candidateStyle = [[NSUserDefaults standardUserDefaults] stringForKey:OVCandidateListStyleNameKey];
    if (![candidateStyle isEqual:OVHorizontalCandidateListStyleName]) {
        _candidateService->useVerticalCandidatePanel();
    }
    else {
        NSLog(@"use hori!");
        _candidateService->useHorizontalCandidatePanel();
    }
}

- (NSString *)alphanumericKeyboardLayoutForInputMethod:(NSString *)identifier
{
    OVInputMethodMap::const_iterator f = _inputMethodMap->find([identifier UTF8String]);
    if (f == _inputMethodMap->end()) {
        return self.sharedAlphanumericKeyboardLayoutIdentifier;
    }

    NSString *layout = nil;
    id obj = (id)CFPreferencesCopyAppValue((CFStringRef)OVAlphanumericKeyboardLayoutKey, (CFStringRef)identifier);
    if ([obj isKindOfClass:[NSString class]]) {
        layout = [(NSString *)obj autorelease];
    }

    return layout ? layout : self.sharedAlphanumericKeyboardLayoutIdentifier;
}

- (BOOL)isTableBasedInputMethodIdentifier:(NSString *)identifier
{
    return [identifier hasPrefix:@"org.openvanilla.OVIMTableBased."];
}

- (BOOL)canInstallCustomTableBasedInputMethodWithTablePath:(NSString *)path willOverrideBuiltInTable:(BOOL *)willOverride error:(NSError **)error
{
    return NO;
}

- (void)installCustomTableBasedInputMethodWithTablePath:(NSString *)path
{
}

- (BOOL)isCustomTableBasedInputMethod:(NSString *)identifier
{
    return NO;
}

- (void)removeCustomTableBasedInputMethod:(NSString *)identifier error:(NSError *)error
{
}

#pragma mark - Properties

- (NSString *)activeInputMethodIdentifier
{
    if (!_activeInputMethod) {
        return nil;
    }

    string identifier = InputMethodConfigIdentifier(_activeInputMethod->identifier());
    return [NSString stringWithUTF8String:identifier.c_str()];
}

- (NSString *)sharedAlphanumericKeyboardLayoutIdentifier
{

    NSString *layout = [[NSUserDefaults standardUserDefaults] stringForKey:OVAlphanumericKeyboardLayoutKey];
    return layout ? layout : OVDefaultAlphanumericKeyboardLayoutIdentifier;
}

- (void)setSharedAlphanumericKeyboardLayoutIdentifier:(NSString *)sharedAlphanumericKeyboardLayoutIdentifier
{
    [[NSUserDefaults standardUserDefaults] setObject:sharedAlphanumericKeyboardLayoutIdentifier forKey:OVAlphanumericKeyboardLayoutKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

#pragma mark - Private Methods

- (void)handleLocaleChangeNotification:(NSNotification *)aNotification
{
    NSArray *tags = [NSLocale preferredLanguages];

    if ([tags count]) {
        self.cachedLocale = [tags objectAtIndex:0];
    }
}

- (BOOL)canSelectInputMethod:(NSString *)identifier
{
    if ([identifier length]) {
        OVInputMethodMap::const_iterator f = _inputMethodMap->find([identifier UTF8String]);
        if (f != _inputMethodMap->end()) {
            return YES;
        }
    }

    return NO;
}
@end
