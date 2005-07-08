#include "CLucene/StdHeader.h"
#ifndef _lucene_store_Directory
#define _lucene_store_Directory

#include "CLucene/store/Lock.h"
#include "CLucene/util/VoidList.h"

#include "InputStream.h"
#include "OutputStream.h"

using namespace lucene::store;
namespace lucene{ namespace store{

    //Java's filesystem API is not used directly, but rather through these
    //classes.  This permits:
    //	. implementation of RAM-based indices, useful for summarization, etc.;
    //	. implementation of an index as a single file.
    //
    //
    //A Directory is a flat list of files.  Files may be written once,
    //when they are created.  Once a file is created it may only be opened for
    //read, or deleted.  Random access is permitted when reading and writing.
    //
    //	@author Doug Cutting
	class Directory {
	public:
		virtual ~Directory(){ };

		// Returns an array of strings, one for each file in the directory. 
		virtual void list(char_t**& list, int_t& size) = 0;
		       
		// Returns true iff a file with the given name exists. 
		virtual bool fileExists(const char_t* name) const = 0;

		// Returns the time the named file was last modified. 
		virtual long_t fileModified(const char_t* name) const = 0;

		// Returns the length of a file in the directory. 
		virtual long_t fileLength(const char_t* name) const = 0;

		// Returns a stream reading an existing file. 
		virtual InputStream& openFile(const char_t* name) = 0;

#ifndef CLUCENE_LITE
		// Removes an existing file in the directory. 
		virtual void deleteFile(const char_t* name, const bool throwError = true) = 0;

		// Renames an existing file in the directory.
		//	If a file already exists with the new name, then it is replaced.
		//	This replacement should be atomic. 
		virtual void renameFile(const char_t* from, const char_t* to) = 0;

		// Creates a new, empty file in the directory with the given name.
		//	Returns a stream writing this file. 
		virtual OutputStream& createFile(const char_t* name) = 0;

		// Construct a {@link Lock}.
		// @param name the name of the lock file
		virtual LuceneLock* makeLock(const char_t* name) = 0;
#endif //CLUCENE_LITE

		// Closes the store. 
		virtual void close() = 0;
	};
}}
#endif


