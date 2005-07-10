#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentInfos_
#define _lucene_index_SegmentInfos_

#include "CLucene/util/VoidList.h"
#include "CLucene/store/Directory.h"
#include "SegmentInfo.h"

namespace lucene{ namespace index {

  //SegmentInfos manages a list of SegmentInfo instances
  //Each SegmentInfo contains information about a segment in a directory.
  //
  //The active segments in the index are stored in the segment info file. 
  //An index only has a single file in this format, and it is named "segments". 
  //This lists each segment by name, and also contains the size of each segment.
  //The format of the file segments is defined as follows:
  //
  //                                        SegCount
  //Segments --> SegCount, <SegName, SegSize>
  //
  //SegCount, SegSize --> UInt32
  //
  //SegName --> String
  //
  //SegName is the name of the segment, and is used as the file name prefix 
  //for all of the files that compose the segment's index.
  //
  //SegSize is the number of documents contained in the segment index. 
  //
  //Note:
  //At http://jakarta.apache.org/lucene/docs/fileformats.html the definition
  //of all file formats can be found. Note that java lucene currently 
  //defines Segments as follows:
  //
  //Segments --> Format, Version, SegCount, <SegName, SegSize>SegCount
  //        
  //Format, SegCount, SegSize --> UInt32        
  //      
  //Format and Version have not been implemented yet
  


  class SegmentInfos:public lucene::util::VoidList<SegmentInfo*> {
    public:

      int_t counter;  // used to name new segments

      //Constructor
      SegmentInfos();

      //Constructor
      //This (bool) form of constructor has been added to allow client to instantiate
      //SegmentInfos object that does not delete its members upon its own deletion.  
      //This change was prompted by a bug in IndexWriter::addIndexes.
      SegmentInfos(bool deleteMembers);

	  //Destructor
      ~SegmentInfos();

	  //Returns a reference to the i-th SegmentInfo in the list.
      SegmentInfo& info(int_t i);

	  //Reads segments file that resides in directory
      void read(Directory& directory);

#ifndef CLUCENE_LITE
	  //Writes a new segments file based upon the SegmentInfo instances it manages
      void write(Directory& directory);
#endif
  };
}}
#endif
