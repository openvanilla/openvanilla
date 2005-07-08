#include "CLucene/StdHeader.h"
#ifndef _lucene_search_RangeQuery_
#define _lucene_search_RangeQuery_
#ifndef NO_RANGE_QUERY

#include "SearchHeader.h"
#include "Scorer.h"
#include "BooleanQuery.h"
#include "TermQuery.h"

#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"
#include "CLucene/index/IndexReader.h"

#include "CLucene/util/StringBuffer.h"


using namespace lucene::index;
using namespace lucene::util;
namespace lucene{ namespace search{
    // A Query that matches documents within an exclusive range. 
	class RangeQuery: public Query
    {
    private: 
        Term* lowerTerm;
        Term* upperTerm;
        bool inclusive;
        IndexReader* reader;
        BooleanQuery* query;
        
    public:
        // Constructs a query selecting all terms greater than 
        // <code>lowerTerm</code> but less than <code>upperTerm</code>.
        // There must be at least one term and either term may be NULL--
        // in which case there is no bound on that side, but if there are 
        // two term, both terms <b>must</b> be for the same field.
		RangeQuery(Term* LowerTerm, Term* UpperTerm, const bool Inclusive);
		~RangeQuery();

		const char_t* getQueryName() const;
        
        void prepare(IndexReader& reader);
        
        float_t sumOfSquaredWeights(Searcher& searcher);
        
        void normalize(const float_t norm);
        
        Scorer* scorer(IndexReader& reader);
        
        BooleanQuery* getQuery();
        
        
        // Prints a user-readable version of this query. 
        const char_t* toString(const char_t* field);
        
        
    private:
        const char_t* getField();
    };

}}
#endif
#endif
