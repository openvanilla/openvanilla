#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_QueryParserBase_
#define _lucene_queryParser_QueryParserBase_

#include "CLucene/search/BooleanClause.h"
#include "CLucene/util/VoidList.h"
#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/analysis/Analyzers.h"
#include "CLucene/index/Term.h"
#include "CLucene/search/TermQuery.h"
#include "CLucene/search/PhraseQuery.h"
#include "CLucene/search/RangeQuery.h"
#include "CLucene/search/PrefixQuery.h"
#include "CLucene/search/FuzzyQuery.h"



using namespace lucene::search;
using namespace lucene::util;
using namespace lucene::analysis;
using namespace lucene::index;
namespace lucene{ namespace queryParser{

	// Contains routines that used by QueryParser.
	class QueryParserBase
	{
    protected:
		static const int_t CONJ_NONE   = 0;
		static const int_t CONJ_AND    = 1;
		static const int_t CONJ_OR     = 2;

		static const int_t MOD_NONE    = 0;
		static const int_t MOD_NOT     = 10;
		static const int_t MOD_REQ     = 11;

    public:
		int_t PhraseSlop; //0

		QueryParserBase();
		~QueryParserBase();

		//Sets the the flag lowercaseExpandedTerms
        void setLowercaseExpandedTerms(bool LowercaseExpandedTerms);

        //Returns the value of lowercaseExpandedTerms
        bool getLowercaseExpandedTerms();

		static void throwParserException(const char_t* message, char_t ch, int_t col, int_t line );

    protected:
		// Adds the next parsed clause.
		void AddClause(VoidList<BooleanClause*>& clauses, int_t conj, int_t mods, Query* q);

		// Returns a query for the specified field.
		Query* GetFieldQuery(const char_t* field, Analyzer& analyzer, const char_t* queryText);

		// Returns a range query.
		Query* GetRangeQuery(const char_t* field, Analyzer& analyzer, const char_t* queryText, bool inclusive);

        //Analyzes the expanded term termStr with the StandardFilter and the LowerCaseFilter.
        char_t *QueryParserBase::AnalyzeExpandedTerm(const char_t* field,const char_t* termStr);
	

#ifndef NO_PREFIX_QUERY
        //Factory method for generating a query. Called when parser parses 
	    //an input term token that uses prefix notation; that is, 
	    //contains a single '*' wildcard character as its last character. 
        //Since this is a special case of generic wildcard term, 
        //and such a query can be optimized easily, this usually results in a different 
        //query object.
        //
        //Depending on settings, a prefix term may be lower-cased automatically. 
        //It will not go through the default Analyzer, however, since normal Analyzers are 
        //unlikely to work properly with wildcard templates. Can be overridden by extending 
        //classes, to provide custom handling for wild card queries, which may be necessary 
        //due to missing analyzer calls.
	    Query* QueryParserBase::GetPrefixQuery(const char_t* field,const char_t* termStr);
#endif	
#ifndef NO_FUZZY_QUERY
		//Factory method for generating a query (similar to getPrefixQuery}). Called when parser parses
        //an input term token that has the fuzzy suffix (~) appended.
        Query* QueryParserBase::GetFuzzyQuery(const char_t* field,const char_t* termStr);
#endif

        bool lowercaseExpandedTerms;
	};
}}
#endif
