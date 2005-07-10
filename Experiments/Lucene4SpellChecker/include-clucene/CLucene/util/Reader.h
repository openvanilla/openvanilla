#ifndef _lucene_util_Reader_
#define _lucene_util_Reader_

#include "CLucene/StdHeader.h"
#include "CLucene/store/FSDirectory.h"

namespace lucene{namespace util{
  class Reader{
  public:
    virtual void close() = 0;
    virtual int_t read(char_t* b, const int_t start, const int_t length) = 0;
    virtual int_t read(char_t* buf) = 0;
    virtual long_t available () = 0;
    virtual uchar_t readChar() = 0;
    virtual uchar_t peek() = 0;
    virtual long_t position() = 0;
    virtual void seek(long_t position) = 0;
    virtual ~Reader(){
    }
  };

  class StringReader:public Reader{
  private:
    const char_t* data;
    int_t pt;
    int_t len;
    bool delVal;
  public:
    StringReader ( const char_t* value );
    StringReader ( const char_t* value, const int length, const bool deletevalue=false );
    ~StringReader();

    int_t read ( char_t* buf );
    int_t read ( char_t* buf, const int_t start, const int_t length );
    long_t available ();
    void close();
    uchar_t readChar();
    uchar_t peek();
    long_t position();
    void seek(long_t position);
  };

    #define FILEREADER_CACHE 64
  class FileReader:public Reader{
  private:
    lucene::store::FSInputStream* stream;
    char_t cache[FILEREADER_CACHE];
    const char_t *cachepos;
  public:

    FileReader ( const char_t* fname );
    ~FileReader();

    int_t read ( char_t* buf );
    int_t read ( char_t* buf, const int_t start, const int_t length );
    long_t available ();
    uchar_t readChar();
    uchar_t peek();
    void close();
    long_t position();
    void seek(long_t position);
  };
}}
#endif
