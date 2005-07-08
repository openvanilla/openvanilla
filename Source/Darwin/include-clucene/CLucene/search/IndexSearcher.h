#include "CLucene/StdHeader.h"
#ifndef _lucene_search_IndexSearcher_
#define _lucene_search_IndexSearcher_

#include "SearchHeader.h"
#include "CLucene/store/Directory.h"
#include "CLucene/document/Document.h"
#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "CLucene/util/BitSet.h"
#include "HitQueue.h"

using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::document;

namespace lucene{ namespace search {
	class SimpleTopDocsCollector:public HitCollector{ 
	private:
		float_t minScore;
		const BitSet* bits;
		HitQueue& hq;
		const int_t nDocs;
		int_t* totalHits;
		const float_t ms;
	public:
		SimpleTopDocsCollector(const BitSet* bs, HitQueue& hitQueue, int_t* totalhits,const int_t ndocs, const float_t minScore);

		void collect(const int_t doc, const float_t score);
	};

	class SimpleFilteredCollector:	public HitCollector{
	private:
		BitSet* bits;
		HitCollector& results;
	public:
		SimpleFilteredCollector(BitSet* bs, HitCollector& collector);
	protected:
		void collect(const int_t doc, const float_t score);
	};

	// Implements search over a single IndexReader. 
	class IndexSearcher:public Searcher{
	private:
		IndexReader *reader;
		bool readerOwner;

	public:
		// Creates a searcher searching the index in the named directory.
		IndexSearcher(const char_t* path);

		// Creates a searcher searching the provided index. 
		IndexSearcher(IndexReader& r);

		~IndexSearcher();
		    
		// Frees resources associated with this Searcher. 
		void close();

		int_t docFreq(const Term& term) const;

		// For use by {@link HitCollector} implementations. 
		Document& doc(const int_t i);

		int_t maxDoc() const;

		TopDocs& Search(Query& query, const Filter* filter, const int_t nDocs);

		void Search(Query& query, const Filter* filter, HitCollector& results);
	};
}}
#endif
