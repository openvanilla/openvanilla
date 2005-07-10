#include "CLucene/StdHeader.h"
#ifndef _lucene_search_SloppyPhraseScorer_
#define _lucene_search_SloppyPhraseScorer_
#ifndef NO_FUZZY_QUERY

#include "PhraseScorer.h"
#include "CLucene/index/Terms.h"

using namespace lucene::index;

namespace lucene{ namespace search{
	
  class SloppyPhraseScorer: public PhraseScorer {
     private:
	   int_t slop;

     public:
       //Constructor
       SloppyPhraseScorer(TermPositions** tps, int_t tpsLength, int_t s, l_byte_t* n, float_t w);
       //Destructor
       ~SloppyPhraseScorer(){}

     protected:
        //Returns the freqency of the phrase
		float_t phraseFreq();
	};
}}
#endif
#endif

