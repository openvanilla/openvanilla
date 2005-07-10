#include "CLucene/StdHeader.h"
#ifndef _lucene_search_PrefixQuery
#define _lucene_search_PrefixQuery
#ifndef NO_PREFIX_QUERY

#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"
#include "CLucene/index/IndexReader.h"
#include "SearchHeader.h"
#include "BooleanQuery.h"
#include "TermQuery.h"
#include "CLucene/util/StringBuffer.h"

using namespace lucene::index;
namespace lucene{ namespace search{ 
	//PrefixQuery is a Query that matches documents containing terms with a specified prefix.

	class PrefixQuery: public Query {
	private:
		Term* prefix;
		BooleanQuery* query;
		IndexReader* reader;
	public:

		//Constructor. Constructs a query for terms starting with prefix
		PrefixQuery(Term* Prefix);

		//Destructor
		~PrefixQuery();

		//Returns the name "PrefixQuery"
		const char_t* getQueryName()const;

		//Resets the PrefixQuery.
		void prepare(IndexReader& reader) ;

		//Sums the squard weights of the rewritten prefix query
		float_t sumOfSquaredWeights(Searcher& searcher);

		//Normalizes the prefix query
		void normalize(const float_t norm) ;

		Scorer* scorer(IndexReader& reader) ;

		//Rewrites the PrefixQuery as an alternate, more primitive query.
		BooleanQuery* getQuery() ;

		//Creates a user-readable version of this query and returns it as as string
		const char_t* toString(const char_t* field) ;
	};
}}
#endif
#endif
