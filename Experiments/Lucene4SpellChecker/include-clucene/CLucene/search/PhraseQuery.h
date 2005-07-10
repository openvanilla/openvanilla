#include "CLucene/StdHeader.h"
#ifndef _lucene_search_PhraseQuery_
#define _lucene_search_PhraseQuery_

#include "SearchHeader.h"
#include "Scorer.h"
#include "BooleanQuery.h"
#include "TermQuery.h"

#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"
#include "CLucene/index/IndexReader.h"

#include "CLucene/util/StringBuffer.h"
#include "CLucene/util/VoidList.h"

#include "ExactPhraseScorer.h"
#include "SloppyPhraseScorer.h"

using namespace lucene::util;
namespace lucene{ namespace search{

	// A Query that matches documents containing a particular sequence of terms.
	// This may be combined with other terms with a {@link BooleanQuery}.
	class PhraseQuery: public Query {
	private:
		const char_t* field;
		VoidList<Term*> terms;
		float_t idf;
		float_t weight;

		int_t slop;

	public:
        //Constructor
        PhraseQuery();
		//Destructor
		~PhraseQuery();

        //Returns the string "PhraseQuery"
        const char_t* getQueryName() const;
        
        //Sets the number of other words permitted between words in query phrase.
        //If zero, then this is an exact phrase search.  For larger values this works
        //like a WITHIN or NEAR operator.
        //
        //The slop is in fact an edit-distance, where the units correspond to
        //moves of terms in the query phrase out of position.  For example, to switch
        //the order of two words requires two moves (the first move places the words
        //atop one another), so to permit re-orderings of phrases, the slop must be
        //at least two.
        //
        //More exact matches are scored higher than sloppier matches, thus search
        //results are sorted by exactness.
        //
        //The slop is zero by default, requiring exact matches.
        void setSlop(const int_t s) { slop = s; }
        
        //Returns the slop.  See setSlop(). 
        int_t getSlop() { return slop; }
        
        //Adds a term to the end of the query phrase. 
        bool add(Term* term);
        
		//Returns the sum of squared weights 
        float_t sumOfSquaredWeights(Searcher& searcher);
        
		//Normalizes the Weight
        void normalize(const float_t norm);
        
        Scorer* scorer(IndexReader& reader);
        
        //added by search highlighter
        void getTerms(Term**& ret, int_t& size);
        
        //Prints a user-readable version of this query. 
        const char_t* toString(const char_t* f);

		void prepare(IndexReader& reader){}
	};
}}
#endif
