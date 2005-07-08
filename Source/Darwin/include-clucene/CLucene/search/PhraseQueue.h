#include "CLucene/StdHeader.h"
#ifndef _lucene_search_PriorityQueue_
#define _lucene_search_PriorityQueue_

#include "CLucene/util/PriorityQueue.h"
#include "PhrasePositions.h"

namespace lucene{ namespace search{
	class PhraseQueue: public lucene::util::PriorityQueue<PhrasePositions*> {
	public:
		PhraseQueue(const int_t size) {
			initialize(size,false);
		}
		~PhraseQueue(){
		}

	protected:
		bool lessThan(PhrasePositions* pp1, PhrasePositions* pp2) {
			if (pp1->doc == pp2->doc) 
				return pp1->position < pp2->position;
			else
				return pp1->doc < pp2->doc;
		}
	};
}}
#endif
