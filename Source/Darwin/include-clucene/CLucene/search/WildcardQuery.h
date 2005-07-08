#include "CLucene/StdHeader.h"
#ifndef _lucene_search_WildcardQuery_
#define _lucene_search_WildcardQuery_
#ifndef NO_WILDCARD_QUERY

#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "MultiTermQuery.h"
#include "WildcardTermEnum.h"

using namespace lucene::index;
namespace lucene{ namespace search {

  //Implements the wildcard search query */
  class WildcardQuery: public MultiTermQuery {
    private: 
      Term* wildcardTerm;
    
    public:
      //Constructor
      WildcardQuery(Term* term);
	  //Destructor
      ~WildcardQuery();
	  //Returns the string "WildcardQuery"
      const char_t* getQueryName() const;
   
	  //Prepares the WildcardQuery with IndexReader reader
      void prepare(IndexReader& reader) ;

	  //Returns the query as a human readable string
      const char_t* toString(const char_t* field);
    };

}}
#endif
#endif
