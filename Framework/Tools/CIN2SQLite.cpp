//
// CIN2SQLite.cpp
//
// Copyright (c) 2007-2010 Lukhnos D. Liu
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

#define OV_USE_SQLITE

#if defined(__APPLE__)
    #include <OpenVanilla/OpenVanilla.h>
#else
    #include "OpenVanilla.h"
#endif

#include <iostream>

using namespace OpenVanilla;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cerr << "usage: CIN2SQLite cin-table sqlite3-db [table-name-prefix]" << endl;
        return 1;
    }
    
    OVCINDataTableParser parser;
    
    // note that we preserve case sensitivity here!
    OVCINDataTable* table = parser.CINDataTableFromFileName(argv[1], true);
    
    if (!table) {
        cerr << "cannot open .cin table: " << argv[1] << ", error code = " << parser.lastError() << endl;
        return 1;
    }

    OVSQLiteConnection* connection = OVSQLiteConnection::Open(argv[2]);
    if (!connection) {
        cerr << "cannot open database: " << argv[2] << endl;
        return 1;
    }
    
    string prefix = argc > 3 ? argv[3] : "";
    string tableName = OVPathHelper::FilenameWithoutPath(argv[1]);
    
    // replace .cin with -cin
    for (string::iterator iter = tableName.begin() ; iter != tableName.end() ; iter++)
        if (*iter == '.')
            *iter = '-';

    int result = 0;
        
    if (!OVCINToSQLiteConvertor::Convert(table, connection, prefix + tableName)) {
        cerr << "Failed to convert .cin to database" << endl;
        result = 1;
    }

    delete connection;
    delete table;
    return result;
}
