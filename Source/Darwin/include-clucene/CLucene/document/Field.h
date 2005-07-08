#include "CLucene/StdHeader.h"
#ifndef _lucene_document_Field_
#define _lucene_document_Field_

#include "CLucene/util/Reader.h"

namespace lucene{ namespace document{ 
    //	A field is a section of a Document.  Each field has two parts, a name and a
    //	value.  Values may be free text, provided as a String or as a Reader, or they
    //	may be atomic keywords, which are not further processed.  Such keywords may
    //	be used to represent dates, urls, etc.  Fields are optionally stored in the
    //	index, so that they may be returned with hits on the document.
	class Field {
	private:
		char_t* name;
		char_t* stringValue;
		lucene::util::Reader* readerValue;
		bool isStored;
		bool isIndexed;
		bool isTokenized;
	public:
		Field(const char_t* Name, const char_t* String, bool store, bool index, bool token);
		Field(const char_t* Name, lucene::util::Reader* reader, bool store, bool index, bool token);
		Field(const char_t* Name, lucene::util::Reader* reader);

        ~Field();
        
		// Constructs a String-valued Field that is not tokenized, but is indexed
		//	and stored.  Useful for non-text fields, e.g. date or url.
		static Field& Keyword(char_t* Name, const char_t* Value);

		// Constructs a String-valued Field that is not tokenized nor indexed,
		//	but is stored in the index, for return with hits.
		static Field& UnIndexed(char_t* Name, const char_t* Value);

		//  Constructs a String-valued Field that is tokenized and indexed,
		//	and is stored in the index, for return with hits.  Useful for short text
		//	fields, like "title" or "subject". 
		static Field& Text(char_t* Name, const char_t* Value);

		//  Constructs a String-valued Field that is tokenized and indexed,
		//	but that is not stored in the index.
		static Field& UnStored(char_t* Name, const char_t* Value);

		//  Constructs a Reader-valued Field that is tokenized and indexed, but is
		//	not stored in the index verbatim.  Useful for longer text fields, like
		//	"body".
		static Field& Text(char_t* Name, lucene::util::Reader* Value);

		//  The name of the field (e.g., "date", "subject", "title", "body", etc.)
		//	as an interned string. 
		char_t* Name();

		// The value of the field as a String, or null.  If null, the Reader value
		//	is used.  Exactly one of stringValue() and readerValue() must be set. 
		const char_t* StringValue();

		//  The value of the field as a Reader, or null.  If null, the String value
		//	is used.  Exactly one of stringValue() and readerValue() must be set. 
		lucene::util::Reader* ReaderValue();

		//  True iff the value of the field is to be stored in the index for return
		//	with search hits.  It is an error for this to be true if a field is
		//	Reader-valued. 
		bool IsStored();

		//  True iff the value of the field is to be indexed, so that it may be
		//	searched on. 
		bool IsIndexed();

		// True iff the value of the field should be tokenized as text prior to
		//	indexing.  Un-tokenized fields are indexed as a single word and may not be
		//	Reader-valued.
		bool IsTokenized();

		// Prints a Field for human consumption.
		char_t* toString();

	};
}}
#endif
