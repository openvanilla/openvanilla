#include "CLucene/StdHeader.h"
#ifndef _lucene_index_FieldsWriter_
#define _lucene_index_FieldsWriter_
#ifndef CLUCENE_LITE

#include "CLucene/util/VoidMap.h"
#include "CLucene/store/Directory.h"
#include "CLucene/store/OutputStream.h"
#include "CLucene/document/Document.h"
#include "CLucene/document/Field.h"
#include "FieldInfos.h"

namespace lucene{ namespace index {
	class FieldsWriter {
	private:
		FieldInfos& fieldInfos;
		OutputStream* fieldsStream;
		OutputStream* indexStream;

		const static char_t* segmentname( const char_t* segment, const char_t* ext );
	public:
		FieldsWriter(Directory& d, const char_t* segment, FieldInfos& fn);
		~FieldsWriter();

		void close();

		void addDocument(lucene::document::Document& doc);
	};
}}
#endif
#endif
