//
// TestOVSQLiteWrapper.cpp
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

#include "UnitTest++.h"

using namespace OpenVanilla;

TEST(SQLiteWrapper)
{
    OVSQLiteConnection* connection = OVSQLiteConnection::Open();
    CHECK(connection);

    if (connection) {
        CHECK_EQUAL(SQLITE_OK, connection->execute("create table foobar(key, value);"));        
        CHECK_EQUAL(SQLITE_OK, connection->execute("insert into foobar values('hello', 'world');"));
        
        OVSQLiteStatement* statement = connection->prepare("select * from foobar where key = ?");
        CHECK(statement);
        
        if (statement) {
            CHECK_EQUAL(SQLITE_OK, statement->bindTextToColumn("hello", 1));
        
            size_t rows = 0;
            
            while (statement->step() == SQLITE_ROW) {
                // cout << statement->textOfColumn(0) << ", " << statement->textOfColumn(1) << endl;
                CHECK_EQUAL("world", statement->textOfColumn(1));
                
                rows++;
            }
            
            CHECK_EQUAL(1, rows);
            
            delete statement;
        }
        
        delete connection;
    }
    
}
