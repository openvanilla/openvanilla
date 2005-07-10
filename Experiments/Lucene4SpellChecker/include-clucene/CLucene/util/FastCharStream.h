#ifndef _lucene_util_FastCharStream_
#define _lucene_util_FastCharStream_

#include "CLucene/StdHeader.h"
#include "CLucene/util/Arrays.h"
#include "CLucene/util/Reader.h"

namespace lucene{ namespace util {

	//Ported implementation of the FastCharStream class LUCENE_EXPORT.
	class FastCharStream
	{
		int_t col;
		int_t line;
	
	public:
		Reader& input;

		//Initializes a new instance of the FastCharStream class LUCENE_EXPORT.
		FastCharStream(Reader& reader);
		
		//Returns the next char_t from the stream.
		uchar_t GetNext();

		void UnGet();
		
		//Returns the current top char_t from the input stream without removing it.
		uchar_t Peek();
		
		
		//Returns <b>True</b> if the end of stream was reached.
		bool Eos()	;

		// Gets the current column.
		int_t Column();

		// Gets the current line.
		int_t Line();
	};
}}
#endif
