#include "CLucene/StdHeader.h"
#ifndef _lucene_search_FilteredTermEnum_
#define _lucene_search_FilteredTermEnum_

#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"

using namespace lucene::index;
namespace lucene{ namespace search {
  //FilteredTermEnum is an abstract class for enumerating a subset of all terms. 
  //
  //Term enumerations are always ordered by term->compareTo().  Each term in
  //the enumeration is greater than all that precede it. 
  
  class FilteredTermEnum: public TermEnum {
    public:
      //Constructor
      FilteredTermEnum(IndexReader& reader, Term* term);
	  //Destructor
      ~FilteredTermEnum();
        
      //Equality measure on the term
      virtual float_t difference() = 0;

      //Returns the docFreq of the current Term in the enumeration.
      int_t DocFreq() const ;
        
      //Increments the enumeration to the next element
      bool next() ;
        
      //Returns the current Term in the enumeration.
      Term* getTerm(const bool pointer=true) ;
        
      //Closes the enumeration to further activity, freeing resources.
      void close();

    protected:
      //Equality compare on the term */
      virtual bool termCompare(Term* term) = 0;
        
      //Indiciates the end of the enumeration has been reached
      virtual bool EndEnum() = 0;
        
      void setEnum(TermEnum* actualEnum) ;
    
    private:
        Term* currentTerm;
        TermEnum* actualEnum;
        

    };
}}
#endif
