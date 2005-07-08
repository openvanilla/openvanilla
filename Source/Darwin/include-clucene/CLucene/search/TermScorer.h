#include "CLucene/StdHeader.h"
#ifndef _lucene_search_TermScorer_
#define _lucene_search_TermScorer_

#include "CLucene/index/Terms.h"
#include "Scorer.h"
#include "CLucene/search/Similarity.h"

using namespace lucene::index;
namespace lucene{ namespace search {
    
	class TermScorer: public Scorer {
	private:
		TermDocs& termDocs;
		l_byte_t* norms;
		const float_t weight;
		int_t doc;

		int_t docs[128];	  // buffered doc numbers
		int_t freqs[128];	  // buffered term freqs
		int_t pointer;
		int_t pointerMax;

		float_t scoreCache[LUCENE_SCORE_CACHE_SIZE];
	public:

		//TermScorer takes TermDocs and delets it when TermScorer is cleaned up
		TermScorer(TermDocs& td, l_byte_t* _norms, const float_t w);

		~TermScorer();

		void score(HitCollector& c, const int_t end);
    };
}}
#endif
