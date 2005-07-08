#include "CLucene/StdHeader.h"
#ifndef _lucene_analysis_standard_StandardTokenizer
#define _lucene_analysis_standard_StandardTokenizer

#include "../AnalysisHeader.h"
#include "../Analyzers.h"
#include "StandardTokenizerConstants.h"
#include "CLucene/util/StringBuffer.h"
#include "CLucene/util/FastCharStream.h"
#include "CLucene/util/Reader.h"

using namespace lucene::analysis;
using namespace lucene::util;

namespace lucene{ namespace analysis { namespace standard {

  // This should be a good tokenizer for most European-language documents.
  class StandardTokenizer: public Tokenizer {
  private:
    int_t rdPos;
    int_t tokenStart;

    // Advance by one character, incrementing rdPos and returning the character.
    uchar_t readChar();
    // Retreat by one character, decrementing rdPos.
    void unReadChar();

    // createToken centralizes token creation for auditing purposes.
    Token* createToken(StringBuffer& sb, TokenTypes tokenCode);
    Token* createToken(const char_t* text, int_t tokenLength, TokenTypes tokenCode);

    Token* ReadDotted(StringBuffer& str, TokenTypes forcedType);

  public:
    FastCharStream& rd;

    // Constructs a tokenizer for this Reader.
    StandardTokenizer(Reader& reader);

    ~StandardTokenizer();

    void close();

    // Returns the next token in the stream, or NULL at EOS.
    // The returned token's type is set to an element of
    // StandardTokenizerConstants::tokenImage.
    Token* next();

    // Reads for number like "1"/"1234.567", or IP address like "192.168.1.2".
    Token* ReadNumber(const char_t* previousNumber, const uchar_t prev);

    Token* ReadAlphaNum(const uchar_t prev);

    // Reads for apostrophe-containing word.
    Token* ReadApostrophe(StringBuffer& str);

    // Reads for something@... it may be a COMPANY name or a EMAIL address
    Token* ReadAt(lucene::util::StringBuffer& str);

    // Reads for COMPANY name like AT&T.
    Token* ReadCompany(lucene::util::StringBuffer& str);
  };

}}}
#endif
