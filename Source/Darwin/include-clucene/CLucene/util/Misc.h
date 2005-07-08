#ifndef _lucene_util_Misc_H
#define _lucene_util_Misc_H

#include "CLucene/StdHeader.h"
#include "CLucene/util/VoidList.h"
#include <sys/stat.h>

namespace lucene{ namespace util{

  class Misc{
  public:
    static ulong_t currentTimeMillis();
    static const char_t* replace_all( const char_t* val, const char_t* srch, const char_t* repl );
    static bool dir_Exists(const char_t* path);

    /* DSR:CL_BUG: Encountered buffer overflow when (e.g.) a text field has a
    ** large value (Field::toString made no attempt to conditionally adjust
    ** maxLen).  Eliminated maxLen in favor of computed total length.  The
    ** join function isn't used in performance-critical places anyway. */
    static char_t* join ( const char_t* a, const char_t* b, const char_t* c=NULL, const char_t* d=NULL,const char_t* e=NULL,const char_t* f=NULL );

    static bool priv_isDotDir( const char_t* name );
    static bool isLetter(int_t c); //replacement for isAlpha. Does the same as java.

#ifdef _UNICODE
        static char* wideToChar(const wchar_t* s);
    static wchar_t* charToWide(const char* s);

    static void charToWide(const char* s, wchar_t* d, size_t len);
    static void wideToChar(const wchar_t* s, char* d, size_t len);
#endif
  };

  template <class parent>class IGCollectable{
  protected:
    bool _canDelete;
  public:
    int_t __refcount;
    IGCollectable(){
      _canDelete=true;
      __refcount = 1;
    }
    virtual ~IGCollectable(){
#ifdef _DEBUG
      if ( __refcount != 0 ){
        _THROWC(  "ERROR: Object collected with non-zero refcount" );
        return;
      }
#endif
    }
    void __addref(){
      __refcount ++;
    }
    //if canDelete is false then don't delete the object - might be a static object
    void finalize(){
      if ( this == NULL )
        return;
      __refcount --;
  #ifdef _DEBUG
      if ( __refcount < 0 ){
        _THROWC( "ERROR: Refcount Sync");
        return;
      }
  #endif
      if ( __refcount == 0 && _canDelete ){
        delete this;
      }
    }
    parent* pointer(){
      __addref();
      return (parent*)this;
    }
  };

}}
#endif
