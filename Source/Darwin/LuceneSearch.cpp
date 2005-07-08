#include "LuceneSearch.h"

static void run(const char* query, vector<const char*>& outputVectorRef)
{
	char* field = "word";
	standard::StandardAnalyzer analyzer;
	Query& q = QueryParser::Parse(query, field, analyzer);
	IndexReader& r = IndexReader::open("./index");
	IndexSearcher s(r);

	Hits& h = s.search(q);
	for (int i = 0; i < h.Length(); i++) {
		Document& doc = h.doc(i);
		const char* foundFieldValue = doc.get(field);
		outputVectorRef.push_back(foundFieldValue);
	}

	delete &h;
	delete &q;
	s.close();
	r.close();
	delete &r;
}
