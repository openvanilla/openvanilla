//
// OVAFAssociatedPhrases.cpp
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

#include "OVAFAssociatedPhrases.h"
#include "OVAFAssociatedPhrasesContext.h"
#include <fstream>
#include <iterator>
#include <memory>

using namespace OpenVanilla;

static const char *const kDefaultSelectionKeys = "!@#$%^&*()";
static const char *const kDefaultUnshiftedSelectionKeyLabels = "1234567890";
static const char *const kDefaultShiftKeySymbol = "⇧";

OVAFAssociatedPhrases::OVAFAssociatedPhrases(const string& tablePath, const string& customTablePath)
    : m_tablePath(tablePath)
    , m_customTablePath(customTablePath)
    , m_table(0)
    , m_selectionKeys(kDefaultSelectionKeys)
    , m_shiftKeySymbol(kDefaultShiftKeySymbol)
    , m_continuousAssociation(true)
{
}

OVAFAssociatedPhrases::~OVAFAssociatedPhrases()
{
    if (m_table) {
        delete m_table;
    }
}

OVEventHandlingContext* OVAFAssociatedPhrases::createContext()
{
    return new OVAFAssociatedPhrasesContext(this);
}

const string OVAFAssociatedPhrases::identifier() const
{
    return string("org.openvanilla.OVAFAssociatedPhrases");
}

bool OVAFAssociatedPhrases::initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService)
{
    checkTable();
    // Keep the module available so importing a valid table can recover it.
    return true;
}

void OVAFAssociatedPhrases::loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    m_continuousAssociation = true;
    if (moduleConfig->hasKey("ContinuousAssociation")) {
        m_continuousAssociation = moduleConfig->isKeyTrue("ContinuousAssociation");
    }

    m_selectionKeys = kDefaultSelectionKeys;
    if (moduleConfig->hasKey("SelectionKeys")) {
        string selKeys = moduleConfig->stringValueForKey("SelectionKeys");

        // Only accept sequences whose length is equal to or more than 4 chars.
        if (selKeys.length() >= 4) {
            m_selectionKeys = selKeys;
        }
    }

    m_shiftKeySymbol = kDefaultShiftKeySymbol;
    if (moduleConfig->hasKey("ShiftKeySymbol")) {
        string symbol = moduleConfig->stringValueForKey("ShiftKeySymbol");
        if (!symbol.empty()) {
            m_shiftKeySymbol = symbol;
        }
    }
    if (moduleConfig->hasKey("UseSpaceAsFirstCandidateSelectionKey")) {
        m_configUseSpaceAsFirstCandidateSelectionKey = static_cast<UseSpaceAsFirstCandidateSelectionKeyOption>(moduleConfig->intValueForKey("UseSpaceAsFirstCandidateSelectionKey"));
    }
    if (moduleConfig->hasKey("SendFirstCandidateWithSpaceWithOnePageList")) {
        m_configSendFirstCandidateWithSpaceWithOnePageList = moduleConfig->isKeyTrue("SendFirstCandidateWithSpaceWithOnePageList");
    }
}

void OVAFAssociatedPhrases::saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    moduleConfig->setKeyBoolValue("ContinuousAssociation", m_continuousAssociation);
    moduleConfig->setKeyStringValue("SelectionKeys", m_selectionKeys);
    moduleConfig->setKeyStringValue("ShiftKeySymbol", m_shiftKeySymbol);
    moduleConfig->setKeyBoolValue("SendFirstCandidateWithSpaceWithOnePageList", m_configSendFirstCandidateWithSpaceWithOnePageList);
    moduleConfig->setKeyIntValue("UseSpaceAsFirstCandidateSelectionKey", m_configUseSpaceAsFirstCandidateSelectionKey);
}

void OVAFAssociatedPhrases::checkTable()
{
    bool customExists = !m_customTablePath.empty() && OVPathHelper::PathExists(m_customTablePath);
    OVFileTimestamp timestamp = OVPathHelper::TimestampForPath(m_tablePath);
    OVFileTimestamp customTimestamp = OVPathHelper::TimestampForPath(m_customTablePath);
    if (m_checkedTable && customExists == m_customTableExists &&
        timestamp == m_tableTimestamp && customTimestamp == m_customTableTimestamp) return;

    m_checkedTable = true;
    m_customTableExists = customExists;
    m_tableTimestamp = timestamp;
    m_customTableTimestamp = customTimestamp;
    OVCINDataTable* replacement = customExists ? LoadTable(m_customTablePath) : nullptr;
    m_tableStatus = replacement ? Custom : (customExists ? CustomInvalid : BuiltIn);
    if (!replacement) replacement = LoadTable(m_tablePath);
    if (!replacement) m_tableStatus = Unavailable;
    delete m_table;
    m_table = replacement;
}

vector<pair<OVKey, string>> OVAFAssociatedPhrases::getSelectionKeyLabelPairs(OVLoaderService* loaderService)
{
    vector<pair<OVKey, string>> keyLabelPairs;
    string selectionKeys = m_selectionKeys;
    if (m_configUseSpaceAsFirstCandidateSelectionKey == OriginalFirstKeySelectsSecondCandidate) {
        selectionKeys = string(" ") + selectionKeys;
    }

    for (int i = 0; i < selectionKeys.size(); i++) {
        string key = string(1, selectionKeys[i]);
        string label = key;
        if (i == 0 && m_configUseSpaceAsFirstCandidateSelectionKey == SpaceAndOriginalFirstKeySelectsFirstCandidate) {
            label = "␣";
        } else if (label == " ") {
            label = "␣";
        } else {
            string::size_type loc = string(kDefaultSelectionKeys).find(label, 0);
            if (loc != string::npos) {
                label = m_shiftKeySymbol + string(1, kDefaultUnshiftedSelectionKeyLabels[loc]);
            }
        }
        keyLabelPairs.push_back(make_pair(loaderService->makeOVKey(key), label));
    }
    return keyLabelPairs;
}

void OVAFAssociatedPhrases::reloadTable()
{
    m_checkedTable = false;
    checkTable();
}

OVCINDataTable* OVAFAssociatedPhrases::ParseTable(const string& data)
{
    if (data.empty() || data.find('\0') != string::npos) return nullptr;
    // Validate UTF-8 by round-tripping code points with the existing UTF-8 helper.
    size_t validatedBytes = 0;
    for (const auto& character : OVUTF8Helper::SplitStringByCodePoint(data)) {
        validatedBytes += character.size();
        unsigned int codePoint = OVUTF8Helper::CodePointFromSingleUTF8String(character);
        if (codePoint > 0x10ffff || (codePoint >= 0xd800 && codePoint <= 0xdfff) ||
            OVUTF8Helper::SingleUTF8StringFromCodePoint(codePoint) != character) return nullptr;
    }
    if (validatedBytes != data.size()) return nullptr;
    OVCINDataTableParser parser;
    std::unique_ptr<OVCINDataTable> table(parser.CINDataTableFromString(data.c_str()));
    if (!table || !table->chardefMap()->size()) return nullptr;
    for (size_t i = 0; i < table->chardefMap()->size(); ++i) {
        const auto entry = table->chardefMap()->keyValuePairAtIndex(i);
        if (OVUTF8Helper::SplitStringByCodePoint(entry.first).size() != 1 || entry.second.empty()) return nullptr;
    }
    return table.release();
}

OVCINDataTable* OVAFAssociatedPhrases::LoadTable(const string& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) return nullptr;
    string data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return ParseTable(data);
}

bool OVAFAssociatedPhrases::ValidateTable(const string& data)
{
    std::unique_ptr<OVCINDataTable> table(ParseTable(data));
    return bool(table);
}
