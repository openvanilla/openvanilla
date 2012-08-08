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

NSString *const OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification = @"OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification";
NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey = @"OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey";
NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey = @"OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey";

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
                          [NSNumber numberWithUnsignedInteger:(NSUInteger)index], OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey,
                          nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification object:controller userInfo:dict];
}
@end


OVOneDimensionalCandidatePanelImpl::OVOneDimensionalCandidatePanelImpl(Class panelClass, OVLoaderService* loaderService)
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
}

OVOneDimensionalCandidatePanelImpl::~OVOneDimensionalCandidatePanelImpl()
{
    [m_candidateController release];
    [m_candidateDelegate release];
}

void OVOneDimensionalCandidatePanelImpl::hide()
{
    m_candidateController.visible = NO;
}

void OVOneDimensionalCandidatePanelImpl::show()
{
    m_candidateController.visible = YES;
}

void OVOneDimensionalCandidatePanelImpl::updateDisplay()
{
    [m_candidateController reloadData];
}

bool OVOneDimensionalCandidatePanelImpl::isVisible()
{
    return m_candidateController.visible;
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
    return m_inControl;
}

void OVOneDimensionalCandidatePanelImpl::cancelEventHandler()
{
    m_inControl = false;
}

void OVOneDimensionalCandidatePanelImpl::reset()
{
    m_inControl = false;
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
    return cpp ? (currentHightlightIndexInCandidateList() % cpp) : 0;
}

void OVOneDimensionalCandidatePanelImpl::setHighlightIndex(size_t index)
{
    size_t cc = m_candidateList.size();
    if (!cc) {
        return;
    }
    
    size_t newIndex = currentHightlightIndexInCandidateList() - currentHightlightIndex() + index;
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
