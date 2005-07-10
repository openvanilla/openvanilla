#ifndef _lucene_search_ScoreDoc_
#define _lucene_search_ScoreDoc_

namespace lucene{ namespace search {
	struct ScoreDoc {
	public:
		float_t score;
		int_t doc;

		ScoreDoc(const int_t d, const float_t s):
			score(s),
			doc(d)
		{
	    }
	};
}}
#endif
