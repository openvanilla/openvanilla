#ifndef _lucene_util_BitVector_
#define _lucene_util_BitVector_

#include "CLucene/StdHeader.h"
#include "CLucene/store/Directory.h"

using namespace std;

namespace lucene{ namespace util{
	class BitVector {
	private:
		int_t size;
		int_t count;

		// !!!This is public just so that methods will inline.  Please don't touch.
		l_byte_t* bits;
		

	public:
		// Constructs a vector capable of holding <code>n</code> bits. 
		BitVector(int_t n);
		~BitVector();
		
		// Constructs a bit vector from the file <code>name</code> in Directory
		//	<code>d</code>, as written by the; method.
		BitVector(lucene::store::Directory& d, const char_t* name);

		// Sets the value of <code>bit</code> to one. 
		void set(const int_t bit);

		// Sets the value of <code>bit</code> to zero. 
		void clear(const int_t bit);

		// Returns <code>true</code> if <code>bit</code> is one and
		//	<code>false</code> if it is zero. 
		bool get(const int_t bit);

		// Returns the number of bits in this vector.  This is also one greater than
		//	the number of the largest valid bit number. 
		int_t Size();

		// Returns the total number of one bits in this vector.  This is efficiently
		//	computed and cached, so that, if the vector is not changed, no
		//	recomputation is done for repeated calls. 
		int_t Count();

		

		// Writes this vector to the file <code>name</code> in Directory
		//	<code>d</code>, in a format that can be read by the constructor {@link
		//	#BitVector(Directory, String)}.  
		void write(lucene::store::Directory& d, const char_t* name);

	};
}}
#endif
