//
// OVIMArray.cpp
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

#include "OVIMArray.h"
#include "OVIMArrayContext.h"

using namespace OpenVanilla;

OpenVanilla::OVIMArray::OVIMArray(const string& tableRootPath)
    : m_tablesLoaded(false)
    , m_tableRootPath(tableRootPath)
    , m_cfgAutoSP(true)
    , m_cfgForceSP(false)
{
    for (size_t i = 0; i < 4; i++) {
        m_tables[i] = 0;
    }
}

OpenVanilla::OVIMArray::~OVIMArray()
{
    clearTables();
}

void OpenVanilla::OVIMArray::clearTables()
{
    for (size_t i = 0; i < 4; i++) {
        if (m_tables[i]) {
            delete m_tables[i];
            m_tables[i] = 0;
        }
    }
    m_tablesLoaded = false;
}

OVEventHandlingContext* OpenVanilla::OVIMArray::createContext()
{
    if (!m_tablesLoaded) {
        checkTables();
    }

    if (!m_tables[0]) {
        return 0;
    }

    return new OVIMArrayContext(this);
}

const string OpenVanilla::OVIMArray::identifier() const
{
    return string("org.openvanilla.OVIMArray");
}

const string OpenVanilla::OVIMArray::localizedName(const string& locale)
{
    if (locale.find("zh") == 0) {
        return string("行列30 (香草)");
    }

    return string("Array30 (OpenVanilla)");
}

bool OpenVanilla::OVIMArray::initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService)
{
    return true;
}

void OpenVanilla::OVIMArray::loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    if (moduleConfig->hasKey("SpecialCodePrompt")) {
        m_cfgAutoSP = moduleConfig->isKeyTrue("SpecialCodePrompt");
    }

    if (moduleConfig->hasKey("QuickMode")) {
        m_cfgForceSP = moduleConfig->isKeyTrue("QuickMode");
    }
}

void OpenVanilla::OVIMArray::saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    moduleConfig->setKeyBoolValue("SpecialCodePrompt", m_cfgAutoSP);
    moduleConfig->setKeyBoolValue("QuickMode", m_cfgForceSP);

    if (!moduleConfig->hasKey("AlphanumericKeyboardLayout")) {
        moduleConfig->setKeyStringValue("AlphanumericKeyboardLayout", "com.apple.keylayout.US");
    }
}

void OpenVanilla::OVIMArray::setCustomMainTablePath(const string& path)
{
    if (m_customMainTablePath == path) {
        return;
    }

    m_customMainTablePath = path;
    clearTables();
}

void OpenVanilla::OVIMArray::checkTables()
{
    if (m_tablesLoaded) {
        return;
    }

    static const char* tableFiles[4] = {
        "array30.cin",
        "array-shortcode.cin",
        "array-special.cin",
        "array-phrase.cin",
    };

    string arrayDir = m_tableRootPath + string(1, OVPathHelper::Separator()) + "Array" + string(1, OVPathHelper::Separator());

    OpenVanilla::OVCINDataTableParser parser;

    // The main table can be replaced by a user-imported table (e.g. a newer
    // Array30 release with the hg symbol groups). Fall back to the bundled
    // table when the custom one is missing or fails to load.
    if (m_customMainTablePath.length()) {
        m_tables[0] = parser.CINDataTableFromFileName(m_customMainTablePath);
        // A parseable file without any chardef is useless as a main table;
        // treat it as a load failure so a stray file cannot brick the module.
        if (m_tables[0] && m_tables[0]->chardefMap()->size() == 0) {
            delete m_tables[0];
            m_tables[0] = 0;
        }
    }
    if (!m_tables[0]) {
        m_tables[0] = parser.CINDataTableFromFileName(arrayDir + tableFiles[0]);
    }

    for (size_t i = 1; i < 4; i++) {
        m_tables[i] = parser.CINDataTableFromFileName(arrayDir + tableFiles[i]);
    }

    m_tablesLoaded = true;
}

OpenVanilla::OVCINDataTable* OpenVanilla::OVIMArray::mainTable()
{
    return m_tables[0];
}

OpenVanilla::OVCINDataTable* OpenVanilla::OVIMArray::shortcodeTable()
{
    return m_tables[1];
}

OpenVanilla::OVCINDataTable* OpenVanilla::OVIMArray::specialTable()
{
    return m_tables[2];
}

OpenVanilla::OVCINDataTable* OpenVanilla::OVIMArray::phraseTable()
{
    return m_tables[3];
}
