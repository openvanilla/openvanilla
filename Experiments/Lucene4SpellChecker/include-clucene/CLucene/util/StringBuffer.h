#ifndef _lucene_util_StringBuffer_
#define _lucene_util_StringBuffer_

#include "CLucene/StdHeader.h"

namespace lucene{ namespace util {
  class StringBuffer{
  public:
    //Constructor. Allocates a buffer with the default length.
    StringBuffer();
	//Constructor. Allocates a buffer of length initSize + 1
    StringBuffer(const int_t initSize);
	//Constructor. Creates an instance of Stringbuffer containing a copy of 
	//the string value
    StringBuffer(const char_t* value);
    //Destructor
    ~StringBuffer();
	//Clears the Stringbuffer and resets it to it default empty state
    void clear();

    //Appends a single character 
    void append(const uchar_t chr);
	//Appends a copy of the string value 
    void append(const char_t* value);
	//Appends a copy of the string value
    void append(const char_t* value, const int_t appendedLength);
	//Appends an integer (after conversion to a character string)
    void append(const int_t value);
    //Appends a double (after conversion to a character string)
    void append(const double value, const int_t digits);

#ifdef _UNICODE
	//Provides an interface for appending literal characters for instance '&'
    //when compiled with _UNICODE
	void append(const char character);
#endif
	//Puts a copy of the string value in front of the current string in the StringBuffer
    void prepend(const char_t* value);
	//Puts a copy of the string value in front of the current string in the StringBuffer
    void prepend(const char_t* value, const int_t prependedLength);

	//Returns the length of the string in the StringBuffer
    int_t length();
	//Returns a copy of the current string in the StringBuffer sized equal to the length of the string 
    //in the StringBuffer.
    char_t* ToString(); 
	//'\0' terminates the StringBuffer and returns its pointer
    char_t* getBuffer(); 

	//Contains the length of string in the StringBuffer	
	int_t len;

  private:
    //A buffer that contains strings
    char_t* buffer;
	//The length of the buffer
    int_t bufferLength;

    //Has the buffer grown to a minimum length of minLength or bigger
    void growBuffer(const int_t minLength);
	//Has the buffer grown to a minimum length of minLength or bigger and shifts the
    //current string in buffer by skippingNInitialChars forward
    void growBuffer(const int_t minLength, const int_t skippingNInitialChars);

  };
}}
#endif
