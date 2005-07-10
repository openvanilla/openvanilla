#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_TokenList_
#define _lucene_queryParser_TokenList_

#include "CLucene/util/VoidList.h"
#include "QueryToken.h"
namespace lucene{ namespace queryParser{

	// Represents a list of the tokens.
	class TokenList
	{
	private:
		lucene::util::VoidList<QueryToken*> tokens;

    public:
		TokenList();
		~TokenList();

		void Add(QueryToken* token);

		void Push(QueryToken* token);

		QueryToken& Peek();

		QueryToken* Extract();

		int_t Count();
	};
}}
#endif
