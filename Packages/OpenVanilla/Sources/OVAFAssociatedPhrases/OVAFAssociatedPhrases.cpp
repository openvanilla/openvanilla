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

using namespace OpenVanilla;

static const char *const kDefaultSelectionKeys = "!@#$%^&*()";
static const char *const kDefaultUnshiftedSelectionKeyLabels = "1234567890";
static const char *const kDefaultShiftKeySymbol = "⇧";

OVAFAssociatedPhrases::OVAFAssociatedPhrases(const string& tablePath)
    : m_tablePath(tablePath)
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
    if (!OVPathHelper::PathExists(m_tablePath)) {
        return false;
    }

    m_tableTimestamp = OVPathHelper::TimestampForPath(m_tablePath);

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
    if (!OVPathHelper::PathExists(m_tablePath)) {
        return;
    }

    OVFileTimestamp timestamp = OVPathHelper::TimestampForPath(m_tablePath);

    // if table exists and the timestamp is not newer, bail
    if (m_table && !(timestamp > m_tableTimestamp)) {
        return;
    }

    // if table exists, delete the table
    if (m_table) {        
        delete m_table;
        m_table = 0;
    }

    // update timestamp and reload the table
    m_tableTimestamp = timestamp;
    OVCINDataTableParser parser;
    m_table = parser.CINDataTableFromFileName(m_tablePath);
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
