#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_QueryToken_
#define _lucene_queryParser_QueryToken_

#include "QueryParserConstants.h"

namespace lucene{ namespace queryParser{

	// Token class that used by QueryParser.
	class QueryToken
	{
	public:
		//Internal constant.
		const char_t* Value;
		//Internal constant.
		const int_t Start;
		//Internal constant.
		const int_t End;
		//Internal constant.
		const QueryTokenTypes Type;

		// Initializes a new instance of the Token class LUCENE_EXPORT.
		QueryToken(const char_t* value, const int_t start, const int_t end, const QueryTokenTypes type);

		~QueryToken();

		// Initializes a new instance of the Token class LUCENE_EXPORT.
		QueryToken(const char_t* value, const QueryTokenTypes type);

		// Initializes a new instance of the Token class LUCENE_EXPORT.
		QueryToken(const QueryTokenTypes type);

		// Returns a string representation of the Token.
		//public override string ToString();
	};
}}
#endif
