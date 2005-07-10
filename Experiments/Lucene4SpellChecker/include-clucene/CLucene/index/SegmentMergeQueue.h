#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentMergeQueue_
#define _lucene_index_SegmentMergeQueue_

#include "CLucene/util/PriorityQueue.h"
#include "SegmentMergeInfo.h"

namespace lucene{ namespace index {
	class SegmentMergeQueue:public lucene::util::PriorityQueue<SegmentMergeInfo*> {
	public:
		//Constructor
	    //Creates a queue of length size
		SegmentMergeQueue(const int_t size);
		
		//Destructor
	    //Does nothing as its parent class will clean up everything
		~SegmentMergeQueue();

		//Closes and destroyes all SegmentMergeInfo Instances in the queue
		void close();
	protected:
		//Overloaded method that implements the lessThan operator for the parent class
        //This method is used by the parent class Priority queue to reorder its internal
        //data structures. This implementation check if stiA is less than the current term of stiB.
		bool lessThan(SegmentMergeInfo* stiA, SegmentMergeInfo* stiB);

	};
}}
#endif
