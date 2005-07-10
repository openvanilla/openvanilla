#ifndef _lucene_Config_
#define _lucene_Config_


//define this if you want debugging code to be enabled
//#define _DEBUG

//define this if you want condition debugging to be enabled
//#define _CND_DEBUG

//define this if multi-threading support is not required
//if not defined, multi-thread locking will
//occur (and its related processing overhead)
//#define LUCENE_DISABLE_MULTITHREADING

//define this if Unicode support is desired
//(This is usually already defined in the compile parameters)
//#define _UNICODE

//define this if read only access
//saves on compiling all the writing code
//#define CLUCENE_LITE


//if defined, indexing information will be sent
//to the IndexWriter->infoStream stream
//#define USE_INFO_STREAM

/////////////////////////
//The following are defines that can make CLucene faster and/or smaller
//special queries sometime require longer search times or may not be necessary
//
//Define this to remove fuzzy query and sloppy scoring
//#define NO_FUZZY_QUERY
//
//Define to remove wildcard t*m or te?m to match term
//#define NO_WILDCARD_QUERY
//
//Define to remove prefix term query - ter* to match term or terms
//#define NO_PREFIX_QUERY
//
//Define to remove range (exlusive and inclusive)
//#define NO_RANGE_QUERY
/////////////////////////


//some defaults, wouldn't usually need to be changed
//Buffer size for input/output streams
#define LUCENE_STREAM_BUFFER_SIZE 1024

// DSR:2004.08.19:
// Formerly, StringBuffer used 1024 as the default size of its internal buffer.
// However, StringBuffer is used primarily for token- and term-oriented
// processing, e.g. in StandardTokenizer.  I've calculated that the average
// token (as produced by StandardTokenizer) in all .txt files distributed in
// the Project Gutenberg CD Image (August 2003 release) has only 6 characters.
// Although most languages are likely to have a longer average word length than
// English due to the popularity of "non-atomized" conjugation and declension
// mechanisms, 1024 is still vastly excessive.
// I made two changes intended to deliver better overall performance:
//   a) Switched to a default StringBuffer character capacity of 32.  Though 32
//      is longer than the average token, the high cost of realloc makes a
//      slightly liberal default size optimal.  I chose the default size of 32
//      after fairly extensive experimentation on the Gutenberg e-texts.  The
//      results are summarized in the following table:
//      ------------------------------------------------------------------------
//      LUCENE_DEFAULT_TOKEN_BUFFER_SIZE value | % faster than default size 1024
//      ------------------------------------------------------------------------
//                                           8 : 4%
//                                          16 : 7%
//                                          32 : 6%
//                                          64 : 3%
//      A default size of 32 is actually slightly slower than 16, but I was
//      experimenting on English text; I expect that 32 will maintain decent
//      performance in languages such as German, and in technical documents
//      with long tokens.
//
//   b) To offset the switch to a smaller default buffer size, I implemented a
//      more aggressive growth strategy.  A StringBuffer now [at least] doubles
//      the size of its internal buffer every time it needs to grow, rather
//      than [at least] increasing by LUCENE_DEFAULT_TOKEN_BUFFER_SIZE no
//      matter how many times it has already grown.
#define LUCENE_DEFAULT_TOKEN_BUFFER_SIZE 32


//Number of documents IndexWriter keeps before writing to disk.
#define LUCENE_WRITER_INDEX_INTERVAL 128

//Size of TermScore cache
#define LUCENE_SCORE_CACHE_SIZE 32

//Maximum length of a token word
//if not defined, then no limit, but may affect performance
//#define LUCENE_TOKEN_WORD_LENGTH 255

#endif
