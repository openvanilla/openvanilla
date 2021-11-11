//
// OVOneDimensionalCandidatePanelImpl.mm
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

#import "OVOneDimensionalCandidatePanelImpl.h"
#import "VTHorizontalCandidateController.h"
#import "VTVerticalCandidateController.h"

using namespace OpenVanilla;

static const NSInteger kDefaultCandidateListTextSize = 16;
static const NSInteger kMinKeyLabelSize = 10;
static const NSInteger kMinCandidateListTextSize = 12;
static const NSInteger kMaxCandidateListTextSize = 196;

NSString *const OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification = @"OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification";
NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey = @"OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey";
NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey = @"OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey";

static bool IsKeyInList(const OVKey* key, OVKeyVector list, size_t* outIndex = 0)
{
    size_t index = 0;
    for (OVKeyVector::const_iterator i = list.begin(), e = list.end(); i != e; ++i, ++index) {
        // First compare *key and *i. If they are not equal, compare their keyCode() if they are both printable.
        // This is because input methods (such as the "%selkey" directive in .cin-based modules) may use ASCII
        // code to indicate that shift keys are used for candidate seleciton keys, such as ! == Shift-1,
        // @ == Shift-2, and so on. This assumption only works with US layout and requires further work for
        // other layouts.
        if (*key == *i ||
            (isprint(key->keyCode()) && isprint(i->keyCode()) && key->keyCode() == i->keyCode())) {
            if (outIndex) {
                *outIndex = index;
            }
            return true;
        }
    }
    
    return false;
}

@interface OVOneDimensionalCandidatePanelImplDelegate : NSObject <VTCandidateControllerDelegate>
{
    OVCandidateListImpl *_candidateList;
}
- (id)initWithCandidateList:(OVCandidateListImpl *)candidateList;
@end

@implementation OVOneDimensionalCandidatePanelImplDelegate
- (id)initWithCandidateList:(OVCandidateListImpl *)candidateList
{
    self = [super init];
    if (self) {
        _candidateList = candidateList;
    }
    return self;
}

- (NSUInteger)candidateCountForController:(VTCandidateController *)controller
{
    return (NSUInteger)_candidateList->size();
}

- (NSString *)candidateController:(VTCandidateController *)controller candidateAtIndex:(NSUInteger)index
{
    return _candidateList->candidateAtIndex(index);
}

- (void)candidateController:(VTCandidateController *)controller didSelectCandidateAtIndex:(NSUInteger)index
{
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
            _candidateList->candidateAtIndex(index), OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey,
            @((NSUInteger)index), OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey,
                    nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification object:controller userInfo:dict];
}
@end


OVOneDimensionalCandidatePanelImpl::OVOneDimensionalCandidatePanelImpl(Class panelClass, OVLoaderService* loaderService)
    : m_inControl(false)
    , m_visible(false)
{
    m_candidateController = [[panelClass alloc] init];
    m_candidateDelegate = [[OVOneDimensionalCandidatePanelImplDelegate alloc] initWithCandidateList:&m_candidateList];
    m_candidateController.delegate = m_candidateDelegate;
    
    string defaultCandidates = "1234567890";
    for (size_t index = 0; index < defaultCandidates.length(); index++) {
        m_defaultCandidateKeys.push_back(loaderService->makeOVKey(defaultCandidates[index]));
    }
    
    m_defaultCancelKeys.push_back(loaderService->makeOVKey(OVKeyCode::Esc));
    m_defaultChooseHighlightedCandidateKeys.push_back(loaderService->makeOVKey(OVKeyCode::Return));
    
    if ([m_candidateController isKindOfClass:[VTHorizontalCandidateController class]]) {
        m_defaultNextCandidateKeys.push_back(loaderService->makeOVKey(OVKeyCode::Right));
        m_defaultPreviousCandidateKeys.push_back(loaderService->makeOVKey(OVKeyCode::Left));
        m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::PageDown));
        m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Down));
        m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Space));
        m_defaultPreviousPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::PageUp));
        m_defaultPreviousPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Up));        
    }
    else {
        m_defaultNextCandidateKeys.push_back(loaderService->makeOVKey(OVKeyCode::Down));
        m_defaultPreviousCandidateKeys.push_back(loaderService->makeOVKey(OVKeyCode::Up));
        m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::PageDown));
        m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Space));
		m_defaultNextPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Right));
        m_defaultPreviousPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::PageUp));
        m_defaultPreviousPageKeys.push_back(loaderService->makeOVKey(OVKeyCode::Left));        
    }
    
    m_candidateKeys = m_defaultCandidateKeys;
    m_cancelKeys = m_defaultCancelKeys;
    m_chooseHighlightedCandidateKeys = m_defaultChooseHighlightedCandidateKeys;
    m_nextCandidateKeys = m_defaultNextCandidateKeys;
    m_nextPageKeys = m_defaultNextPageKeys;
    m_previousCandidateKeys = m_defaultPreviousCandidateKeys;
    m_previousPageKeys = m_defaultPreviousPageKeys;

    applyFontSettings();
}

OVOneDimensionalCandidatePanelImpl::~OVOneDimensionalCandidatePanelImpl()
{
//    [m_candidateController release];
//    [m_candidateDelegate release];
}

void OVOneDimensionalCandidatePanelImpl::hide()
{
    m_visible = false;
}

void OVOneDimensionalCandidatePanelImpl::show()
{
    m_visible = true;
}

void OVOneDimensionalCandidatePanelImpl::updateDisplay()
{
    NSUInteger selectedIndex = m_candidateController.selectedCandidateIndex;
    [m_candidateController reloadData];
    m_candidateController.selectedCandidateIndex = selectedIndex;
}

bool OVOneDimensionalCandidatePanelImpl::isVisible()
{
    return m_visible;
}

void OVOneDimensionalCandidatePanelImpl::setPrompt(const string& prompt)
{   
}

string OVOneDimensionalCandidatePanelImpl::prompt()
{
    return string();
}

bool OVOneDimensionalCandidatePanelImpl::yieldToCandidateEventHandler()
{
    m_inControl = true;

    if (m_candidateList.size()) {
        m_candidateController.selectedCandidateIndex = 0;
    }

    return m_inControl;
}

void OVOneDimensionalCandidatePanelImpl::cancelEventHandler()
{
    m_inControl = false;
}

void OVOneDimensionalCandidatePanelImpl::reset()
{
    m_inControl = false;
    m_visible = false;
    m_candidateList.clear();
    m_candidateController.visible = NO;
    [m_candidateController reloadData];
}

bool OVOneDimensionalCandidatePanelImpl::isHorizontal() const
{
    return [m_candidateController isKindOfClass:[VTHorizontalCandidateController class]];
}

bool OVOneDimensionalCandidatePanelImpl::isVertical() const
{
    return [m_candidateController isKindOfClass:[VTVerticalCandidateController class]];   
}

OVCandidateList* OVOneDimensionalCandidatePanelImpl::candidateList()
{
    return &m_candidateList;
}

size_t OVOneDimensionalCandidatePanelImpl::candidatesPerPage() const
{
    return (size_t)[m_candidateController.keyLabels count];
}

void OVOneDimensionalCandidatePanelImpl::setCandidatesPerPage(size_t number)
{    
    if (!number || number > candidatesPerPage()) {
        return;
    }
    
    m_candidateController.keyLabels = [m_candidateController.keyLabels subarrayWithRange:NSMakeRange(0, (NSUInteger)number)];
}

size_t OVOneDimensionalCandidatePanelImpl::pageCount() const
{
    size_t cpp = candidatesPerPage();
    if (!cpp) {
        return 0;
    }
    
    size_t cc = m_candidateList.size();
    size_t p = cc / cpp;
    return (cc % cpp) ? (p + 1) : p;
}

size_t OVOneDimensionalCandidatePanelImpl::currentPage() const
{
    return currentHightlightIndexInCandidateList() / candidatesPerPage();
}

size_t OVOneDimensionalCandidatePanelImpl::currentPageCandidateCount() const
{
    size_t cc = m_candidateList.size();
    size_t cpp = candidatesPerPage();
    return cc >= cpp ? cpp : cc;
}

bool OVOneDimensionalCandidatePanelImpl::allowsPageWrapping() const
{
    return false;
}

void OVOneDimensionalCandidatePanelImpl::setAllowsPageWrapping(bool allowsPageWrapping)
{
}

size_t OVOneDimensionalCandidatePanelImpl::currentHightlightIndex() const
{
    size_t cpp = candidatesPerPage();
    if (!cpp) {
        return 0;
    }

    return (size_t)(m_candidateController.selectedCandidateIndex - [m_candidateController candidateIndexAtKeyLabelIndex:0]);
}

void OVOneDimensionalCandidatePanelImpl::setHighlightIndex(size_t index)
{
    size_t cc = m_candidateList.size();
    if (!cc) {
        return;
    }
    
    size_t newIndex = (size_t)[m_candidateController candidateIndexAtKeyLabelIndex:0] + index;
    if (newIndex >= cc) {
        newIndex = cc - 1;
    }
    
    m_candidateController.selectedCandidateIndex = (NSUInteger)newIndex;
}

size_t OVOneDimensionalCandidatePanelImpl::currentHightlightIndexInCandidateList() const
{
    return (size_t)m_candidateController.selectedCandidateIndex;    
}

size_t OVOneDimensionalCandidatePanelImpl::goToNextPage()
{
    size_t cc = m_candidateList.size();
    if (!cc) {
        return 0;
    }
    
    size_t newIndex = currentHightlightIndexInCandidateList() + candidatesPerPage();
    if (newIndex >= cc) {
        newIndex = cc - 1;
    }

    m_candidateController.selectedCandidateIndex = (NSUInteger)newIndex;
    return currentPage();
}

size_t OVOneDimensionalCandidatePanelImpl::goToPreviousPage()
{
    if (!pageCount()) {
        return 0;
    }

    size_t cc = m_candidateList.size();
    if (!cc) {
        return 0;
    }
    
    size_t newIndex = currentHightlightIndexInCandidateList();
    
    if (newIndex < candidatesPerPage()) {
        newIndex = 0;
    }
    else {
        newIndex -= candidatesPerPage();
    }

    m_candidateController.selectedCandidateIndex = (NSUInteger)newIndex;
    return currentPage();    
}

size_t OVOneDimensionalCandidatePanelImpl::goToPage(size_t page)
{
    if (page >= pageCount()) {
        return currentPage();
    }
    
    m_candidateController.selectedCandidateIndex = (NSUInteger)(page * candidatesPerPage());
    return page;
}

const OVKey OVOneDimensionalCandidatePanelImpl::candidateKeyAtIndex(size_t index)
{
    NSCAssert(index < m_candidateKeys.size(), @"Index must be within bound");
    return m_candidateKeys[index];
}

void OVOneDimensionalCandidatePanelImpl::setCandidateKeys(const OVKeyVector& keys)
{
    m_candidateKeys = keys;
    NSMutableArray *labels = [NSMutableArray array];
    for (OVKeyVector::const_iterator i = keys.begin(), e = keys.end(); i != e; ++i) {
        [labels addObject:[NSString stringWithUTF8String:(*i).receivedString().c_str()]];
    }

    m_candidateController.keyLabels = labels;
}

void OVOneDimensionalCandidatePanelImpl::setNextPageKeys(const OVKeyVector& keys)
{
    m_nextPageKeys = keys;
}

void OVOneDimensionalCandidatePanelImpl::setPreviousPageKeys(const OVKeyVector& keys)
{
    m_previousPageKeys = keys;
}

void OVOneDimensionalCandidatePanelImpl::setNextCandidateKeys(const OVKeyVector& keys)
{
    m_nextCandidateKeys = keys;
}

void OVOneDimensionalCandidatePanelImpl::setPreviousCandidateKeys(const OVKeyVector& keys)
{
    m_previousCandidateKeys = keys;
}

void OVOneDimensionalCandidatePanelImpl::setCancelKeys(const OVKeyVector& keys)
{
    m_cancelKeys = keys;
}

void OVOneDimensionalCandidatePanelImpl::setChooseHighlightedCandidateKeys(const OVKeyVector& keys)
{
    m_chooseHighlightedCandidateKeys = keys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultCandidateKeys() const
{
    return m_defaultCandidateKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultNextPageKeys() const
{
    return m_defaultNextPageKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultNextCandidateKeys() const
{
    return m_defaultNextCandidateKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultPreviousPageKeys() const
{
    return m_defaultPreviousPageKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultPreviousCandidateKeys() const
{
    return m_defaultPreviousCandidateKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultCancelKeys() const
{
    return m_defaultCancelKeys;
}

const OVKeyVector OVOneDimensionalCandidatePanelImpl::defaultChooseHighlightedCandidateKeys() const
{
    return m_defaultChooseHighlightedCandidateKeys;
}

bool OVOneDimensionalCandidatePanelImpl::isInControl() const
{
    return m_inControl;
}

OVOneDimensionalCandidatePanelImpl::KeyHandlerResult OVOneDimensionalCandidatePanelImpl::handleKey(OVKey* key)
{
    size_t selectedCandidateKeyIndex;

    if (IsKeyInList(key, m_candidateKeys, &selectedCandidateKeyIndex)) {
        if (selectedCandidateKeyIndex >= currentPageCandidateCount()) {
            return Invalid;
        }

        hide();
        m_inControl = false;
        setHighlightIndex(selectedCandidateKeyIndex);
        return CandidateSelected;
    }
    else if (IsKeyInList(key, m_chooseHighlightedCandidateKeys)) {
        hide();
        m_inControl = false;
        return CandidateSelected;
    }
    else if (IsKeyInList(key, m_cancelKeys)) {
        hide();
        m_inControl = false;
        return Canceled;
    }
    else if (IsKeyInList(key, m_nextPageKeys)) {
        size_t index = currentHightlightIndexInCandidateList();
        size_t lastIndex = m_candidateList.size();
        if (lastIndex) {
            lastIndex--;
        }
        size_t clampedIndex = min(index + candidatesPerPage(), lastIndex);
        if (index == clampedIndex) {
            return Invalid;
        }
        
        m_candidateController.selectedCandidateIndex = (NSUInteger)clampedIndex;
        return Handled;
    }
    else if (IsKeyInList(key, m_nextCandidateKeys)) {
        size_t index = currentHightlightIndexInCandidateList();
        size_t lastIndex = m_candidateList.size();
        if (lastIndex) {
            lastIndex--;
        }
        size_t clampedIndex = min(index + 1, lastIndex);
        m_candidateController.selectedCandidateIndex = (NSUInteger)clampedIndex;
        return Handled;        
    }
    else if (IsKeyInList(key, m_previousPageKeys)) {
        size_t index = currentHightlightIndexInCandidateList();
        if (!index) {
            return Invalid;
        }
        
        size_t cpp = candidatesPerPage();
        if (index > cpp) {
            index -= cpp;
        }
        else {
            index = 0;            
        }
        
        m_candidateController.selectedCandidateIndex = (NSUInteger)index;
        return Handled;
    }
    else if (IsKeyInList(key, m_previousCandidateKeys)) {
        size_t index = currentHightlightIndexInCandidateList();
        if (!index) {
            return Invalid;
        }
        
        index--;
        m_candidateController.selectedCandidateIndex = (NSUInteger)index;
        return Handled;
    }
    
    return NonCandidatePanelKeyReceived;
}

void OVOneDimensionalCandidatePanelImpl::setPanelOrigin(NSPoint origin, CGFloat adjustmentHeight)
{
    [m_candidateController setWindowTopLeftPoint:origin bottomOutOfScreenAdjustmentHeight:adjustmentHeight];
}

void OVOneDimensionalCandidatePanelImpl::updateVisibility()
{
    if (m_candidateController.visible != m_visible) {
        m_candidateController.visible = m_visible;
    }
}

void OVOneDimensionalCandidatePanelImpl::applyFontSettings(NSString *fontName, NSUInteger fontSize)
{
    if (fontSize == 0) {
        fontSize = kDefaultCandidateListTextSize;
    }

    if (fontSize < kMinCandidateListTextSize) {
        fontSize = kMinCandidateListTextSize;
    }

    if (fontSize > kMaxCandidateListTextSize) {
        fontSize = kMaxCandidateListTextSize;
    }

    NSInteger keyLabelSize = fontSize / 2;
    if (keyLabelSize < kMinKeyLabelSize) {
        keyLabelSize = kMinKeyLabelSize;
    }

    m_candidateController.keyLabelFont = fontName ? [NSFont fontWithName:fontName size:keyLabelSize] : [NSFont systemFontOfSize:keyLabelSize];
    m_candidateController.candidateFont = fontName ? [NSFont fontWithName:fontName size:fontSize] : [NSFont systemFontOfSize:fontSize];
}

void OVOneDimensionalCandidatePanelImpl::setCandidateKeysAndLabels(const vector<pair<OVKey, string>>& keyLabelPairs)
{
    vector<OVKey> keys;
    NSMutableArray *labels = [NSMutableArray array];
    for (const auto& keyLabelPair : keyLabelPairs) {
        keys.push_back(keyLabelPair.first);
        [labels addObject:[NSString stringWithUTF8String:keyLabelPair.second.c_str()]];
    }

    m_candidateKeys = keys;
    m_candidateController.keyLabels = labels;
}
