#include "CLucene/StdHeader.h"
#ifndef _lucene_search_TermQuery_
#define _lucene_search_TermQuery_

#include "SearchHeader.h"
#include "Scorer.h"
#include "CLucene/index/Term.h"
#include "TermScorer.h"
#include "CLucene/index/IndexReader.h"
#include "CLucene/util/StringBuffer.h"
#include "CLucene/index/Terms.h"

using namespace lucene::index;
namespace lucene{ namespace search {

    // A Query that matches documents containing a term.
    //  this may be combined with other terms with a {@link BooleanQuery}.
    class TermQuery: public Query {
    private:
		Term* term;
		float_t idf;
		float_t weight;
	public:
		// Constructs a query for the term <code>t</code>. 
		TermQuery(Term& t);
		~TermQuery();

		const char_t* getQueryName() const;
	    
		float_t sumOfSquaredWeights(Searcher& searcher);
	    
		void normalize(const float_t norm);
	      
		//added by search highlighter
		Term* getTerm()
		;
	    
		Scorer* scorer(IndexReader& reader);
	    
		// Prints a user-readable version of this query. 
		const char_t* toString(const char_t* field);

		void prepare(IndexReader& reader){}
    };
}}
#endif

