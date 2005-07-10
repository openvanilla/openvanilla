#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_QueryParser_
#define _lucene_queryParser_QueryParser_

#include "QueryParserConstants.h"
#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/util/Reader.h"
#include "CLucene/search/SearchHeader.h"
#include "CLucene/index/Term.h"

#include "TokenList.h"
#include "QueryToken.h"
#include "QueryParserBase.h"
#include "Lexer.h"

using namespace lucene::util;
using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::search;
namespace lucene{ namespace queryParser{

	//QueryParser is a query parser.
	//The only method that clients should need to call is Parse().
	//The syntax for query const char_t*s is as follows:
	//A Query is a series of clauses. A clause may be prefixed by:
	//
	//a plus (+) or a minus (-) sign, indicating that the 
	//clause is required or prohibited respectively; or
	//a term followed by a colon, indicating the field to be searched.
	//This enables one to construct queries which search multiple fields.
	//
	//
	//A clause may be either:
	//
	//a term, indicating all the documents that contain this term; or
	//a nested query, enclosed in parentheses. Note that this may be 
	//used with a +/- prefix to require any of a set of terms.
	//
	//
	//Thus, in BNF, the query grammar is:
	//
	//Query  ::= ( Clause )*
	//Clause ::= ["+", "-"] [<TERM> ":"] (<TERM> | "(" <QUERY> ")" )
	//
	//
	//Examples of appropriately formatted queries can be found in the test cases.
	//

	class QueryParser : public QueryParserBase
	{
	private:
		Analyzer& analyzer;
		const char_t* field;
		TokenList* tokens;
	
	public:
		// Initializes a new instance of the QueryParser class with a specified field and
		// analyzer values.
		QueryParser(const char_t* _field, Analyzer& _analyzer);
		~QueryParser();

		// Returns a new instance of the Query class with a specified query, field and
		// analyzer values.
		static Query& Parse(const char_t* query, const char_t* field, Analyzer& analyzer);

		// Returns a parsed Query instance.
		Query& Parse(const char_t* query);

		// Returns a parsed Query instance.
		Query& Parse(Reader& reader);

	private:
		//matches for CONJUNCTION
		//CONJUNCTION ::= <AND> | <OR>
		int_t MatchConjunction();

		//matches for MODIFIER
		//MODIFIER ::= <PLUS> | <MINUS> | <NOT>
		int_t MatchModifier();


		//matches for QUERY
		//QUERY ::= [MODIFIER] CLAUSE (<CONJUNCTION> [MODIFIER] CLAUSE)*
		Query* MatchQuery(const char_t* field);

		//matches for CLAUSE
		//CLAUSE ::= [TERM <COLON>] ( TERM | (<LPAREN> QUERY <RPAREN>))
		Query* MatchClause(const char_t* field);

		//matches for TERM
		//TERM ::= TERM | PREFIXTERM | WILDTERM | NUMBER
		//         [ <FUZZY> ] [ <CARAT> <NUMBER> [<FUZZY>]]
		//
		//          | (<RANGEIN> | <RANGEEX>) [<CARAT> <NUMBER>]
		//			| <QUOTED> [SLOP] [<CARAT> <NUMBER>]
		Query* MatchTerm(const char_t* field);

		// matches for QueryToken of the specified type and returns it
		// otherwise Exception throws
		QueryToken* MatchQueryToken(QueryTokenTypes expectedType);

		//Extracts the first token from the Tokenlist tokenlist
	    //and destroys it
		void QueryParser::ExtractAndDeleteToken(void);
	};
}}
#endif
