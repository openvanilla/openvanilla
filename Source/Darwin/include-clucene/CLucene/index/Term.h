#include "CLucene/StdHeader.h"
#ifndef _lucene_index_Term_
#define _lucene_index_Term_

#include "CLucene/util/Misc.h"

namespace lucene{ namespace index{

/*
	A Term represents a word from text.  This is the unit of search.  It is
    composed of two elements, the text of the word, as a string, and the name of
    the field that the text occured in, an interned string.

	Note that terms may represent more than words from text fields, but also
    things like dates, email addresses, urls, etc.  

	IMPORTANT NOTE:
    Term inherits from the template class IGCollectable<Term> which tries to do
	some garbage collection by counting the references an instance has. As a result
	of this construction you MUST USE THE finalize() method of the parent class IGCollectable<Term>
	when you want to delete an instance of Term!	


    ABOUT intrn 

	intrn indicates if field and text are interned or not. Interning of Strings is the process of
	converting duplicated strings to shared ones. In Java there is some kind
	of stringpool that manages all the references to interned strings. In CLucene we don't
	have such an mechanism. Therefor one must understand that all that intrn does is telling if
	Term is the owner of field and text or it is not.

	intrn = false -> Term is the owner
	intrn = true  -> Term is not the owner.

	This is needed when Term is destroyed to check if field and text must be deleted or not.

*/

	class Term: public lucene::util::IGCollectable<Term> {
	  private:
		char_t* field;
		char_t* text;
		bool    intrn;    //Indicates if Term is interned. 

	  public:
        //Constructor. Constructs a Term with the given field and text.
		//Field and text are copied.
        Term(const char_t* fld, const char_t* txt);
		
        //Constructor. Constructs a Term with the given field and text. Field and text are not copied
	    //Field and text are deleted in destructor only if intern is false. 
		Term(char_t* fld, char_t* txt, const bool intern, const bool canDelete=true);
		
		//Destructor.
        ~Term();

		//Returns the field of this term, an interned string. The field indicates
        //the part of a document which this term came from. 
		const char_t* Field() const;

        //Returns the text of this term.  In the case of words, this is simply the
        //text of the word.  In the case of dates and other types, this is an
        //encoding of the object as a string.
		const char_t* Text() const;

        //Compares two terms, returning true if they have the same
        //field and text. 
		bool equals(const Term* o) const;

		//Resets the field and text of a Term.
		void set(char_t* fld, char_t* txt, const bool intern);

        //Compares two terms, to see if this term belongs before,is equal to or after
        //after the argument term.
		int_t compareTo(const Term& other) const;

        //Forms the contents of Field and term in some kind of tuple notation
        //<field:text>
		char_t* toString() const;	  
	};
}}
#endif
