#include "CLucene/StdHeader.h"
#ifndef _lucene_queryParser_Lexer_
#define _lucene_queryParser_Lexer_

#include "QueryParserConstants.h"

#include "CLucene/util/FastCharStream.h"
#include "CLucene/util/Reader.h"
#include "CLucene/util/StringBuffer.h"

#include "TokenList.h"

using namespace lucene::util;
namespace lucene{ namespace queryParser{

	// A simple Lexer that is used by QueryParser.
	class Lexer
	{
	private:
		FastCharStream* reader;
		bool delSR;  //Indicates if the InputStream input of reader must be deleted or not

    public:
		// Initializes a new instance of the Lexer class with the specified
		// query to lex.
		Lexer(const char_t* query);

		// Initializes a new instance of the Lexer class with the specified
		// TextReader to lex.
		Lexer(Reader& source);

		//Breaks the input stream onto the tokens list tokens
		void Lex(TokenList *tokens);
		
		~Lexer();

	private:
		QueryToken* GetNextToken();

		// Reads an integer number
		const char_t* ReadIntegerNumber(const uchar_t ch);

#ifndef NO_RANGE_QUERY
		// Reads an inclusive range like [some words]
		QueryToken* ReadInclusiveRange(const uchar_t prev);

		// Reads an exclusive range like {some words}
		QueryToken* ReadExclusiveRange(const uchar_t prev);
#endif

		// Reads quoted string like "something else"
		QueryToken* ReadQuoted(const uchar_t prev);

		QueryToken* ReadTerm(const uchar_t prev);


		const char_t* ReadEscape(const uchar_t prev);
	};
}}
#endif
