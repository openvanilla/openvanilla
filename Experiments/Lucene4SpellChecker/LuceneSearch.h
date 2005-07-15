#ifndef _LUCENESEARCH_H_
#define _LUCENESEARCH_H_

#include <vector>
#include <string>

#include "CLucene.h"

using namespace std;

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::document;
using namespace lucene::search;
using namespace lucene::queryParser;

class LuceneSearch {
public:    
    static void run(
        string& queryString,
        string& modulePathString,
        vector<string>& outputVectorRef)
    {
        char* field = "word";
        standard::StandardAnalyzer analyzer;
    	Query& q = QueryParser::Parse(queryString.c_str(), field, analyzer);
    	string indexDirPath = modulePathString + "/indexDir";
    	IndexReader& r = IndexReader::open(indexDirPath.c_str());
        IndexSearcher s(r);

    	Hits& h = s.search(q);
        for (int i = 0; i < h.Length(); i++) {
            Document& doc = h.doc(i);
    		string foundFieldValue(doc.get(field));
            outputVectorRef.push_back(foundFieldValue);
    	}

        delete &h;
    	delete &q;
        s.close();
    	r.close();
        delete &r;
    }
};

#endif
