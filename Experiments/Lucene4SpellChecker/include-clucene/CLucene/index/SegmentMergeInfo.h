#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentMergeInfo_
#define _lucene_index_SegmentMergeInfo_

#include "CLucene/util/BitVector.h"
#include "SegmentTermEnum.h"
#include "SegmentHeader.h"

namespace lucene{ namespace index {
	class SegmentMergeInfo {
	public:
		SegmentTermEnum& termEnum;
		Term* term;
		int_t base;
		int_t* docMap;				  // maps around deleted docs
		SegmentReader& reader;
		SegmentTermPositions& postings;
         
		//Constructor
		SegmentMergeInfo(const int_t b, SegmentTermEnum& te, SegmentReader& r);

		//Destructor
		~SegmentMergeInfo();

		//Moves the current term of the enumeration termEnum to the next and term
	    //points to this new current term
		bool next();

		//Closes the the resources
		void close();
 
	};
}}
#endif

