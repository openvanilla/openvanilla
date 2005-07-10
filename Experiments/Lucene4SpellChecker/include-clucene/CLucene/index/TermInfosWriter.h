#include "CLucene/StdHeader.h"
#ifndef _lucene_index_TermInfosWriter_
#define _lucene_index_TermInfosWriter_
#ifndef CLUCENE_LITE

#include "CLucene/store/Directory.h"
#include "FieldInfos.h"
#include "TermInfo.h"
#include "Term.h"

namespace lucene{ namespace index {


	// This stores a monotonically increasing set of <Term, TermInfo> pairs in a
	// Directory.  A TermInfos can be written once, in order.  
	class TermInfosWriter {
	private:
		FieldInfos& fieldInfos;
		OutputStream* output;
		Term* lastTerm;
		TermInfo* lastTi;
		int_t size;
		long_t lastIndexPointer;
		bool isIndex;
		TermInfosWriter* other;
	
		const static char_t* segmentname( const char_t* segment, const char_t* ext );

	public:
	    //Constructor
		TermInfosWriter(Directory& directory, const char_t* segment, FieldInfos& fis);
		~TermInfosWriter();


		// Adds a new <Term, TermInfo> pair to the set.
		//	Term must be lexicographically greater than all previous Terms added.
		//	TermInfo pointers must be positive and greater than all previous.
		void add(Term& term, const TermInfo& ti);

		// Called to complete TermInfos creation. 
		void close();

	private:
        //Constructor
		TermInfosWriter(Directory& directory, const char_t* segment, FieldInfos& fis, bool isIndex);

        //Helps constructors to initialize instances
        void _TermInfosWriter(Directory& directory, const char_t* segment, bool IsIndex);


		void writeTerm(Term& term);
		static int_t stringDifference(const char_t* s1, const char_t* s2);
	};
}}
#endif
#endif
