#include "CLucene/StdHeader.h"
#ifndef _lucene_search_PhraseScorer_
#define _lucene_search_PhraseScorer_

#include "PhraseQueue.h"
#include "PhrasePositions.h"
#include "HitCollector.h"
#include "Scorer.h"
#include "Similarity.h"

namespace lucene{ namespace search{

	class PhraseScorer: public Scorer {
	protected:
		l_byte_t* norms;
		float_t weight;

		PhraseQueue* pq;        //is used to order the list point to by first and last
		PhrasePositions* first; //Points to the first in the list of PhrasePositions
		PhrasePositions* last;  //Points to the last in the list of PhrasePositions

	public:
		//Constructor
		PhraseScorer(TermPositions** tps, const int_t tpsLength, l_byte_t* n, float_t w);
		//Destructore
		~PhraseScorer();

		void score(HitCollector& results, const int_t end);

	protected:
		virtual float_t phraseFreq()=0;

        //Transfers the PhrasePositions from the PhraseQueue pq to
        //the PhrasePositions list with first as its first element
		void pqToList();

		//Moves first to the end of the list
		void firstToLast();
	};
}}
#endif
