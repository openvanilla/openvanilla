#ifndef _LUCENESEARCH_H_
#define _LUCENESEARCH_H_

#include <vector>
#include <string>

#include <windows.h>

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
		Query* q = NULL;
		IndexReader* r = NULL;
		IndexSearcher* s = NULL;
		Hits* h = NULL;
		
		TCHAR queryStringT[100];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, queryString.c_str(), queryString.length(), queryStringT, 100);
		q = QueryParser::Parse(queryStringT, _T("word"), &analyzer);
    	string indexDirPath = modulePathString + "indexDir";
    	r = IndexReader::open(indexDirPath.c_str());
		s = new IndexSearcher(r);
    	h = s->search(q);
		for (int i = 0; i < h->length() - 1; i++) {
			Document doc = h->doc(i);

			LPTSTR resultT = doc.get(_T("word"));
			int resultLen = _tcslen(resultT);
			char result[100];
			_strset(result, '\0');
			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, resultT, resultLen, result, 100, NULL, NULL);

			string foundFieldValue(result);
			outputVectorRef.push_back(foundFieldValue);
		}

		s->close();
    	r->close();
		/*
		delete h;
		delete q;
		delete s;
		delete r;
		*/
    }
};

#endif
