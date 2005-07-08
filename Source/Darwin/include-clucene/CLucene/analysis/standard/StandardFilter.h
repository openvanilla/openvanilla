#include "CLucene/StdHeader.h"
#ifndef _lucene_analysis_standard_StandardFilter
#define _lucene_analysis_standard_StandardFilter

#include "../AnalysisHeader.h"
#include "../Analyzers.h"
#include "StandardTokenizerConstants.h"
#include "CLucene/util/StringBuffer.h"

using namespace std;
using namespace lucene::analysis;
namespace lucene{ namespace analysis { namespace standard {

	// Normalizes tokens extracted with {@link StandardTokenizer}. 
	class StandardFilter: public TokenFilter{
	public:
		// Construct filtering <i>in</i>. 
		StandardFilter(TokenStream* in, bool deleteTokenStream);

		~StandardFilter();

		// Returns the next token in the stream, or NULL at EOS.
		// <p>Removes <tt>'s</tt> from the end of words.
		// <p>Removes dots from acronyms.
		Token* next();
	};
}}}
#endif
