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

OVAFAssociatedPhrases::OVAFAssociatedPhrases(const string& tablePath)
    : m_tablePath(tablePath)
    , m_table(0)
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


void OVAFAssociatedPhrases::checkTable()
{
    if (!OVPathHelper::PathExists(m_tablePath)) {
        cerr << "a\n";
        return;
    }

    OVFileTimestamp timestamp = OVPathHelper::TimestampForPath(m_tablePath);

    // if table exists and the timestamp is not newer, bail
    if (m_table && !(timestamp > m_tableTimestamp)) {
        cerr << "b\n";
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
    cerr << "table? " << (void*)m_table << "\n";
}
