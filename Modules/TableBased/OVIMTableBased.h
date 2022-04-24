//
// OVIMTableBased.h
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


#ifndef OVIMTableBased_h
#define OVIMTableBased_h

#include "OpenVanilla.h"

namespace OpenVanilla {
    using namespace std;

    class OVIMTableBasedContext;

    class OVIMTableBased : public OVInputMethod {
    public:
        OVIMTableBased(const string& tablePath);
        ~OVIMTableBased();

        virtual OVEventHandlingContext* createContext();
        virtual const string identifier() const;
        virtual const string localizedName(const string& locale);
        virtual bool initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService);
        virtual void loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);
        virtual void saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);

    private:
        bool fetchTableProperty(const string& key, string& outValue);

        string m_tablePath;
        OVFileTimestamp m_tableTimestamp;
        map<string, string> m_preloadedTableProperties;

    protected:
        friend class OVIMTableBasedContext;
        virtual void checkTable();

        OVCINDataTable *m_table;

        bool m_configClearReadingBufferAtCompositionError;
        bool m_configComposeWhileTyping;
        char m_configMatchOneChar;
        char m_configMatchZeroOrMoreChar;
        size_t m_configMaximumRadicalLength;
        bool m_configSendFirstCandidateWithSpaceWithOnePageList;
        bool m_configShouldComposeAtMaximumRadicalLength;
        bool m_configUseSpaceAsFirstCandidateSelectionKey;
        bool m_configOnlyUseNumPadNumbersForRadicals;
    };
};

#endif
