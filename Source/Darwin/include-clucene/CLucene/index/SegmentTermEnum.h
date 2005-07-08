#ifndef _lucene_index_SegmentTermEnum_
#define _lucene_index_SegmentTermEnum_
#include "CLucene/StdHeader.h"

#include "Terms.h"
#include "FieldInfos.h"
#include "TermInfo.h"

namespace lucene{ namespace index {

    //SegmentTermEnum is an enumeration of all Terms and TermInfos

	class SegmentTermEnum:public TermEnum{
	private:
		FieldInfos& fieldInfos; //contains the Field Infos for the segment

		Term* term;             //points to the current Term in the enumeration
		TermInfo* termInfo;     //points to the TermInfo matching the current Term in the enumeration

		bool isIndex;           //Indicates if the Segment is a an index

		char_t* buffer;         //The buffer that contains the data read from the Term Infos File
	
		bool isClone;           //Indicates if SegmentTermEnum is an orignal instance or
		                        //a clone of another SegmentTermEnum
		int_t bufferLength;     //Length of the buffer

	public:
		InputStream& input;     //The InputStream that reads from the Term Infos File
		int_t size;             //The size of the enumeration
		int_t position;         //The position of the current (term) in the enumeration
		long_t indexPointer;    
		Term* prev;             //The previous current 

		//Constructor
		SegmentTermEnum(InputStream& i, FieldInfos& fis, const bool isi );

		//Constructor. 
		//The instance is created by cloning all properties of clone
		SegmentTermEnum( SegmentTermEnum& );
		
		//Destructor
		~SegmentTermEnum();

		//Moves the current of the set to the next in the set
		bool next();

		//Returns the current term. 
		Term* getTerm(const bool pointer = true);

        //Scan for Term Scratch without allocating new Terms
		void scanTo(const Term *Scratch);

		//Closes the enumeration to further activity, freeing resources. 
		void close();

		//Returns the document frequency of the current term in the set
		int_t DocFreq() const;

		//Repositions term and termInfo within the enumeration
		void seek(const long_t pointer, const int_t p, Term& t, TermInfo& ti);
		
		//Returns a clone of the current termInfo
		TermInfo* getTermInfo()const;

		//Retrieves a clone of termInfo through the reference ti
		void getTermInfo(TermInfo& ti)const;

		// Returns the freqPointer from the current TermInfo in the enumeration.
		long_t freqPointer()const;

		// Returns the proxPointer from the current TermInfo in the enumeration.
		long_t proxPointer()const;

        //Returns a clone of this instance
		SegmentTermEnum* clone();

    private:
		//Reads the next term in the enumeration
		Term* readTerm();
        //Instantiate a buffer of length length+1
		void growBuffer(const int_t length);

	};
}}

#endif
