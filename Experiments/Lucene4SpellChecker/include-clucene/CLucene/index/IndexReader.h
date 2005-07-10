#ifndef _lucene_index_IndexReader_
#define _lucene_index_IndexReader_
#include "CLucene/StdHeader.h"

#include "CLucene/store/Directory.h"
#include "CLucene/store/FSDirectory.h"
#include "CLucene/store/Lock.h"
#include "CLucene/document/Document.h"
#include "SegmentInfos.h"
#include <sys/stat.h>
#include "Terms.h"

namespace lucene{ namespace index {
    //IndexReader is an abstract class LUCENE_EXPORT, providing an interface for accessing an
    //index.  Search of an index is done entirely through this abstract interface,
    //so that any subclass which implements it is searchable.
    //
    //Concrete subclasses of IndexReader are usually constructed with a call to
    //the static method open.
    //
    //For efficiency, in this API documents are often referred to via
    //document numbers, non-negative integers which each name a unique
    //document in the index.  These document numbers are ephemeral--they may change
    //as documents are added to and deleted from an index.  Clients should thus not
    //rely on a given document having the same number between sessions. 
	class IndexReader {
	private:
		lucene::store::LuceneLock* writeLock;
	protected:
		IndexReader(Directory& dir);
		
		virtual void doDelete(const int_t docNum) = 0;

		// Implements close. 
		virtual void doClose() = 0;

	public:
		Directory& directory;

		// Release the write lock, if needed. 
		virtual ~IndexReader();

		// Returns an IndexReader reading the index in an FSDirectory in the named path. 
		static IndexReader& open(const char_t* path, const bool closeDir=true);

		// Returns an IndexReader reading the index in the given Directory. 
		static IndexReader& open( Directory& directory, const bool closeDir=true);

		// Returns the time the index in the named directory was last modified. 
		static long_t lastModified(const char_t* directory);

		// Returns the time the index in this directory was last modified. 
		static long_t lastModified(const Directory& directory);
		
		//Checks if an index exists in the named directory
		static bool indexExists(const char_t* directory);

        //Checks if an index exists in the directory
		static bool indexExists(const Directory& directory);

		//Returns the number of documents in this index. 
		virtual int_t NumDocs() = 0;

		//Returns one greater than the largest possible document number.
		//This may be used to, e.g., determine how big to allocate an array which
		//will have an element for every document number in an index.
		virtual int_t MaxDoc() const = 0;

		//Returns the stored fields of the n-th Document in this index. 
		virtual lucene::document::Document& document(const int_t n) =0;

		//Returns true if document n has been deleted 
		virtual bool isDeleted(const int_t n) = 0;

		//Returns the byte-encoded normalization factor for the named field of
		//every document.  This is used by the search code to score documents.
		//see org.apache.lucene.search.Similarity#norm
		virtual l_byte_t* getNorms(const char_t* field) = 0;//array

		//Returns an enumeration of all the terms in the index.
		//The enumeration is ordered by Term.compareTo().  Each term
		//is greater than all that precede it in the enumeration.
		virtual TermEnum& getTerms() const =0;

		//Returns an enumeration of all terms after a given term.
		//The enumeration is ordered by Term.compareTo().  Each term
		//is greater than all that precede it in the enumeration.
		virtual TermEnum& getTerms(const Term* t) const = 0;

		//Returns the number of documents containing the term t. 
		virtual int_t docFreq(const Term& t) const = 0;

		//Returns an enumeration of all the documents which contain term. 
		TermDocs& termDocs(Term* term) const;

        //Returns an enumeration of all the documents which contain  term. For each 
        //document, in addition to the document number and frequency of the term in 
        //that document, a list of all of the ordinal positions of the term in the document 
        //is available.		
		TermPositions& termPositions(Term* term);

		// Returns an unpositioned TermPositions enumerator. 
		virtual TermPositions& termPositions() const = 0;

		// Returns an unpositioned TermDocs enumerator. 
		virtual TermDocs& termDocs() const = 0;

#ifndef CLUCENE_LITE
        DEFINE_MUTEX(Delete_LOCK);
		//Deletes the document numbered docNum.  Once a document is deleted it will not appear 
        //in TermDocs or TermPostitions enumerations. Attempts to read its field with the document 
        //method will result in an error.  The presence of this document may still be reflected in 
        //the docFreq statistic, though this will be corrected eventually as the index is further modified.  

		void Delete(const int_t docNum);

		//Deletes all documents containing term. Returns the number of deleted documents
		int_t Delete(Term* term);
#endif //CLUCENE_LITE

        DEFINE_MUTEX(close_LOCK);
		//Closes files associated with this index and also saves any new deletions to disk.
        //No other methods should be called after this has been called.
		void close();

        //Checks if the index in the named directory is currently locked.       
        static bool isLocked(const Directory& directory);

        //Checks if the index in the named directory is currently locked.       
		static bool isLocked(const char_t* directory);


		//Forcibly unlocks the index in the named directory.
		//Caution: this should only be used by failure recovery code,
		//when it is known that no other process nor thread is in fact
		//currently accessing this index.
		static void unlock(Directory& directory);
	};

	class IndexReaderLockWith:public LuceneLockWith{
	public:
		Directory* directory;
		bool       closeDir;

        //Constructor	
        IndexReaderLockWith(LuceneLock* lock, Directory* dir, bool cd){
            this->lock = lock;
            this->directory = dir;
            this->closeDir = cd;
        }


        //Reads the segmentinfo file and depending on the number of segments found
        //it returns a SegmentsReader or a SegmentReader
		void* doBody();

	};
}}

#endif


