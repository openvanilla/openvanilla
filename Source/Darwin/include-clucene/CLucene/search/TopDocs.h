#include "CLucene/StdHeader.h"
#ifndef _lucene_search_TopDocs_
#define _lucene_search_TopDocs_

#include "ScoreDoc.h"
namespace lucene{ namespace search {
	class TopDocs {
	public:
		const int_t totalHits;
		ScoreDoc** scoreDocs;
		const int_t scoreDocsLength;
		bool deleteScoreDocs;

		//TopDocs takes ownership of the ScoreDoc array
		TopDocs(const int_t th, ScoreDoc **sds, const int_t sdLength);
		~TopDocs();
	};
}}
#endif

