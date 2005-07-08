#include "CLucene/StdHeader.h"
#ifndef _lucene_index_FieldInfo_
#define _lucene_index_FieldInfo_

namespace lucene{ namespace index {
    class FieldInfo {
	  public:
	    //name of the field
		char_t*     name;
        //Is field indexed? true = yes false = no
		bool        isIndexed;

		//What does number represent?
		const int_t number;

		FieldInfo(char_t* na, const bool tk, const int_t nu):
		    name(stringDuplicate(na)),isIndexed(tk),number(nu){
        //Func - Constructor
		//       Initialises FieldInfo.
		//       na holds the name of the field
		//       tk indicates whether this field is indexed or not
		//       nu indicates its number
		//Pre  - na != NULL and holds the name of the field
		//       tk is true or false
		//       number >= 0
		//Post - The FieldInfo instance has been created and initialized.
		//       name holds the duplicated string of na
		//       isIndexed = tk
		//       number = nu  
			
	    }
	    
	    ~FieldInfo(){
		//Func - Destructor
		//Pre  - true
		//Post - The instance has been destroyed
			if (name){
				delete [] name;
                }
	    }
	};
}}
#endif
 
