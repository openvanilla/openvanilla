#include "CLucene/StdHeader.h"
#ifndef _lucene_search_HitQueue_
#define _lucene_search_HitQueue_

#include "CLucene/util/PriorityQueue.h"
#include "ScoreDoc.h"

namespace lucene{ namespace search {
	class HitQueue: public lucene::util::PriorityQueue<ScoreDoc*> {
	public:
		HitQueue(const int_t size);
		~HitQueue();

	protected:
		bool lessThan(ScoreDoc* hitA, ScoreDoc* hitB);
	};
}}
#endif
