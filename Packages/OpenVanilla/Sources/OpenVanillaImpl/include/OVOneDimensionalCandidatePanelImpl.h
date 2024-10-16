//
// OVOneDimensionalCandidatePanelImpl.h
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

#import "OVCandidateService.h"
#import "OVCandidateListImpl.h"

//@import CandidateUI;

extern NSString *const OVOneDimensionalCandidatePanelImplDidSelectCandidateNotification;
extern NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateIndexKey;
extern NSString *const OVOneDimensionalCandidatePanelImplSelectedCandidateStringKey;

@class VTCandidateController;
@protocol VTCandidateControllerDelegate;

namespace OpenVanilla {

    class OVOneDimensionalCandidatePanelImpl : public OVOneDimensionalCandidatePanel {
    public:
        OVOneDimensionalCandidatePanelImpl(Class panelClass, OVLoaderService* loaderService);
        ~OVOneDimensionalCandidatePanelImpl();
        
        virtual void hide();
        virtual void show();
        virtual void updateDisplay();
        virtual bool isVisible();        
        virtual void setPrompt(const string& prompt);
        virtual string prompt();
        virtual bool yieldToCandidateEventHandler();
        virtual void cancelEventHandler();
        virtual void reset();

        virtual bool isHorizontal() const;
        virtual bool isVertical() const;
    
        virtual OVCandidateList* candidateList();
    
        virtual size_t candidatesPerPage() const;
        virtual void setCandidatesPerPage(size_t number);
        virtual size_t pageCount() const;
        virtual size_t currentPage() const;
        virtual size_t currentPageCandidateCount() const;
        virtual bool allowsPageWrapping() const;
        virtual void setAllowsPageWrapping(bool allowsPageWrapping);

        virtual size_t currentHightlightIndex() const;
        virtual void setHighlightIndex(size_t index);
        virtual size_t currentHightlightIndexInCandidateList() const;

        virtual size_t goToNextPage();
        virtual size_t goToPreviousPage();
        virtual size_t goToPage(size_t page);

        virtual const OVKey candidateKeyAtIndex(size_t index);
        virtual void setCandidateKeys(const OVKeyVector& keys);        
        virtual void setNextPageKeys(const OVKeyVector& keys);
        virtual void setPreviousPageKeys(const OVKeyVector& keys);
        virtual void setNextCandidateKeys(const OVKeyVector& keys);
        virtual void setPreviousCandidateKeys(const OVKeyVector& keys);
        virtual void setCancelKeys(const OVKeyVector& keys);
        virtual void setChooseHighlightedCandidateKeys(const OVKeyVector& keys);
    
        virtual const OVKeyVector defaultCandidateKeys() const;
        virtual const OVKeyVector defaultNextPageKeys() const;
        virtual const OVKeyVector defaultNextCandidateKeys() const;
        virtual const OVKeyVector defaultPreviousPageKeys() const;
        virtual const OVKeyVector defaultPreviousCandidateKeys() const;
        virtual const OVKeyVector defaultCancelKeys() const;
        virtual const OVKeyVector defaultChooseHighlightedCandidateKeys() const;
        
        enum KeyHandlerResult {
            Handled,
            CandidateSelected,
            Canceled,
            NonCandidatePanelKeyReceived,
            Invalid
        };

        virtual bool isInControl() const;        
        virtual KeyHandlerResult handleKey(OVKey* key);
        virtual void setPanelOrigin(NSPoint origin, CGFloat adjustmentHeight);
        virtual void updateVisibility();
        virtual void applyFontSettings(NSString *fontName = nil, NSUInteger fontSize = 0);

        virtual void setCandidateKeysAndLabels(const vector<pair<OVKey, string>>& keyLabelPairs);

    protected:
        OVCandidateListImpl m_candidateList;
        bool m_inControl;
        bool m_visible;
        VTCandidateController *m_candidateController;
        NSObject<VTCandidateControllerDelegate> *m_candidateDelegate;

        OVKeyVector m_candidateKeys;
        OVKeyVector m_cancelKeys;
        OVKeyVector m_chooseHighlightedCandidateKeys;
        OVKeyVector m_nextPageKeys;
        OVKeyVector m_nextCandidateKeys;
        OVKeyVector m_previousPageKeys;
        OVKeyVector m_previousCandidateKeys;

        OVKeyVector m_defaultCandidateKeys;
        OVKeyVector m_defaultCancelKeys;
        OVKeyVector m_defaultChooseHighlightedCandidateKeys;
        OVKeyVector m_defaultNextPageKeys;
        OVKeyVector m_defaultPreviousPageKeys;
        OVKeyVector m_defaultNextCandidateKeys;
        OVKeyVector m_defaultPreviousCandidateKeys;
        OVKeyVector m_spaceKeys;
    };
};
