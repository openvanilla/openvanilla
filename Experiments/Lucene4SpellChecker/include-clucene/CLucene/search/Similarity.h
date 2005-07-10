#include "CLucene/StdHeader.h"
#ifndef _lucene_search_Similarity_
#define _lucene_search_Similarity_

#include "CLucene/index/Term.h"
#include "SearchHeader.h"

namespace lucene{ namespace search {
	
	// Internal class used for scoring.
	// <p>Public only so that the indexing code can compute and store the
	// normalization byte for each document. 
	class Similarity {
	private:
		
		static float_t* SIMILARITY_NORM_TABLE();

	public:
		// Computes the normalization byte for a document given the total number of
		// terms contained in the document.  These values are stored in an index and
		// used by the search code. 
		static l_byte_t normb(const int_t numTerms);

		static float_t normf(const l_byte_t normByte);

		static float_t tf(const int_t freq);

		static float_t tf(const float_t freq);
		    
		static float_t idf(const lucene::index::Term& term, const Searcher& searcher);

		static float_t idf(const int_t docFreq, const int_t numDocs);
		    
		static float_t coord(const int_t overlap, const int_t maxOverlap);
	};
}}
#endif
