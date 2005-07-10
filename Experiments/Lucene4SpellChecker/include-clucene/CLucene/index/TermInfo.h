#ifndef _lucene_index_TermInfo
#define _lucene_index_TermInfo
#include "CLucene/StdHeader.h"

namespace lucene{ namespace index {

	// A TermInfo is the record of information stored for a term.
	class TermInfo {
	public:
		// The number of documents which contain the term. 
		int_t docFreq;

		//A pointer into the TermFreqs file (.frq)
		//The .frq file contains the lists of documents which contain each term, 
		//along with the frequency of the term in that document.
		long_t freqPointer;

		//A pointer into the TermPosition file (.prx).
		//The .prx file contains the lists of positions that each term 
		//occurs at within documents.
		long_t proxPointer;

        //Constructor
		TermInfo();

        //Constructor
		TermInfo(const int_t df, const long_t fp, const long_t pp);

		//Constructor
		//Initialises this instance by copying the values of another TermInfo ti
		TermInfo(const TermInfo& ti);

        //Destructor
		~TermInfo();

		//Sets a new document frequency, a new freqPointer and a new proxPointer
		void set(const int_t df, const long_t fp, const long_t pp);

		//Sets a new document frequency, a new freqPointer and a new proxPointer
	    //by copying these values from another instance of TermInfo
		void set(const TermInfo& ti);
	};
}}
#endif
