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

OVIMArray::OVIMArray(const string& tableRootPath)
    : m_mainTable(0)
    , m_shortCodeTable(0)
    , m_specialCodeTable(0)
    , m_cfgAutoSP(true)
    , m_cfgForceSP(false)
{
    m_mainTablePath = OVPathHelper::PathCat(tableRootPath, "array30.cin");
    m_shortCodeTablePath = OVPathHelper::PathCat(tableRootPath, "array-shortcode.cin");
    m_specialCodeTablePath =OVPathHelper::PathCat(tableRootPath, "array-special.cin");
}

OVIMArray::~OVIMArray()
{
    if (m_mainTable) {
        delete m_mainTable;
    }

    if (m_shortCodeTable) {
        delete m_shortCodeTable;
    }

    if (m_specialCodeTable) {
        delete m_specialCodeTable;
    }
}

OVEventHandlingContext* OVIMArray::createContext()
{
    checkTables();
    if (!tablesUsable()) {
        return 0;
    }

    return new OVIMArrayContext(this);
}

const string OVIMArray::identifier() const
{
    return string("org.openvanilla.OVIMArray");
}

const string OVIMArray::localizedName(const string& locale)
{
    if (locale.find("zh") == 0) {
        return string("行列");
    }

    return string("Array");
}

bool OVIMArray::initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService)
{
    if (!OVPathHelper::PathExists(m_mainTablePath)) {
        return false;
    }

    if (!OVPathHelper::PathExists(m_shortCodeTablePath)) {
        return false;
    }

    if (!OVPathHelper::PathExists(m_specialCodeTablePath)) {
        return false;
    } 

    m_mainTableTimestamp = OVPathHelper::TimestampForPath(m_mainTablePath);
    m_shortCodeTableTimestamp = OVPathHelper::TimestampForPath(m_shortCodeTablePath);
    m_specialCodeTableTimestamp = OVPathHelper::TimestampForPath(m_specialCodeTablePath);

    m_preloadedMainTableProperties = OVCINDataTableParser::QuickParseProperty(m_mainTablePath);
    if (!m_preloadedMainTableProperties.size()) {
        return false;
    }

    return true;
}

void OVIMArray::loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    if (moduleConfig->hasKey("SpecialCodePrompt")) {
        m_cfgAutoSP = moduleConfig->isKeyTrue("SpecialCodePrompt");
    }

    if (moduleConfig->hasKey("QuickMode")) {
        m_cfgForceSP = moduleConfig->isKeyTrue("QuickMode");
    }        
}

void OVIMArray::saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    moduleConfig->setKeyBoolValue("SpecialCodePrompt", m_cfgAutoSP);    
    moduleConfig->setKeyBoolValue("QuickMode", m_cfgForceSP);

    if (!moduleConfig->hasKey("AlphanumericKeyboardLayout")) {
        moduleConfig->setKeyStringValue("AlphanumericKeyboardLayout", "com.apple.keylayout.US");
    }
}

bool OVIMArray::fetchTableProperty(const string& key, string& outValue)
{
    if (m_mainTable) {
        string value = m_mainTable->findProperty(key);
        if (value.length()) {
            outValue = value;
            return true;
        }
        return false;
    }

    map<string, string>::iterator f = m_preloadedMainTableProperties.find(key);
    if (f == m_preloadedMainTableProperties.end()) {
        return false;
    }

    outValue = (*f).second;
    return true;
}

void OVIMArray::checkTables()
{
    if (!OVPathHelper::PathExists(m_mainTablePath)) {
        return;
    }

    if (!OVPathHelper::PathExists(m_shortCodeTablePath)) {
        return;
    }

    if (!OVPathHelper::PathExists(m_specialCodeTablePath)) {
        return;
    } 

    OVFileTimestamp timestamp;

    timestamp = OVPathHelper::TimestampForPath(m_mainTablePath);
    if (timestamp > m_mainTableTimestamp && m_mainTable) {
        delete m_mainTable;
        m_mainTable = 0;
        m_mainTableTimestamp = timestamp;        
    }

    OVCINDataTableParser mainTableParser;
    m_mainTable = mainTableParser.CINDataTableFromFileName(m_mainTablePath);


    timestamp = OVPathHelper::TimestampForPath(m_shortCodeTablePath);
    if (timestamp > m_shortCodeTableTimestamp && m_shortCodeTable) {
        delete m_shortCodeTable;
        m_shortCodeTable = 0;
        m_shortCodeTableTimestamp = timestamp;        
    }

    OVCINDataTableParser shortCodeTableParser;
    m_shortCodeTable = shortCodeTableParser.CINDataTableFromFileName(m_shortCodeTablePath);

    timestamp = OVPathHelper::TimestampForPath(m_specialCodeTablePath);
    if (timestamp > m_specialCodeTableTimestamp && m_specialCodeTable) {
        delete m_specialCodeTable;
        m_specialCodeTable = 0;
        m_specialCodeTableTimestamp = timestamp;        
    }

    OVCINDataTableParser specialCodeTableParser;
    m_specialCodeTable = specialCodeTableParser.CINDataTableFromFileName(m_specialCodeTablePath);
}

bool OVIMArray::tablesUsable()
{
    return m_mainTable != 0 && m_shortCodeTable != 0 && m_specialCodeTable != 0;
}
