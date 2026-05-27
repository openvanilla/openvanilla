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
    : m_lazyInitialized(false)
    , m_tableRootPath(tableRootPath)
    , m_cfgAutoSP(true)
    , m_cfgForceSP(false)
{
    for (size_t index = 0; index < 4; index++) {
        m_tables[index] = 0;
    }
}

OpenVanilla::OVIMArray::~OVIMArray()
{
    for (size_t index = 0; index < 4; index++) {
        delete m_tables[index];
    }
}

OVEventHandlingContext* OpenVanilla::OVIMArray::createContext()
{
    if (!checkTable()) {
        return 0;
    }

    return new OpenVanilla::OVIMArrayContext(this);
}

const string OpenVanilla::OVIMArray::identifier() const
{
    return string("org.openvanilla.OVIMArray");
}

const string OpenVanilla::OVIMArray::localizedName(const string& locale)
{
    if (locale.find("zh") == 0) {
        return string("行列");
    }

    return string("Array");
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

bool OpenVanilla::OVIMArray::checkTable()
{
    if (m_tables[0]) {
        return true;
    }

    if (m_lazyInitialized) {
        return false;
    }
    m_lazyInitialized = true;

    const char* filenames[] = {
        "array30.cin",
        "array-shortcode.cin",
        "array-special.cin",
        "array-phrase.cin",
    };

    vector<string> roots;
    roots.push_back(m_tableRootPath);
    roots.push_back(OVPathHelper::PathCat(m_tableRootPath, "Array"));

    for (size_t tableIndex = 0; tableIndex < 4; tableIndex++) {
        OVCINDataTable* loadedTable = 0;
        for (vector<string>::const_iterator root = roots.begin(), end = roots.end(); root != end; ++root) {
            string tablePath = OVPathHelper::PathCat(*root, filenames[tableIndex]);
            OVCINDataTableParser parser;
            loadedTable = parser.CINDataTableFromFileName(tablePath);
            if (loadedTable) {
                break;
            }
        }

        if (!loadedTable) {
            for (size_t index = 0; index < 4; index++) {
                delete m_tables[index];
                m_tables[index] = 0;
            }
            return false;
        }
        m_tables[tableIndex] = loadedTable;
    }

    return true;
}

OVCINDataTable* OpenVanilla::OVIMArray::table(size_t index) const
{
    if (index >= 4) {
        return 0;
    }
    return m_tables[index];
}

bool OpenVanilla::OVIMArray::isAutoSP() const
{
    return m_cfgAutoSP;
}

bool OpenVanilla::OVIMArray::isForceSP() const
{
    return m_cfgForceSP;
}

void OpenVanilla::OVIMArray::setForceSP(bool value)
{
    m_cfgForceSP = value;
}
