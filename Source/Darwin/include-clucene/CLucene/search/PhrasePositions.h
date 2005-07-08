#include "CLucene/StdHeader.h"
#ifndef _lucene_search_PhrasePositions_
#define _lucene_search_PhrasePositions_

#include "CLucene/index/Terms.h"

using namespace lucene::index;
namespace lucene{ namespace search{

  class PhrasePositions {
    public:
      int_t doc;             //current doc
      int_t position;        //position in doc
      int_t count;           //remaining pos in this doc
      int_t offset;          // position in phrase
      TermPositions* tp;     // stream of positions
      PhrasePositions* next; // used to make lists

	  //Constructor
      PhrasePositions(TermPositions* Tp, const int_t o);
	  //Destructor
	  ~PhrasePositions();

      void Next();

      void firstPosition();

      bool nextPosition();
	};
}}
#endif
