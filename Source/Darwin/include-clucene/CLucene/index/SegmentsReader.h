#include "CLucene/StdHeader.h"
#ifndef _lucene_index_SegmentsReader
#define _lucene_index_SegmentsReader

#include "SegmentHeader.h"

#include "SegmentInfo.h"
#include "IndexReader.h"
#include "CLucene/document/Document.h"
#include "Terms.h"
#include "SegmentMergeQueue.h"

namespace lucene{ namespace index{

	class SegmentsTermDocs:public TermDocs {
	protected:
		SegmentTermDocs** segTermDocs;

		SegmentReader** readers;
		int_t readersLength;
		const int_t* starts;
		Term* term;

		int_t base;
		int_t pointer;

		SegmentTermDocs* current;              // == segTermDocs[pointer]
	public:
		SegmentsTermDocs();
		SegmentsTermDocs(SegmentReader** r, int_t rLen, const int_t* s);
		~SegmentsTermDocs();

		int_t Doc() const;
		int_t Freq() const;

		void seek(Term* tterm);

		bool next();

		// Optimized implementation.
		int_t read(int_t docs[], int_t freqs[]);

		// As yet unoptimized implementation.
		bool skipTo(const int_t target);

		void close();


	protected:
		virtual SegmentTermDocs* termDocs(const SegmentReader& reader) const;
	private:
		SegmentTermDocs* termDocs(const int_t i) const;

	};


	//SegmentsTermEnum represents the enumeration of all terms of all readers

	class SegmentsTermEnum: public virtual TermEnum {
	private:
		SegmentMergeQueue* queue;

		Term* term;
		int_t docFreq;
	public:
		//Constructor
		//Opens all enumerations of all readers
		SegmentsTermEnum(SegmentReader** readers, const int_t readersLength, const int_t starts[], const Term* t);

		//Destructor
		~SegmentsTermEnum();

		//Move the current term to the next in the set of enumerations
		bool next();

		//Returns the current term of the set of enumerations
		Term* getTerm(const bool pointer = true);

		//Returns the document frequency of the current term in the set
		int_t DocFreq() const;

		//Closes the set of enumerations in the queue
		void close();
	};


	class SegmentsTermPositions:public virtual SegmentsTermDocs,public virtual TermPositions {
	public:
		SegmentsTermPositions(SegmentReader** r, const int_t rLen, const int_t* s);
		~SegmentsTermPositions() {};
		int_t nextPosition();

		//TODO: The virtual members required in TermPositions are defined in the subclass SegmentTermDocs,
		//but why is there a compiler error because of this.

		void seek(Term* term);
		int_t Doc() const;
		int_t Freq() const;

		int_t read(int_t docs[], int_t freqs[]);
		bool skipTo(const int_t target);
		bool next();
		void close();

	protected:
		SegmentTermDocs* termDocs(const SegmentReader& reader) const;
	};


	class SegmentsReader:public IndexReader{
	protected:
		SegmentReader** readers;
		int_t readersLength;
		int_t* starts;			  // 1st docno for each segment

	private:
		lucene::util::VoidMap<const char_t*,l_byte_t*> normsCache;
		int_t maxDoc;
		int_t numDocs;

	public:
		bool closeDir;
		SegmentsReader(Directory& directory, SegmentReader** r, int_t rLen);
		~SegmentsReader();

		DEFINE_MUTEX(NumDocs_LOCK);
		// synchronized
		int_t NumDocs();

		int_t MaxDoc() const;

		lucene::document::Document& document(const int_t n);

		bool isDeleted(const int_t n);

		DEFINE_MUTEX(getNorms_LOCK);
		// synchronized
		l_byte_t* getNorms(const char_t* field);

		TermEnum& getTerms() const;

		TermEnum& getTerms(const Term* term) const;

		int_t docFreq(const Term& t) const;

		TermDocs& termDocs() const;

		TermPositions& termPositions() const;

	private:
		DEFINE_MUTEX(doDelete_LOCK);
		// synchronized
		void doDelete(const int_t n);

		int_t readerIndex(const int_t n) const;

		DEFINE_MUTEX(doClose_LOCK);
		// synchronized
		void doClose();
	};


}}
#endif
