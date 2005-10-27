#ifndef _LUCENESEARCH_H_
#define _LUCENESEARCH_H_

#include <vector>
#include <string>

#include "CLucene.h"

using namespace std;

using namespace lucene::analysis;
using namespace lucene::index;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::search;

class LuceneSearch {
public:    
    static void run(
        string& queryString,
        string& modulePathString,
        vector<string>& outputVectorRef)
    {
        standard::StandardAnalyzer analyzer;
		QueryParser parser(_T("word"), &analyzer);
		Query* q = parser.Parse((const TCHAR*)queryString.c_str());
    	string indexDirPath = modulePathString + "/indexDir";
    	IndexReader* r = IndexReader::open(indexDirPath.c_str());
        IndexSearcher s(r);

    	Hits* h = s.search(q);
        for (int i = 0; i < h->length(); i++) {
            Document doc = h->doc(i);
			string foundFieldValue((char *)doc.get(_T("word")));
            outputVectorRef.push_back(foundFieldValue);
    	}

        delete h;
    	delete q;
        s.close();
    	r->close();
        delete r;
    }
};

#endif
