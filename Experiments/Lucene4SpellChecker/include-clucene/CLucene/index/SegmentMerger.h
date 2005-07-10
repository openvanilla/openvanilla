#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentMerger_
#define _lucene_index_SegmentMerger_

#include "CLucene/store/Directory.h"
#include "CLucene/document/Document.h"
#include "CLucene/util/BitVector.h"
#include "CLucene/util/VoidList.h"
#include "SegmentMergeInfo.h"
#include "SegmentMergeQueue.h"
#include "FieldInfos.h"
#include "FieldsWriter.h"
#include "TermInfosWriter.h"

namespace lucene{ namespace index {
	class SegmentMerger {
	private:
		//Directory of the segment
		Directory& directory;     
		//name of the new segment
		const char_t* segment;
		//Set of SegmentReaders
		lucene::util::VoidList<SegmentReader*> readers;
		//Field Infos for the FieldInfo instances of all fields
		FieldInfos* fieldInfos;

		//The queue that holds SegmentMergeInfo instances
		SegmentMergeQueue* queue;
		//OutputStream to the new Frequency File
		lucene::store::OutputStream* freqOutput;
		//OutputStream to the new Prox File
		lucene::store::OutputStream* proxOutput;
		//Writes Terminfos that have been merged
		TermInfosWriter* termInfosWriter;
		TermInfo termInfo; //(new) minimize consing
	public:		
		//Constructor
		SegmentMerger(Directory& dir, const char_t* name);

		//Destructor
		~SegmentMerger();

		//Adds a SegmentReader to the set of readers
		void add(SegmentReader& reader);

		//Returns a reference to the i-th SegmentReader
		SegmentReader& segmentReader(const int_t i);

		//Merge all the segments managed by the SegmentReaders in readers
		void merge();

	private:
        //Returns an allocated buffer in which it creates a filename by 
        //concatenating segment with ext and x
		const char_t* segmentname(const char_t* ext, const int_t x=-1);

		//Merge the fields of all segments 
		void mergeFields();

		//Merge the terms of all segments
		void mergeTerms();

		//Merges all TermInfos into a single segment
		void mergeTermInfos();

        //Merge the TermInfo of a term found in one or more segments.
		void mergeTermInfo( SegmentMergeInfo** smis, const int_t n);
		    
        //Process postings from multiple segments all positioned on the
        //same term. Writes out merged entries into freqOutput and
        //the proxOutput streams.
		int_t appendPostings(SegmentMergeInfo** smis, const int_t n);

		//Merges the norms for all fields 
		void mergeNorms();
	};
}}
#endif
