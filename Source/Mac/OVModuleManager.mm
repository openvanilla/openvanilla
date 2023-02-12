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
#import <set>
#import "OVAFAssociatedPhrasesContext.h"
#import "OVCandidateServiceImpl.h"
#import "OVConstants.h"
#import "OVIMArray.h"
#import "OVIMTableBased.h"
#import "OVIMBig5Code.h"
#import "OVLoaderServiceImpl.h"
#import "OVPlistBackedKeyValueMapImpl.h"
#import "OVToolTipWindowController.h"
#import "VXHanConvert.h"

extern NSString *const OVModuleManagerDidReloadNotification = @"OVModuleManagerDidReloadNotification";
extern NSString *const OVModuleManagerDidUpdateActiveInputMethodNotification = @"OVModuleManagerDidUpdateActiveInputMethodNotification";

using namespace OpenVanilla;

static string InputMethodConfigIdentifier(const string& identifier)
{
    if (identifier.find(".") != string::npos) {
        return identifier;
    }

    return string("org.openvanilla.module.") + identifier;
}

@interface OVModuleManager ()
- (NSString *)rootPathForCustomInputMethodTables;
- (BOOL)canSelectInputMethod:(NSString *)identifier;
- (void)handleLocaleChangeNotification:(NSNotification *)aNotification;
@property (assign) OVInputMethodMap* inputMethodMap;
@end

@implementation OVModuleManager
@dynamic activeInputMethodIdentifier;
@dynamic sharedAlphanumericKeyboardLayoutIdentifier;
@synthesize inputMethodIdentifiers = _inputMethodIdentifiers;

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
        _toolTipWindowController = [[OVToolTipWindowController alloc] init];
        _inputMethodMap = new OVInputMethodMap;
        _inputMethodIdentifiers = [[NSMutableArray alloc] init];
        _customTableBasedInputMethodIdentifierTableNameMap = [[NSMutableDictionary alloc] init];
        _currentLocale = @"en";

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
    delete _associatedPhrasesModule;

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
        OVPlistBackedKeyValueMapImpl kvmi((__bridge CFStringRef)identifier);
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
        string locale = [self.currentLocale UTF8String];
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
    [_customTableBasedInputMethodIdentifierTableNameMap removeAllObjects];

    for (OVInputMethodMap::iterator i = _inputMethodMap->begin(), e = _inputMethodMap->end(); i != e; ++i) {
        delete (*i).second;
    }

    _activeInputMethod = 0;
    _inputMethodMap->clear();

    vector<OVInputMethod*> inputMethods;
    map<OVInputMethod*, string> customTableInputMethodTableNameMap;
    NSMutableSet *customTableNames = [NSMutableSet set];

    NSString *userTableRoot = [self rootPathForCustomInputMethodTables];
    if (userTableRoot) {
        NSArray *subpaths = [[NSFileManager defaultManager] subpathsAtPath:userTableRoot];

        for (NSString *tableName in subpaths) {
            if (![[tableName pathExtension] isEqualToString:@"cin"]) {
                continue;
            }

            NSString *tablePath = [userTableRoot stringByAppendingPathComponent:tableName];
            OVInputMethod *inputMethod = new OVIMTableBased([tablePath fileSystemRepresentation]);
            inputMethods.push_back(inputMethod);
            customTableInputMethodTableNameMap[inputMethod] = string([tableName UTF8String]);
            [customTableNames addObject:tableName];
        }
    }

    NSArray *basicTables = [NSArray arrayWithObjects:
                            @"cj.cin",
                            @"simplex.cin",
                            @"dayi3.cin",
                            @"jyutping.cin",
                            @"jyutping-toneless.cin",
                            @"ehq-symbols.cin",
                            nil];
    NSString *tableRoot = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables/TableBased"];

    for (NSString *tableName in basicTables) {
        if ([customTableNames containsObject:tableName]) {
            continue;
        }

        NSString *tablePath = [tableRoot stringByAppendingPathComponent:tableName];

        OVInputMethod *inputMethod = new OVIMTableBased([tablePath fileSystemRepresentation]);
        inputMethods.push_back(inputMethod);
    }

    NSString *arrayTableRoot = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables"];
    OVInputMethod *inputMethod = new OpenVanilla::OVIMArray([arrayTableRoot UTF8String]);
    inputMethods.push_back(inputMethod);
    OVInputMethod *big5Code = new OpenVanilla::OVIMBig5Code();
    inputMethods.push_back(big5Code);

    for (vector<OVInputMethod*>::iterator i = inputMethods.begin(), e = inputMethods.end(); i != e; ++i) {
        OVInputMethod* inputMethod = *i;
        OVPathInfo info;

        // TODO: furnish path info for dynamically-loaded bundles (if supported in the future)
        bool result = inputMethod->initialize(&info, self.loaderService);
        if (!result) {
            delete inputMethod;
        }
        else {
            string identifier = InputMethodConfigIdentifier(inputMethod->identifier());
            NSString *idNSStr = [NSString stringWithUTF8String:identifier.c_str()];
            _inputMethodMap->operator[](identifier) = inputMethod;
            [_inputMethodIdentifiers addObject:idNSStr];

            map<OVInputMethod*, string>::iterator f = customTableInputMethodTableNameMap.find(inputMethod);
            if (f != customTableInputMethodTableNameMap.end()) {
                NSString *tableName = [NSString stringWithUTF8String:(*f).second.c_str()];
                [_customTableBasedInputMethodIdentifierTableNameMap setObject:tableName forKey:idNSStr];
            }
        }
    }

    OVPathInfo info;
    NSString *associatedPhraseTable = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables/AssociatedPhrases/associated-phrases.cin"];
    _associatedPhrasesModule = new OVAFAssociatedPhrases([associatedPhraseTable UTF8String]);
    bool result = _associatedPhrasesModule->initialize(&info, self.loaderService);
    if (!result) {
        delete _associatedPhrasesModule;
        _associatedPhrasesModule = 0;
    }

    [self handleLocaleChangeNotification:nil];
    [self synchronizeActiveInputMethodSettings];

    [[NSNotificationCenter defaultCenter] postNotificationName:OVModuleManagerDidReloadNotification object:self];
}

- (void)synchronizeAroundFilterSettings
{
    if (!_associatedPhrasesModule) {
        return;
    }

    NSString *identifier = [NSString stringWithUTF8String:_associatedPhrasesModule->identifier().c_str()];
    OVPlistBackedKeyValueMapImpl kvmi((__bridge CFStringRef)identifier);
    OVKeyValueMap kvm(&kvmi);
    _associatedPhrasesModule->loadConfig(&kvm, _loaderService);
    _associatedPhrasesModule->saveConfig(&kvm, _loaderService);
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

    [self synchronizeAroundFilterSettings];

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
        _candidateService->useHorizontalCandidatePanel();
    }
}

- (void)writeOutActiveInputMethodSettings
{
    if (_activeInputMethod) {
        OVPlistBackedKeyValueMapImpl kvmi((__bridge CFStringRef)self.activeInputMethodIdentifier);
        OVKeyValueMap kvm(&kvmi);
        _activeInputMethod->saveConfig(&kvm, _loaderService);
    }
}

- (NSString *)alphanumericKeyboardLayoutForInputMethod:(NSString *)identifier
{
    if (!identifier) {
        return self.sharedAlphanumericKeyboardLayoutIdentifier;
    }

    OVInputMethodMap::const_iterator f = _inputMethodMap->find([identifier UTF8String]);
    if (f == _inputMethodMap->end()) {
        return self.sharedAlphanumericKeyboardLayoutIdentifier;
    }

    NSString *layout = nil;
    id obj = (__bridge id)CFPreferencesCopyAppValue((CFStringRef)OVAlphanumericKeyboardLayoutKey, (CFStringRef)identifier);

    if (obj) {
        layout = obj;
        if (![layout isKindOfClass:[NSString class]]) {
            layout = nil;
        }
    }

    return layout ? layout : self.sharedAlphanumericKeyboardLayoutIdentifier;
}

- (BOOL)isTableBasedInputMethodIdentifier:(NSString *)identifier
{
    return [identifier hasPrefix:@"org.openvanilla.OVIMTableBased."];
}

- (BOOL)canInstallCustomTableBasedInputMethodWithTablePath:(NSString *)path willOverrideBuiltInTable:(BOOL *)willOverride identifier:(NSString **)identifierIfInstalled localizedName:(NSString **)localizedNameIfInstalled error:(NSError **)error
{
    const char *posixPath = [path fileSystemRepresentation];

    OVInputMethod *inputMethod = new OVIMTableBased(posixPath);

    // TODO: furnish path info for dynamically-loaded bundles (if supported in the future)
    OVPathInfo info;
    bool result = inputMethod->initialize(&info, self.loaderService);

    if (!result) {
        delete inputMethod;

        NSString *message = [NSString stringWithFormat:NSLocalizedString(@"Cannot use the file \"%@\": file may be corrupt or is not a valid CIN file", nil), path];;
        NSDictionary *userInfo = [NSDictionary dictionaryWithObjectsAndKeys:message, NSLocalizedDescriptionKey, nil];
        NSError *returnError = [NSError errorWithDomain:[[NSBundle mainBundle] bundleIdentifier] code:-1 userInfo:userInfo];
        if (error) {
            *error = returnError;
        }
        return NO;
    }

    string identifier = InputMethodConfigIdentifier(inputMethod->identifier());
    NSString *idNSStr = [NSString stringWithUTF8String:identifier.c_str()];
    if (willOverride) {
        if ([self.inputMethodIdentifiers containsObject:idNSStr]) {
            *willOverride = YES;
        }
    }

    if (identifierIfInstalled) {
        *identifierIfInstalled = idNSStr;
    }

    if (localizedNameIfInstalled) {
        string locale = [self.currentLocale UTF8String];
        string localizedName = inputMethod->localizedName(locale);
        *localizedNameIfInstalled = [NSString stringWithUTF8String:localizedName.c_str()];
    }

    delete inputMethod;
    return YES;
}

- (void)installCustomTableBasedInputMethodWithTablePath:(NSString *)path
{
    if (![self canInstallCustomTableBasedInputMethodWithTablePath:path willOverrideBuiltInTable:NULL identifier:NULL localizedName:NULL error:NULL]) {
        return;
    }

    NSString *userTableRoot = [self rootPathForCustomInputMethodTables];
    if (!userTableRoot) {
        return;
    }

    // TODO: Show error info
    NSString *targetPath = [userTableRoot stringByAppendingPathComponent:[path lastPathComponent]];
    NSError *error = nil;
    BOOL success = [[NSFileManager defaultManager] copyItemAtPath:path toPath:targetPath error:&error];
    if (!success) {
        NSLog(@"Cannot copy %@ to %@, error: %@", path, targetPath, error);
    }

    [self reload];
}

- (BOOL)isCustomTableBasedInputMethod:(NSString *)identifier
{
    return [_customTableBasedInputMethodIdentifierTableNameMap objectForKey:identifier] != nil;
}

- (BOOL)removeCustomTableBasedInputMethod:(NSString *)identifier error:(NSError **)error
{
    if (![self isCustomTableBasedInputMethod:identifier]) {
        // TODO: furnish error info
        return NO;
    }

    NSString *userTableRoot = [self rootPathForCustomInputMethodTables];
    if (!userTableRoot) {
        // TODO: furnish error info
        return NO;
    }

    NSString *baseTableName = [_customTableBasedInputMethodIdentifierTableNameMap objectForKey:identifier];
    NSString *path = [userTableRoot stringByAppendingPathComponent:baseTableName];
    BOOL success = [[NSFileManager defaultManager] removeItemAtPath:path error:error];
    if (success) {
        [self reload];
    }
    return success;
}

- (NSString *)filteredStringWithString:(NSString *)input
{
    if (![input length]) {
        return input;
    }

    uint16_t (*filterFunction)(uint16_t) = 0;

    if (self.traditionalToSimplifiedChineseFilterEnabled) {
        filterFunction = VXUCS2TradToSimpChinese;
    }
    else if (self.simplifiedToTraditionalChineseFilterEnabled) {
        filterFunction = VXUCS2SimpToTradChinese;
    }

    if (filterFunction) {
        NSUInteger length = [input length];
        unichar *chars = (unichar *)calloc(length, sizeof(unichar));
        if (chars) {
            [input getCharacters:chars range:NSMakeRange(0, length)];
            for (NSUInteger i = 0; i < length; i++) {
                unichar f = filterFunction(chars[i]);
                if (f) {
                    chars[i] = f;
                }
            }

            NSString *output = [[NSString alloc] initWithCharacters:chars length:length];
            free(chars);
            return output;
        }
    }

    return input;
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

- (BOOL)traditionalToSimplifiedChineseFilterEnabled
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:OVTraditionalToSimplifiedChineseFilterEnabledKey];
}

- (void)setTraditionalToSimplifiedChineseFilterEnabled:(BOOL)traditionalToSimplifiedChineseFilterEnabled
{
    [[NSUserDefaults standardUserDefaults] setBool:traditionalToSimplifiedChineseFilterEnabled forKey:OVTraditionalToSimplifiedChineseFilterEnabledKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (BOOL)simplifiedToTraditionalChineseFilterEnabled
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:OVSimplifiedToTraditionalChineseFilterEnabledKey];
}

- (void)setSimplifiedToTraditionalChineseFilterEnabled:(BOOL)simplifiedToTraditionalChineseFilterEnabled
{
    [[NSUserDefaults standardUserDefaults] setBool:simplifiedToTraditionalChineseFilterEnabled forKey:OVSimplifiedToTraditionalChineseFilterEnabledKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (BOOL)associatedPhrasesAroundFilterEnabled
{
    return [[NSUserDefaults standardUserDefaults] boolForKey:OVAssociatedPhrasesAroundFilterEnabledKey];
}
- (void)setAssociatedPhrasesAroundFilterEnabled:(BOOL)associatedPhrasesAroundFilterEnabled
{
    [[NSUserDefaults standardUserDefaults] setBool:associatedPhrasesAroundFilterEnabled forKey:OVAssociatedPhrasesAroundFilterEnabledKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

#pragma mark - Private Methods

- (NSString *)rootPathForCustomInputMethodTables
{
    NSArray *appSupportPaths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    if (![appSupportPaths count]) {
        return nil;
    }

    NSString *appSupportRoot = [appSupportPaths objectAtIndex:0];

    NSString *appDataRoot = [appSupportRoot stringByAppendingPathComponent:[[[NSBundle mainBundle] infoDictionary] objectForKey:(id)kCFBundleNameKey]];
    NSString *tableRoot = [appDataRoot stringByAppendingPathComponent:@"UserData/TableBased"];

    BOOL isDir = NO;
    BOOL pathExists = [[NSFileManager defaultManager] fileExistsAtPath:tableRoot isDirectory:&isDir];

    if (!pathExists) {
        [[NSFileManager defaultManager] createDirectoryAtPath:tableRoot withIntermediateDirectories:YES attributes:nil error:NULL];
        pathExists = [[NSFileManager defaultManager] fileExistsAtPath:tableRoot isDirectory:&isDir];
    }

    if (pathExists && isDir) {
        return tableRoot;
    }

    return nil;
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

- (void)handleLocaleChangeNotification:(NSNotification *)aNotification
{
    NSArray *tags = [NSLocale preferredLanguages];

    if ([tags count]) {
        self.currentLocale = [tags objectAtIndex:0];
    }
}
@end
