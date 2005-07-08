#include "CLucene/StdHeader.h"
#ifndef _lucene_search_BooleanQuery_
#define _lucene_search_BooleanQuery_

#include "CLucene/index/IndexReader.h"
#include "CLucene/util/StringBuffer.h"
#include "SearchHeader.h"
#include "BooleanClause.h"
#include "BooleanScorer.h"
#include "Scorer.h"

using namespace lucene::index;
namespace lucene{ namespace search {

    // A Query that matches documents matching boolean combinations of other
    // queries, typically {@link TermQuery}s or {@link PhraseQuery}s.
	class BooleanQuery:public Query {
	private:
		lucene::util::VoidList<BooleanClause*>clauses;
    
	public:
        // Constructs an empty boolean query. 
        BooleanQuery();
        
        ~BooleanQuery();
        
        const char_t* getQueryName() const;
        
        // Adds a clause to a boolean query.  Clauses may be:
        //<ul>
        //<li><code>required</code> which means that documents which <i>do not</i>
        //match this sub-query will <i>not</i> match the boolean query;
        //<li><code>prohibited</code> which means that documents which <i>do</i>
        //match this sub-query will <i>not</i> match the boolean query; or
        //<li>neither, in which case matched documents are neither prohibited from
        //nor required to match the sub-query.
        //</ul>
        //It is an error to specify a clause as both <code>required</code> and
        //<code>prohibited</code>.
		void add(Query& query, const bool deleteQuery, const bool required, const bool prohibited);
	    
		// Adds a clause to a boolean query. 
		void add(BooleanClause& clause);
	    
		void prepare(IndexReader& reader);
	      
		//added by search highlighter
		BooleanClause** getClauses();

	    /* DSR:PROPOSED: Give client code access to clauses.size() so we know how
	    ** large the array returned by getClauses is. */
	    int_t getClauseCount();

		float_t sumOfSquaredWeights(Searcher& searcher);
	    
		void normalize(const float_t norm);
	    
		Scorer* scorer(IndexReader& reader);
	    
		// Prints a user-readable version of this query. 
		const char_t* toString(const char_t* field);    
    };
}}
#endif
