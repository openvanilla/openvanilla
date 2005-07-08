#include "CLucene/StdHeader.h"
#ifndef _lucene_search_Scorer_
#define _lucene_search_Scorer_

#include "HitCollector.h"
namespace lucene{ namespace search {
    class Scorer {
	public:
		virtual ~Scorer(){
		}
		virtual void score(HitCollector& hc, const int_t maxDoc) = 0;
    };
}}
#endif
