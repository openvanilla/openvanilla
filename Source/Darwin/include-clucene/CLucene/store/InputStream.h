#ifndef _lucene_store_InputStream_
#define _lucene_store_InputStream_
#include "CLucene/StdHeader.h"

namespace lucene{ namespace store{

	class InputStream{
	private:
		l_byte_t* buffer; //array of bytes
		char_t* chars; //array of chars
		int_t chars_length;


	protected:
		long_t length;					// set by subclasses
		long_t bufferStart;			  // position in file of buffer
		int_t bufferLength;			  // end of valid l_byte_ts
		int_t bufferPosition;		  // next l_byte_t to read

		InputStream(InputStream& clone);
	public:
		InputStream();
		virtual ~InputStream();

		virtual InputStream& clone()=0;

		// InputStream-like methods @see java.io.InputStream 
		l_byte_t readByte();

		void readBytes(l_byte_t* b, const int_t offset, const int_t len);

		int_t readInt();

		int_t readVInt();

		long_t readLong();

		long_t readVLong();

		char_t* readString(const bool unique);
		char_t* readString();

		void readChars( char_t* buffer, const int_t start, const int_t len);

		virtual void close();
		
		// RandomAccessFile-like methods @see java.io.RandomAccessFile 
		long_t getFilePointer();

		void seek(const long_t pos);
		long_t Length();

	protected:
		void refill();

		virtual void readInternal(l_byte_t* b, const int_t offset, const int_t len) = 0;
		virtual void seekInternal(const long_t pos) = 0;
	};
}}

#endif
