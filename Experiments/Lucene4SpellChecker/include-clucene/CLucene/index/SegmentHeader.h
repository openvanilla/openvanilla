#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentHeader_
#define _lucene_index_SegmentHeader_

#include "SegmentInfo.h"
#include "CLucene/util/BitVector.h"
#include "CLucene/util/VoidMap.h"
#include "Term.h"
#include "FieldInfos.h"
#include "FieldsReader.h"
#include "IndexReader.h"
#include "TermInfosReader.h"

namespace lucene{ namespace index{

	class SegmentTermDocs:public virtual TermDocs {
	protected:
		// SegmentReader parent
		const void* parent;

	private:
		InputStream* freqStream;
		int_t freqCount;
		lucene::util::BitVector* deletedDocs;
	public:
		int_t doc;
		int_t freq;

		//Parent is a segment reader
		SegmentTermDocs( void* Parent);
		virtual ~SegmentTermDocs();

		void seek(Term* term);
		void seek(const TermInfo* ti);
		void close();
		int_t Doc()const;
		int_t Freq()const;

		bool next();

		// Optimized implementation.
		int_t read(int_t docs[], int_t freqs[]);

		// As yet unoptimized implementation.
		bool skipTo(const int_t target);

	protected:
		void skippingDoc();
	};





	class SegmentTermPositions: public virtual SegmentTermDocs,public virtual TermPositions {
	private:
		InputStream* proxStream;
		int_t proxCount;
		int_t position;

	public:
		//parent is a segment reader
		SegmentTermPositions(void* Parent);
		~SegmentTermPositions();

		void seek(TermInfo* ti);

		void close();
		int_t nextPosition();
		bool next();
		int_t read(const int_t docs[], const int_t freqs[]);

		//TODO: The virtual members required in TermPositions are defined in the subclass SegmentTermDocs,
		//but why is there a compiler error because of this.
		void seek(Term* term);
		int_t Doc() const;
		int_t Freq() const;
		int_t read(int_t docs[], int_t freqs[]);
		bool skipTo(const int_t target);

	protected:
		void skippingDoc();


	};



	//The class Norm represents the normalizations for a field.
	//These normailizations are read from an InputStream in into an array of bytes called bytes

	class Norm {
	public:
		InputStream& in;
		l_byte_t* bytes;

		//Constructor
		Norm(InputStream& instrm);
		//Destructor
		~Norm();
	};


	//A SegementReader is an IndexReader responsible for reading 1 segment of an index
	class SegmentReader: public IndexReader{
	private:
		//Iindicates if this SegmentReader is responsible for closing the directory
		bool closeDirectory;

		//Holds the name of the segment that is being read
		const char_t* segment;

		//Indicates if there are documents marked as deleted
		bool   deletedDocsDirty;

		DEFINE_MUTEX(norms_mutex);

		//Holds all norms for all fields in the segment
		lucene::util::VoidMap<const char_t*,Norm*> norms;
	public:
		//Reads the Field Info file
		FieldsReader* fieldsReader;
		//a bitVector that manages which documents have been deleted
		lucene::util::BitVector* deletedDocs;
		//an InputStream to the frequency file
		InputStream* freqStream;
		//For reading the Term Dictionary .tis file
		TermInfosReader* tis;
		//an InputStream to the prox file
		InputStream* proxStream;
		//For reading the fieldInfos file
		FieldInfos* fieldInfos;

		//Constructor. Opens all files of a segment
		SegmentReader(SegmentInfo& si, const bool closeDir=false);
		//Destructor.
		virtual ~SegmentReader();

		//synchronized
		DEFINE_MUTEX(doClose_LOCK);

		//Closes all streams to the files of a single segment
		void doClose();

		//Checks if a segment managed by SegmentInfo si has deletions
		static bool hasDeletions(const SegmentInfo& si);

		//synchronized
		DEFINE_MUTEX(doDelete_LOCK);
		//Marks document docNum as deleted
		void doDelete(const int_t docNum);

		//Returns all file names managed by this SegmentReader
		StringArrayConst& files();
		//Returns an enumeration of all the Terms and TermInfos in the set.
		TermEnum& getTerms() const;
		//Returns an enumeration of terms starting at or after the named term t
		TermEnum& getTerms(const Term* t) const;

		//synchronized
		DEFINE_MUTEX(document_LOCK);
		//todo: make const func
		//Returns a document identified by n
		lucene::document::Document& document(const int_t n);

		//synchronized
		DEFINE_MUTEX(isDeleted_LOCK);

		//todo: make const func
		//Checks if the n-th document has been marked deleted
		bool isDeleted(const int_t n);

		//Returns an unpositioned TermDocs enumerator.
		TermDocs& termDocs() const;
		//Returns an unpositioned TermPositions enumerator.
		TermPositions& termPositions() const;

		//Returns the number of documents which contain the term t
		int_t docFreq(const Term& t) const;

		//Returns the actual number of documents in the segment
		int_t NumDocs();
		//Returns the number of  all the documents in the segment including the ones that have
		//been marked deleted
		int_t MaxDoc() const;
		//Returns the bytes array that holds the norms of a named field
		l_byte_t* getNorms(const char_t* field);
		//Reads the Norms for field from disk starting at offset in the inputstream
		void getNorms(const char_t* field, l_byte_t* bytes, const int_t offset);
		//Returns an inputstream to the norms file for field
		InputStream* normStream(const char_t* field);
		//Creates a filename in buffer by concatenating Segment with ext and x
		static void segmentname(char_t* buffer, const char_t* Segment, const char_t* ext, const int_t x=-1);
		//Returns an allocated buffer in which it creates a filename by
		//concatenating segment with ext and x
		char_t* segmentname(const char_t* ext, const int_t x=-1);
		//Creates a filename in buffer by concatenating segment with ext and x
		void segmentname(char_t* buffer,const char_t* ext, const int_t x=-1 );

	private:
		//Open all norms files for all fields
		void openNorms();
		//Closes all norms files
		void closeNorms();
	};

	class SegmentReaderLockWith:public LuceneLockWith{
	public:
		SegmentReader* reader;

		//Contructor
		SegmentReaderLockWith(LuceneLock* lock, SegmentReader* rdr){
			this->lock = lock;
			this->reader = rdr;
		}

		//Flushes all deleted docs of reader to a new deletions file overwriting the current
		void* doBody();
	};


}}

#endif
