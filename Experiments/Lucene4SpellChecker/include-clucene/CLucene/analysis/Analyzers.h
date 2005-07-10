#include "CLucene/StdHeader.h"
#ifndef _lucene_analysis_Analyzers_
#define _lucene_analysis_Analyzers_

#include "CLucene/util/VoidMap.h"
#include "CLucene/util/Reader.h"
#include "AnalysisHeader.h"
#include "CLucene/util/Misc.h"

using namespace lucene::util;
namespace lucene{ namespace analysis {
    class CharTokenizer:public Tokenizer {
	private:
		int_t offset, bufferIndex, dataLen;
		const static int_t MAX_WORD_LEN = 255;
		const static int_t IO_BUFFER_SIZE = 1024;
		uchar_t buffer[MAX_WORD_LEN+1];
		char_t ioBuffer[IO_BUFFER_SIZE+1];
		//Reader* input; input is in tokenizer base class (bug fix thanks to Andy Osipienko)
	protected:
		// Returns true iff a character should be included in a token.  This
		// tokenizer generates as tokens adjacent sequences of characters which
		// satisfy this predicate.  Characters for which this is false are used to
		// define token boundaries and are not included in tokens.
		virtual bool isTokenChar(const uchar_t c) = 0;

		// Called on each token character to normalize it before it is added to the
		// token.  The default implementation does nothing.  Subclasses may use this
		// to, e.g., lowercase tokens. 
		virtual uchar_t normalize(const uchar_t c);
	
	public:
		CharTokenizer(Reader* in);

        virtual ~CharTokenizer(){}
    
		// Returns the next token in the stream, or null at EOS. 
		// *** This is not a pointer. Use of it must deleted.
		Token* next();
	};
	
	
	
	
	class LetterTokenizer:public CharTokenizer {
	public:
		// Construct a new LetterTokenizer. 
		LetterTokenizer(Reader* in):
		  CharTokenizer(in) {}
	
	    ~LetterTokenizer(){}
	protected:
		// Collects only characters which satisfy
		// {@link Character#isLetter(char_t)}.
		bool isTokenChar(const uchar_t c);
	};
	
	
	
	
	
	// LowerCaseTokenizer performs the function of LetterTokenizer
	// and LowerCaseFilter together.  It divides text at non-letters and converts
	// them to lower case.  While it is functionally equivalent to the combination
	// of LetterTokenizer and LowerCaseFilter, there is a performance advantage
	// to doing the two tasks at once, hence this (redundant) implementation.
	// <P>
	// Note: this does a decent job for most European languages, but does a terrible
	// job for some Asian languages, where words are not separated by spaces.
	class LowerCaseTokenizer:public LetterTokenizer {
	public:
		// Construct a new LowerCaseTokenizer. 
		LowerCaseTokenizer(Reader* in):
			LetterTokenizer(in) {}
	
	    ~LowerCaseTokenizer(){}
		void close(){}

	protected:
		// Collects only characters which satisfy
		// {@link Character#isLetter(char_t)}.
		uchar_t normalize(const uchar_t chr);
	};
	
	

	class WhitespaceTokenizer: public CharTokenizer {
	public:
		// Construct a new WhitespaceTokenizer. 
		WhitespaceTokenizer(Reader* in):CharTokenizer(in) {}
		~WhitespaceTokenizer(){}

		void close(){ }
	protected:
		// Collects only characters which do not satisfy
		// {@link Character#isWhitespace(char_t)}.
		bool isTokenChar(const uchar_t c);
	};
	
	
	// An Analyzer that uses WhitespaceTokenizer. 
    class WhitespaceAnalyzer: public Analyzer {
     public:
      TokenStream& tokenStream(const char_t* fieldName, Reader* reader);
	  ~WhitespaceAnalyzer(){}
    };
    
    
    
    
    
    class SimpleAnalyzer: public Analyzer {
	public:
		TokenStream& tokenStream(const char_t* fieldName, Reader* reader);
		~SimpleAnalyzer(){}
	};
	
	
	
	
	class LowerCaseFilter: public TokenFilter {
	public:
		LowerCaseFilter(TokenStream* in, bool deleteTokenStream):TokenFilter(in,deleteTokenStream) {}
		~LowerCaseFilter(){}
		Token* next();
    };
    
    
    
    // Removes stop words from a token stream. 
	class StopFilter: public TokenFilter {
	private:
		VoidMap< char_t*, char_t*>& table;
	public:
		// Constructs a filter which removes words from the input
		//	TokenStream that are named in the array of words. 
		StopFilter(TokenStream* in, bool deleteTokenStream, char_t* stopWords[], int_t stopWordsLength);

		~StopFilter(){}

		// Constructs a filter which removes words from the input
		//	TokenStream that are named in the Hashtable.
		StopFilter(TokenStream* in, bool deleteTokenStream, VoidMap< char_t*, char_t*>& stopTable):
			TokenFilter(in, deleteTokenStream),
			table(stopTable)
		{} 
		  
		// Builds a Hashtable from an array of stop words, appropriate for passing
		//	into the StopFilter constructor.  This permits this table construction to
		//	be cached once when an Analyzer is constructed. 
		static void fillStopTable(VoidMap< char_t*, char_t*>& stopTable,
                                          char_t* stopWords[],
                                          int_t length);

		// Returns the next input Token whose termText() is not a stop word. 
		Token* next();
	};
	
	
	
	
    //An array containing some common English words that are usually not
	//useful for searching.
	
    // Filters LetterTokenizer with LowerCaseFilter and StopFilter. 
    class StopAnalyzer: public Analyzer {
        VoidMap< char_t*, char_t*> stopTable;
    
    public:
        // Builds an analyzer which removes words in ENGLISH_STOP_WORDS. 
        StopAnalyzer();
        
        // Builds an analyzer which removes words in the provided array. 
        StopAnalyzer( char_t* stopWords[], int_t length);
        // Filters LowerCaseTokenizer with StopFilter. 
        TokenStream& tokenStream(const char_t* fieldName, Reader* reader);
		static char_t *ENGLISH_STOP_WORDS[];
		const static int_t ENGLISH_STOP_WORDS_LENGTH = 34;
    };

}}
#endif
