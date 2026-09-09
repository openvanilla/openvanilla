//
// OVIMArrayContext.h
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

#ifndef OVIMArrayContext_h
#define OVIMArrayContext_h

#include "OpenVanilla.h"
#include "OVIMArray.h"

#include <string>
#include <vector>

namespace OpenVanilla {
    using namespace std;

    class OVIMArray;

    // The Array (行列30) input method context. This is a modern-API
    // implementation of the behavior originally shipped in the legacy
    // ::OVIMArrayContext engine; it does not use the legacy framework at all.
    class OVIMArrayContext : public OVEventHandlingContext {
    public:
        OVIMArrayContext(OVIMArray* module);
        virtual ~OVIMArrayContext();
        virtual void startSession(OVLoaderService* loaderService);
        virtual void stopSession(OVLoaderService* loaderService);
        virtual bool handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);

        bool isComposing();

    protected:
        enum State {
            WaitKey1 = 0,
            WaitKey2,
            WaitKey3,
            WaitCandidate
        };

        enum TableIndex {
            MainTable = 0,
            ShortcodeTable = 1,
            SpecialTable = 2,
            PhraseTable = 3
        };

        static const int RetPass = 0;
        static const int RetDone = 1;
        static const int RetContinue = 2;

        virtual void changeState(State s);
        virtual void changeBackState();
        virtual void updateDisplay(OVTextBuffer* composingText);
        virtual void dispatchStateHandler(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);

        virtual int waitKey1(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual int waitKey2(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual int waitKey3(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual int waitCandidate(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);

        virtual void updateCandidate(OpenVanilla::OVCINDataTable* table, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void showSymbolCandidates(OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void commitKeySeq(int tableIndex, const string& errorMessage, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void sendAndReset(const string& text, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void clearAll(OVTextBuffer* composingText, OVCandidateService* candidateService);
        virtual void clearCandidate(OVCandidateService* candidateService);

        // Symbol-table prefixes: "w" is the original symbol menu, "hg" is the
        // second menu added by the v2026 Array30 table. A digit typed right
        // after either of them picks a symbol group instead of selecting a
        // candidate.
        virtual bool isSymbolPrefix(const string& seq) const;
        // True when the whole sequence is a complete symbol code, e.g. "w1",
        // "hg1".
        virtual bool isSymbolSeq(const string& seq) const;
        virtual bool isForceSPSeq() const;

        virtual bool isKeynameKey(char c) const;
        virtual string composedReading() const;
        virtual string queryKey() const;
        virtual string queryKeyName(const string& keys) const;
        virtual bool hasWildcard() const;
        virtual bool hasOnlyWildcard() const;

        // Candidate window paging state (the panel is used as a display
        // surface only; this context handles all keys itself).
        virtual void showCandidatePage(OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void pageDown(OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual void pageUp(OVCandidateService* candidateService, OVLoaderService* loaderService);
        virtual bool selectCandidateBySelKey(char key, string& outText);
        virtual bool candidateListHasOnlyOnePage() const;

        OVIMArray* m_module;
        vector<string> m_components;      // raw key codes, e.g. {"h","g","2"}
        State m_state;
        vector<string> m_candidates;     // values of the current candidate list
        size_t m_candidatePage;
        string m_selKeys;
        bool m_candidatesOnDuty;
    };
};

#endif
