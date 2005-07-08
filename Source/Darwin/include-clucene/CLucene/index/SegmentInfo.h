#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentInfo_
#define _lucene_index_SegmentInfo_

#include "CLucene/store/Directory.h"

namespace lucene{ namespace index{
	class SegmentInfo {
	public:
        //Unique name in directory dir
		const char_t* name;	
		//Number of docs in the segment
		const int_t docCount;				  
		//Directory where the segment resides
		lucene::store::Directory& dir;				  

		SegmentInfo(const char_t* Name, const int_t DocCount, lucene::store::Directory& Dir):
		    name(stringDuplicate(Name)),docCount(DocCount),dir(Dir){
		//Func - Constructor. Initialises SegmentInfo.
		//Pre  - Name holds the unique name in the directory Dir
		//       DocCount holds the number of documents in the segment
		//       Dir holds the Directory where the segment resides
		//Post - The instance has been created. name contains the duplicated string Name.
		//       docCount = DocCount and dir references Dir
	    }

		~SegmentInfo(){
		//Func - Destructor
		//Pre  - true
        //Post - The instance of SegmentInfo has been destroyed
			if (name){
			    delete[] name;
                }
		}
	};
}}
#endif
