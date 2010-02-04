//
// TestOVCINDatabaseService.cpp
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

#include <OpenVanilla/OpenVanilla.h>
#include <iostream>

using namespace OpenVanilla;

OVBenchmark g_benchmark;
string g_benchmarkItem;

void start(const string& msg)
{
    g_benchmarkItem = msg;
    cout << g_benchmarkItem << " started." << endl;
    g_benchmark.start();
}

void end()
{
    g_benchmark.stop();
    cout << g_benchmarkItem << "ended, elapsed time " << g_benchmark.elapsedSeconds() << endl;
}


int main(int argc, char* argv[])
{
    string queryString;
    
    if (argc < 2) {
        cerr << "usage TestOVCINDatabaseService path" << endl;
        return 1;
    }
    
    if (argc > 2)
        queryString = argv[2];
    else
        queryString = "a";
    
    start(".cin scan");
    OVCINDatabaseService dataService(argv[1], "*.cin", "", 0);
    end();
    
    vector<string> tables = dataService.tables();
    vector<string>::iterator iter;
    
    start("preparse all .cins");
    for (iter = tables.begin() ; iter != tables.end() ; ++iter)
    {
        string ename = dataService.valueForPropertyInTable("ename", *iter);
        string cname = dataService.valueForPropertyInTable("cname", *iter);

        cout << *iter << ", ename = " << ename << ", cname = " << cname << endl;
    }
    end();        
    
    start("read all .cins");
    for (iter = tables.begin() ; iter != tables.end() ; ++iter)
    {
        OVBenchmark benchmark;
        benchmark.start();
        OVKeyValueDataTableInterface* table = dataService.createKeyValueDataTableInterface(*iter);
        
        vector<string> result = table->valuesForKey(queryString);
        
        cout << "table = " << *iter << ", ";
        cout << "%ename = " << table->valueForProperty("ename") << ", ";
        
        if (result.size()) {
            cout << "find key a = ";
            
            for (vector<string>::iterator iter = result.begin() ; iter != result.end() ; ++iter)
                cout << *iter << ", ";
            cout << endl;
        }
        else {
            cout << "find key a = (not found)" << endl;
        }
        
        benchmark.stop();
        
    }    
    end();
    
    return 0;
}


