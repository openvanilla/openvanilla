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


