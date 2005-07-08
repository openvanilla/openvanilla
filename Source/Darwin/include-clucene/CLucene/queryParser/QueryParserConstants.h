#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_QueryParserConstants_
#define _lucene_queryParser_QueryParserConstants_

namespace lucene{ namespace queryParser{

	enum QueryTokenTypes
	{
		AND_,
		OR,
		NOT,
		PLUS,
		MINUS,
		LPAREN,
		RPAREN,
		COLON,
		CARAT,
		QUOTED,
		TERM,
		FUZZY,
		SLOP,
		PREFIXTERM,
		WILDTERM,
		RANGEIN,
		RANGEEX,
		NUMBER,
		EOF_
	};

}}

#endif
