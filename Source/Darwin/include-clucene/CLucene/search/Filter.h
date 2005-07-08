#ifndef _lucene_search_Filter_
#define _lucene_search_Filter_

#include "CLucene/index/IndexReader.h"
#include "CLucene/util/BitSet.h"

namespace lucene{ namespace search {
  // Abstract base class providing a mechanism to restrict searches to a subset
  // of an index.
  class Filter {
  public:
    virtual ~Filter() {} // Abstract class needs a virtual destructor.

    // Returns a BitSet with true for documents which should be permitted in
    //search results, and false for those that should not.
    virtual lucene::util::BitSet* bits(lucene::index::IndexReader& reader) const=0;
  };
}}
#endif
