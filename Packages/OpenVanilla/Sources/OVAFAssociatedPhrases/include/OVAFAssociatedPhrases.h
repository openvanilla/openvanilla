//
// OVAFAssociatedPhrases.h
//
// Copyright (c) 2004-2013 Lukhnos Liu (lukhnos at openvanilla dot org)
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


#ifndef OVAFAssociatedPhrases_h
#define OVAFAssociatedPhrases_h

#include "OpenVanilla.h"

namespace OpenVanilla {
    using namespace std;

    enum UseSpaceAsFirstCandidateSelectionKeyOption {
        Disabled,
        OriginalFirstKeySelectsSecondCandidate,
        SpaceAndOriginalFirstKeySelectsFirstCandidate,
    };

    class OVAFAssociatedPhrasesContext;

    class OVAFAssociatedPhrases : public OVAroundFilter {
    public:
        OVAFAssociatedPhrases(const string& tablePath);
        ~OVAFAssociatedPhrases();

        virtual OVEventHandlingContext* createContext();
        virtual const string identifier() const;
        virtual bool initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService);
        virtual void loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);
        virtual void saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);

    private:
        string m_tablePath;
        OVFileTimestamp m_tableTimestamp;
        string m_shiftKeySymbol;

    protected:
        friend class OVAFAssociatedPhrasesContext;
        virtual void checkTable();
        OVCINDataTable *m_table;
        bool m_continuousAssociation; // Use the output of this filter as the next input.
        string m_selectionKeys; // The actual selection keys.
        UseSpaceAsFirstCandidateSelectionKeyOption m_configUseSpaceAsFirstCandidateSelectionKey;
        bool m_configSendFirstCandidateWithSpaceWithOnePageList;

        vector<pair<OVKey, string>> getSelectionKeyLabelPairs(OVLoaderService* loaderService);
    };
};

#endif
