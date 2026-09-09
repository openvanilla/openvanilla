//
// OVIMArray.h
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Based on the work of vgod
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


#ifndef OVIMArray_h
#define OVIMArray_h

#include "OpenVanilla.h"

#include <string>

class OVIMArray;

namespace OpenVanilla {
    using namespace std;

    class OVIMArrayContext;

    class OVIMArray : public OVInputMethod {
    public:
        OVIMArray(const string& tableRootPath);
        ~OVIMArray();

        virtual OVEventHandlingContext* createContext();
        virtual const string identifier() const;
        virtual const string localizedName(const string& locale);
        virtual bool initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService);
        virtual void loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);
        virtual void saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);

        // Replaces the main table (array30.cin) with a user-imported table;
        // the bundled table remains the fallback. Empty path = use built-in.
        virtual void setCustomMainTablePath(const string& path);

        // Table access for the context.
        virtual void checkTables();
        virtual OVCINDataTable* mainTable();
        virtual OVCINDataTable* shortcodeTable();
        virtual OVCINDataTable* specialTable();
        virtual OVCINDataTable* phraseTable();

        virtual bool isAutoSP() const { return m_cfgAutoSP; }
        virtual bool isForceSP() const { return m_cfgForceSP; }
        virtual void setAutoSP(bool value) { m_cfgAutoSP = value; }
        virtual void setForceSP(bool value) { m_cfgForceSP = value; }

    protected:
        void clearTables();

        bool m_tablesLoaded;
        string m_tableRootPath;
        OVCINDataTable* m_tables[4];

        bool m_cfgAutoSP;
        bool m_cfgForceSP;
        string m_customMainTablePath;
    };
};

#endif
