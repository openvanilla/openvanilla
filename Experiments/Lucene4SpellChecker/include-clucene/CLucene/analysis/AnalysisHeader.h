#include "CLucene/StdHeader.h"
#ifndef _lucene_analysis_AnalysisHeader_
#define _lucene_analysis_AnalysisHeader_

#include "CLucene/util/Reader.h"

namespace lucene{ namespace analysis{

	class Token {
	private:
		const int_t startOffset;				  // start in source text
		const int_t endOffset;				  // end in source text
		const char_t* type;				  // lexical type
		int_t positionIncrement;

	public:
#ifndef LUCENE_TOKEN_WORD_LENGTH
		char_t* termText;				  // the text of the term
#else
		char_t termText[LUCENE_TOKEN_WORD_LENGTH];				  // the text of the term
#endif

		// Constructs a Token with the given term text, and start & end offsets.
		//	The type defaults to "word."
		Token(const char_t* text, const int_t start, const int_t end):
			//termText( stringDuplicate(text) ),
			startOffset (start),
			endOffset (end),
			type ( _T("word") ),
			positionIncrement (1)
		{
#ifndef LUCENE_TOKEN_WORD_LENGTH
			termText = stringDuplicate(text);
#else
			int len = stringLength(text);
			if ( len>LUCENE_TOKEN_WORD_LENGTH ){
				stringNCopy(termText,text,LUCENE_TOKEN_WORD_LENGTH);
				termText[LUCENE_TOKEN_WORD_LENGTH] = 0;
			}else
				stringNCopy(termText,text,(len+1));
#endif
		}

		~Token(){
#ifndef LUCENE_TOKEN_WORD_LENGTH
			  delete[] termText;
#endif
		}

		// Constructs a Token with the given text, start and end offsets, & type.
		Token(const char_t* text, const int_t start, const int_t end, const char_t* typ):
			//termText( stringDuplicate(text) ),
			startOffset (start),
			endOffset (end),
			type ( typ ),
			//type (stringDuplicate(typ) ) //shouldn't need to duplicate type, already in tokenImage
			positionIncrement (1)
		{
#ifndef LUCENE_TOKEN_WORD_LENGTH
			termText = stringDuplicate(text);
#else
			int len = stringLength(text);
			if ( len>LUCENE_TOKEN_WORD_LENGTH ){
				stringNCopy(termText,text,LUCENE_TOKEN_WORD_LENGTH);
				termText[LUCENE_TOKEN_WORD_LENGTH] = 0;
			}else
				stringNCopy(termText,text,(len+1));
#endif
		}

		/* Set the position increment.  This determines the position of this
		* token relative to the previous Token in a TokenStream, used in
		* phrase searching.
		*
		* The default value is 1.
		*
		* Some common uses for this are:
		*
		* - Set it to zero to put multiple terms in the same position.  This is
		* useful if, e.g., a word has multiple stems.  Searches for phrases
		* including either stem will match.  In this case, all but the first stem's
		* increment should be set to zero: the increment of the first instance
		* should be one.  Repeating a token with an increment of zero can also be
		* used to boost the scores of matches on that token.
		*
		* - Set it to values greater than one to inhibit exact phrase matches.
		* If, for example, one does not want phrases to match across removed stop
		* words, then one could build a stop word filter that removes stop words and
		* also sets the increment to the number of stop words removed before each
		* non-stop word.  Then exact phrase queries will only match when the terms
		* occur with no intervening stop words.
		*/
		void setPositionIncrement(int_t posIncr) {
			if (posIncr < 0) {
				_THROWC("positionIncrement must be >= 0");
			}
			positionIncrement = posIncr;
		}

		int_t getPositionIncrement() { return positionIncrement; }

		// Returns the Token's term text.
		const char_t* TermText() { return termText; }

		// Returns this Token's starting offset, the position of the first character
		//	corresponding to this token in the source text.
		//
		//	Note that the difference between endOffset() and startOffset() may not be
		//	equal to termText.length(), as the term text may have been altered by a
		//	stemmer or some other filter.
		int_t StartOffset() const { return startOffset; }

		// Returns this Token's ending offset, one greater than the position of the
		//	last character corresponding to this token in the source text.
		int_t EndOffset() const { return endOffset; }

		// Returns this Token's lexical type.  Defaults to "word".
		const char_t* Type() const { return type; }
	};

	class TokenStream {
	public:
		// Returns the next token in the stream, or null at EOS.
		virtual Token* next() = 0;

		// Releases resources associated with this stream.
		virtual void close() = 0;

		virtual ~TokenStream() { }
	};


	class Analyzer {
	public:
		// Creates a TokenStream which tokenizes all the text in the provided
		//	Reader.  Default implementation forwards to tokenStream(Reader) for
		//	compatibility with older version.  Override to allow Analyzer to choose
		//	strategy based on document and/or field.  Must be able to handle null
		//	field name for backward compatibility.
		virtual TokenStream& tokenStream(const char_t* fieldName, lucene::util::Reader* reader)=0;

		virtual ~Analyzer(){ }
	};


	class Tokenizer:public TokenStream {
		protected:
			// The text source for this Tokenizer.
			lucene::util::Reader* input;

		public:
			Tokenizer::Tokenizer() {
				input = NULL;
			}

			// By default, closes the input Reader.
			virtual void close() {
				if (input != NULL) {
					input->close();
				}
			}

			virtual ~Tokenizer(){ }
	};

	class TokenFilter:public TokenStream {
	protected:
		// The source of tokens for this filter.
		TokenStream* input;
		bool deleteTokenStream;

		TokenFilter(TokenStream* in, bool deleteTS):
			input(in),
			deleteTokenStream(deleteTS)
		{
		}
		virtual ~TokenFilter(){
			if ( deleteTokenStream )
				delete input;
		}

	public:
		// Close the input TokenStream.
		void close() {
			input->close();
		}

	};
}}
#endif
