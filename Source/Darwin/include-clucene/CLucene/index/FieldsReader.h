#include "CLucene/StdHeader.h"
#ifndef _lucene_index_FieldsReader_
#define _lucene_index_FieldsReader_

#include "CLucene/util/VoidMap.h"
#include "CLucene/store/Directory.h"
#include "CLucene/document/Document.h"
#include "CLucene/document/Field.h"
#include "FieldInfos.h"

namespace lucene{ namespace index {
	class FieldsReader {
	private:
		const FieldInfos& fieldInfos;
		lucene::store::InputStream* fieldsStream;
		lucene::store::InputStream* indexStream;
		int_t size;
		const static char_t* segmentname(const char_t* segment, const char_t* ext );
	public:
		FieldsReader(Directory& d, const char_t* segment, FieldInfos& fn);
		~FieldsReader();

		void close();

		int_t Size();

		lucene::document::Document& doc(int_t n);
	};
}}
#endif
