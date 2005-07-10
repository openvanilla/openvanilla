#include "CLucene/StdHeader.h"
#ifndef _lucene_search_ExactPhraseScorer_
#define _lucene_search_ExactPhraseScorer_

#include "PhraseScorer.h"
#include "CLucene/index/Terms.h"

using namespace lucene::index;
namespace lucene{ namespace search{

  class ExactPhraseScorer: public PhraseScorer {
    public:
      //Constructor
      ExactPhraseScorer(TermPositions** tps, int_t tpsLength, l_byte_t* n, float_t w);
      //Destructor
      ~ExactPhraseScorer(){}
    protected:
      //Returns the exact freqency of the phrase
      float_t phraseFreq();
    };
}}
#endif
