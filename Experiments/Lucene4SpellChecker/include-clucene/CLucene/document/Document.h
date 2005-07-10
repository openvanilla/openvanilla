#include "CLucene/StdHeader.h"
#ifndef _lucene_document_Document_
#define _lucene_document_Document_

#include "Field.h"

using namespace std;
namespace lucene{ namespace document{ 
	// Documents are the unit of indexing and search.
	//
	// A Document is a set of fields.  Each field has a name and a textual value.
	// A field may be stored with the document, in which case it is returned with
	// search hits on the document.  Thus each document should typically contain
	// stored fields which uniquely identify it.
	class DocumentFieldList {
	public:
		DocumentFieldList(Field* f, DocumentFieldList* n);
		~DocumentFieldList();
		Field* field;
		DocumentFieldList* next;
	};

	class DocumentFieldEnumeration {
	private:
		const DocumentFieldList* fields;
	public:
		DocumentFieldEnumeration(const DocumentFieldList* fl);
		~DocumentFieldEnumeration();
		bool hasMoreElements() const;
		Field* nextElement();
	};

	class Document {
	private:
		DocumentFieldList* fieldList;
	public:
		// Constructs a new document with no fields. 
		Document();
        ~Document();
        
		// Adds a field to a document.  Several fields may be added with
		// the same name.  In this case, if the fields are indexed, their text is
		// treated as though appended for the purposes of search. 
		void add(Field& field);

		// Returns a field with the given name if any exist in this document, or
		//	null.  If multiple fields may exist with this name, this method returns the
		//	last added such added. 
		bool getField(const char_t* name, Field*& retField);

		// Returns the string value of the field with the given name if any exist in
		//	this document, or null.  If multiple fields may exist with this name, this
		//	method returns the last added such added. 
		const char_t* get(const char_t* name);

		// Returns an Enumeration of all the fields in a document. 
		DocumentFieldEnumeration* fields();

		// Prints the fields of a document for human consumption.
		const char_t* toString();
	};
}}
#endif
