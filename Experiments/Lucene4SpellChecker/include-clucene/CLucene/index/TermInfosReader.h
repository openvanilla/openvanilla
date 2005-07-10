#include "CLucene/StdHeader.h"
#ifndef _lucene_index_TermInfosReader_
#define _lucene_index_TermInfosReader_

#include "Terms.h"
#include "SegmentTermEnum.h"
#include "CLucene/store/Directory.h"
#include "FieldInfos.h"
#include "TermInfo.h"
#include "TermInfosWriter.h"

namespace lucene{ namespace index {
	class TermInfosReader{
	private:
		lucene::store::Directory& directory;
		const char_t* segment;
		FieldInfos& fieldInfos;

		SegmentTermEnum* _enum;
		int_t size;

		Term** indexTerms;
		TermInfo** indexInfos;
		long_t* indexPointers;
		int_t indexTermsLength;

	public:
		//Constructor.
        //Reads the TermInfos file (.tis) and eventually the Term Info Index file (.tii)
		TermInfosReader(Directory& dir, const char_t* seg, FieldInfos& fis);
		//Destructor
		~TermInfosReader();
		//Close the enumeration of TermInfos
		void close();
		
		//Return the size of the enumeration of TermInfos
		int_t Size();

        DEFINE_MUTEX(getInt_LOCK);
		// Returns the nth term in the set. 
		// synchronized
		Term* get(const int_t position);

        DEFINE_MUTEX(getTerm_LOCK);
		// Returns the TermInfo for a Term in the set
		// synchronized
		TermInfo* get(const Term& term);
		
        DEFINE_MUTEX(getPosition_LOCK);
		// Returns the position of a Term in the set
		// synchronized 
		int_t getPosition(const Term& term);

        DEFINE_MUTEX(getTerms_LOCK);
		//Returns an enumeration of all the Terms and TermInfos in the set
		// synchronized 
		SegmentTermEnum& getTerms();

        DEFINE_MUTEX(getTermsTerm_LOCK);
		// Returns an enumeration of terms starting at or after the named term. 
		// synchronized 
		SegmentTermEnum& getTerms(const Term& term);
	private:
		//Reads the term info index file or .tti file.
		void readIndex();

		// Returns the offset of the greatest index entry which is less than term.
		int_t getIndexOffset(const Term& term);

		//Reposition the current Term and TermInfo to indexOffset
		void seekEnum(const int_t indexOffset);  

		//Scans the Enumeration of terms for term and returns the corresponding TermInfo instance if found.
        //The search is started from the current term.
		TermInfo* scanEnum(const Term& term);

        //Scans the enumeration to the requested position and returns the
        //Term located at that position
		Term* scanEnum(const int_t position);

		//Return a filename by concatenating segment with ext
        const char_t* segmentname( const char_t* ext );
	};
}}
#endif
