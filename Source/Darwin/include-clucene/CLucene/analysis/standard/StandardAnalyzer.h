#include "CLucene/StdHeader.h"
#ifndef _lucene_analysis_standard_StandardAnalyzer
#define _lucene_analysis_standard_StandardAnalyzer

#include "CLucene/util/VoidMap.h"
#include "CLucene/util/Reader.h"
#include "../AnalysisHeader.h"
#include "../Analyzers.h"
#include "StandardFilter.h"
#include "StandardTokenizer.h"

using namespace lucene::util;
using namespace lucene::analysis;

namespace lucene{ namespace analysis { namespace standard {
	//An array containing some common English words that are usually not
	//useful for searching.
	static char_t* STOP_WORDS [] = 
	{
        _T("a"), _T("and"), _T("are"), _T("as"), _T("at"), _T("be"), _T("but"), _T("by"),
		_T("for"), _T("if"), _T("in"), _T("into"), _T("is"), _T("it"),
		_T("no"), _T("not"), _T("of"), _T("on"), _T("or"), _T("s"), _T("such"),
		_T("t"), _T("that"), _T("the"), _T("their"), _T("then"), _T("there"), _T("these"),
		_T("they"), _T("this"), _T("to"), _T("was"), _T("will"), _T("with")
	};
	static int_t STOP_WORDS_LENGTH = 34;




	//Represents a standard analyzer.
	class StandardAnalyzer : public Analyzer 
	{
	private:
		VoidMap< char_t*, char_t*> stopTable;
	public:
		// <summary> Builds an analyzer. </summary>
		StandardAnalyzer();

		//<summary> Builds an analyzer with the given stop words. </summary>
		StandardAnalyzer( char_t* stopWords[], int_t stopWordsLength);

		~StandardAnalyzer();


		// <summary>
		// Constructs a StandardTokenizer filtered by a 
		// StandardFilter, a LowerCaseFilter and a StopFilter.
		// </summary>
		TokenStream& tokenStream(const char_t* fieldName, Reader* reader) 
		;
	};
}}}
#endif
