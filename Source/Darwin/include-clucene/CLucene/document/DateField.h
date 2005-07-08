#ifndef _lucene_document_DateField_
#define _lucene_document_DateField_

#include "CLucene/StdHeader.h"

namespace lucene{ namespace document{

  const static char_t* MAX_DATE_STRING = _T("zzzzzzzzz");
  const static char_t* MIN_DATE_STRING = _T("000000000");

  /** Provides support for converting dates to strings and vice-versa.  The
  * strings are structured so that lexicographic sorting orders by date.  This
  * makes them suitable for use as field values and search terms.  */
  class DateField {
  private:
    static void initDS();
  public:
    const static int_t DATE_LEN = 9;
    /* 101559956668415 is the largest integer that, when represented in
    ** base-CHAR_RADIX (base-36), will fit into a 9-character buffer. */
    #ifdef COMPILER_MSVC
      const static long_t MAX_TIME = 101559956668415i64;
    #else
      const static long_t MAX_TIME = 101559956668415LL;
    #endif

    /** Converts a millisecond time to a string suitable for indexing. */
    static char_t* timeToString(const long_t time);

    /** Converts a string-encoded date into a millisecond time. */
    static long_t stringToTime(const char_t* s);
  };
}}

#endif
