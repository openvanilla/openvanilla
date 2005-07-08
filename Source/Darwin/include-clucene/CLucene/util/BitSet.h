#ifndef _lucene_util_BitSet_
#define _lucene_util_BitSet_

#include "CLucene/StdHeader.h"

namespace lucene{ namespace util {
  class BitSet {
    int_t size;
    bool *bits;
  public:
    BitSet ( int_t _size ):
      size(_size),
      bits( new bool[_size] )
    {
      for ( int i=0;i<size;i++ )
        bits[i] = false;
    }
    ~BitSet(){
      delete[] bits;
    }

    bool get(const int_t bit) const{
        if ( bit < 0 || bit > size-1 )
          _THROWC( "bit out of range" );
      return bits[bit];
    }
    void set(const int_t bit){
      if ( bit < 0 || bit > size-1 )
        _THROWC( "bit out of range" );
      bits[bit] = true;
    }

    int_t getSize() const {
      return size;
    }
  };
}}
#endif
