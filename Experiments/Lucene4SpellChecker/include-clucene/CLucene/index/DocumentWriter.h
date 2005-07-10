#include "CLucene/StdHeader.h"
#ifndef _lucene_index_DocumentWriter_
#define _lucene_index_DocumentWriter_
#ifndef CLUCENE_LITE

#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/document/Document.h"
#include "CLucene/store/Directory.h"
#include "FieldInfos.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/document/Field.h"
#include "TermInfo.h"
#include "CLucene/search/Similarity.h"
#include "TermInfosWriter.h"
#include "FieldsWriter.h"
#include <functional>

using namespace std;
namespace lucene{ namespace index {

	class Posting {				  // info about a Term in a doc
	public:
		int_t *positions;				  // positions it occurs at
		int_t positionsLength;
		Term& term;					  // the Term
		int_t freq;					  // its frequency in doc
		
		int_t getPositionsLength();
		Posting(Term& t, const int_t position);
		~Posting();
	};


	class TermCompare:public binary_function<void*,void*,bool>
	{
	public:
		bool operator()( void* val1, void* val2 ) const{
			Term* t1 = (Term*)val1;
			Term* t2 = (Term*)val2;

			int_t ret = stringCompare(t1->Field(),t2->Field());
			if( ret == 0 )
				return ( stringCompare(t1->Text(),t2->Text()) < 0 );
			else
				return ( ret < 0 );
		}
	};

	class DocumentWriter {
	private:
		lucene::analysis::Analyzer& analyzer;
		lucene::store::Directory& directory;
		FieldInfos* fieldInfos; //array
		const int_t maxFieldLength;

		// Keys are Terms, values are Postings.
		// Used to buffer a document before it is written to the index.
		lucene::util::VoidMap<Term*,Posting*,TermCompare> postingTable;
		int_t *fieldLengths; //array
		Term& termBuffer;

		const static char_t* segmentname(const char_t* segment, const char_t* ext );
		  
	public:
		DocumentWriter(lucene::store::Directory& d, lucene::analysis::Analyzer& a, const int_t mfl);
		~DocumentWriter();

		void addDocument(const char_t* segment, lucene::document::Document& doc);


	private:
		// Tokenizes the fields of a document into Postings.
		void invertDocument(lucene::document::Document& doc);

		void addPosition(const char_t* field, const char_t* text, const int_t position);

		void sortPostingTable(Posting**& Array, int_t& arraySize);

		static void quickSort(Posting**& postings, const int_t lo, const int_t hi);

		void writePostings(Posting** postings, const int_t postingsLength, const char_t* segment);

		void writeNorms(lucene::document::Document& doc, const char_t* segment);

		void clearPostingTable();

		DocumentWriter& operator=( const DocumentWriter& str )
		{
			throw "CAN'T ASSIGN THIS OBJECT";
		}
	};


}}
#endif
#endif
