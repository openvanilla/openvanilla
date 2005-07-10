#include "CLucene/StdHeader.h"
#ifndef _lucene_index_FieldInfos_
#define _lucene_index_FieldInfos_

#include "CLucene/store/Directory.h"
#include "CLucene/document/Document.h"
#include "CLucene/document/Field.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/util/VoidList.h"
#include "FieldInfo.h"

using namespace std;
namespace lucene{ namespace index {
	class FieldInfos {
	private:
		lucene::util::VoidList<FieldInfo*> byNumber;
		lucene::util::VoidMap<const char_t*,FieldInfo*> byName;
	public:
		FieldInfos();
		~FieldInfos();

		FieldInfos(Directory& d, const char_t* name);
	public:
		int_t fieldNumber(const char_t* fieldName)const;


		FieldInfo& fieldInfo(const char_t* fieldName) const;
		const char_t* fieldName(const int_t fieldNumber)const;

		FieldInfo& fieldInfo(const int_t fieldNumber) const;

		int_t size()const;

#ifndef CLUCENE_LITE
		// Adds field info for a Document. 
		void add(lucene::document::Document& doc);

		// Merges in information from another FieldInfos. 
		void add(FieldInfos& other);

		void add( char_t* name, const bool isIndexed);
		void write(lucene::store::Directory& d, const char_t* name);
		void write(lucene::store::OutputStream& output);
#endif

	private:
		void read(lucene::store::InputStream& input);
		void addInternal( char_t* name, const bool dupName,const bool isIndexed);

	};
}}
#endif
