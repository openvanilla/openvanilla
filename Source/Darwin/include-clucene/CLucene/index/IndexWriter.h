#include "CLucene/StdHeader.h"
#ifndef _lucene_index_IndexWriter_
#define _lucene_index_IndexWriter_
#ifndef CLUCENE_LITE

#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/document/Document.h"
#include "CLucene/util/VoidList.h"
#include "CLucene/store/Lock.h"
#include "CLucene/store/TransactionalRAMDirectory.h"

#include "SegmentHeader.h"

using namespace std;
using namespace lucene::store;
namespace lucene{ namespace index {

	//	An IndexWriter creates and maintains an index.
	//
	//	The third argument to the <a href="#IndexWriter"><b>constructor</b></a>
	//	determines whether a new index is created, or whether an existing index is
	//	opened for the addition of new documents.
	//
	//	In either case, documents are added with the <a
	//	href="#addDocument"><b>addDocument</b></a> method.  When finished adding
	//	documents, <a href="#close"><b>close</b></a> should be called.
	//
	//	If an index will not have more documents added for a while and optimal search
	//	performance is desired, then the <a href="#optimize"><b>optimize</b></a>
	//	method should be called before the index is closed.
	class IndexWriter {
	private:
		// how to analyze text
		lucene::analysis::Analyzer& analyzer;

		TransactionalRAMDirectory* ramDirectory; // for temp segs

		LuceneLock* writeLock;

		DEFINE_MUTEX(THIS_LOCK);

		void _IndexWriter(const bool create);

		void _finalize();

		bool ownDir; //if directory belongs to this object
	public:
		// the segments
		SegmentInfos& segmentInfos;

		// where this index resides
		Directory& directory;

		// Release the write lock, if needed.
		~IndexWriter();

		// If non-NULL, information about merges will be printed to this.
		ostream* infoStream;

		// The Java implementation of Lucene silently truncates any tokenized
		// field if the number of tokens exceeds a certain threshold.  Although
		// that threshold is adjustable, it is easy for the client programmer
		// to be unaware that such a threshold exists, and to become its
		// unwitting victim.
		// CLucene implements a less insidious truncation policy.  Up to
		// DEFAULT_MAX_FIELD_LENGTH tokens, CLucene behaves just as JLucene
		// does.  If the number of tokens exceeds that threshold without any
		// indication of a truncation preference by the client programmer,
		// CLucene raises an exception, prompting the client programmer to
		// explicitly set a truncation policy by adjusting maxFieldLength.
		const static int_t DEFAULT_MAX_FIELD_LENGTH = 10000;
		const static int_t FIELD_TRUNC_POLICY__WARN = -1;
		int_t maxFieldLength;

		// Determines how often segment indexes are merged by addDocument().  With
		// smaller values, less RAM is used while indexing, and searches on
		// unoptimized indexes are faster, but indexing speed is slower.  With larger
		// values more RAM is used while indexing and searches on unoptimized indexes
		// are slower, but indexing is faster.  Thus larger values (> 10) are best
		// for batched index creation, and smaller values (< 10) for indexes that are
		// interactively maintained.
		//
		// <p>This must never be less than 2.  The default value is 10.
		int_t mergeFactor;
		// Determines the largest number of documents ever merged by addDocument().
		// Small values (e.g., less than 10,000) are best for interactive indexing,
		// as this limits the length of pauses while indexing to a few seconds.
		// Larger values are best for batched indexing and speedier searches.
		//
		// <p>The default value is {@link Integer#MAX_VALUE}.
		int_t maxMergeDocs;

		// Constructs an IndexWriter for the index in <code>path</code>.  Text will
		//	be analyzed with <code>a</code>.  If <code>create</code> is true, then a
		//	new, empty index will be created in <code>path</code>, replacing the index
		//	already there, if any.
		IndexWriter(const char_t* path, lucene::analysis::Analyzer& a, const bool create);

		// Constructs an IndexWriter for the index in <code>d</code>.  Text will be
		//	analyzed with <code>a</code>.  If <code>create</code> is true, then a new,
		//	empty index will be created in <code>d</code>, replacing the index already
		//	there, if any.
		IndexWriter(Directory& d, lucene::analysis::Analyzer& a, const bool create);

		DEFINE_MUTEX(close_LOCK);
		// Flushes all changes to an index, closes all associated files, and closes
		//	the directory that the index is stored in.
		// synchronized
		void close(const bool closeDir=true);

		DEFINE_MUTEX(docCount_LOCK);
		// Returns the number of documents currently in this index.
		// synchronized
		int_t docCount();


		// Adds a document to this index.
		void addDocument(lucene::document::Document& doc);

		DEFINE_MUTEX(optimize_LOCK);
		// Merges all segments together into a single segment, optimizing an index
		//	for search.
		// synchronized
		void optimize();


		DEFINE_MUTEX(addIndexes_LOCK);
		// Merges all segments from an array of indexes into this index.
		//
		// <p>This may be used to parallelize batch indexing.  A large document
		// collection can be broken into sub-collections.  Each sub-collection can be
		// indexed in parallel, on a different thread, process or machine.  The
		// complete index can then be created by merging sub-collection indexes
		// with this method.
		//
		// <p>After this completes, the index is optimized.
		// synchronized
		void addIndexes(Directory** dirs, const int_t dirsLength);




		//Some operating systems (e.g. Windows) don't permit a file to be deleted
		//	while it is opened for read (e.g. by another process or thread).  So we
		//	assume that when a delete fails it is because the file is open in another
		//	process, and queue the file for subsequent deletion.
		void deleteSegments(lucene::util::VoidList<SegmentReader*> &segments);
		void deleteFiles(const StringArrayConst& files, Directory& directory);

		void deleteFiles(const StringArrayConst& files, StringArrayConst& deletable);
	private:
		// Merges all RAM-resident segments.
		void flushRamSegments();

		// Incremental segment merger.
		void maybeMergeSegments();

		// Pops segments off of segmentInfos stack down to minSegment, merges them,
		//	and pushes the merged index onto the top of the segmentInfos stack.
		void mergeSegments(const uint_t minSegment);

		StringArrayConst& readDeleteableFiles();
		void writeDeleteableFiles(StringArrayConst& files);


		DEFINE_MUTEX(newSegmentName_LOCK);
		// synchronized
		char_t* newSegmentName();
	};

	class IndexWriterLockWith:public LuceneLockWith{
	public:
		IndexWriter* writer;
		bool create;
		void* doBody();
		IndexWriterLockWith(LuceneLock* lock, IndexWriter* wr, bool cr){
			this->lock = lock;
			this->writer = wr;
			this->create = cr;
		}
	};

	class IndexWriterLockWith2:public LuceneLockWith{
	public:
		lucene::util::VoidList<SegmentReader*>* segmentsToDelete;
		IndexWriter* writer;
		void* doBody();
		IndexWriterLockWith2(LuceneLock* lock, IndexWriter* wr, lucene::util::VoidList<SegmentReader*>* std){
			this->lock = lock;
			this->writer = wr;
			this->segmentsToDelete = std;
		}
	};

}}
#endif
#endif
