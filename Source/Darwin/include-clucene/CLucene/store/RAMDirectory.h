#include "CLucene/StdHeader.h"
#ifndef _lucene_store_RAMDirectory_
#define _lucene_store_RAMDirectory_

#include "Lock.h"
#include "Directory.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/util/Arrays.h"

namespace lucene{ namespace store{
	class RAMDirectory; //predefined

	class RAMFile {
	public:
		lucene::util::VoidList<l_byte_t*> buffers;
		long_t length;
		long_t lastModified;

#ifdef _DEBUG
		char_t* filename;
#endif

		RAMFile();
		~RAMFile();
	};

#ifndef CLUCENE_LITE
	class RAMLock: public virtual LuceneLock{
	private:
		RAMDirectory* directory;
		const char_t* fname;
	public:
		RAMLock(const char_t* name, RAMDirectory* dir);
		~RAMLock();
		bool obtain();

		void release();
	};
#endif

#ifndef CLUCENE_LITE
	class RAMOutputStream: public OutputStream {
	protected:
		RAMOutputStream(RAMOutputStream& clone);
	public:
		RAMFile& file;
		int_t pointer;

		RAMOutputStream(RAMFile& f);
		~RAMOutputStream();

		OutputStream& clone();

		// output methods:
		void flushBuffer(const l_byte_t* src, const int_t len);

		virtual void close();

		// Random-at methods
		virtual void seek(const long_t pos);
		long_t Length();
	};
#endif

	class RAMInputStream:public InputStream {
	private:
		RAMFile& file;
		int_t pointer;

	public:
		RAMInputStream(RAMFile& f);
		RAMInputStream(RAMInputStream& clone);
		~RAMInputStream();
		InputStream& clone();

		// InputStream methods
		void readInternal(l_byte_t *dest, const int_t idestOffset, const int_t len);

		void close();

		// Random-at methods
		void seekInternal(const long_t pos);

		RAMInputStream& operator=( const RAMInputStream& str )
		{
			throw "CAN'T ASSIGN THIS OBJECT";
		}
	};





	class RAMDirectory:public Directory{
	protected:
		lucene::util::VoidMap<const char_t*,RAMFile*> files;
	public:
		DEFINE_MUTEX(files_mutex);

		// Returns an array of strings, one for each file in the directory.
		void list(char_t**& list, int_t& size);

		RAMDirectory();
		~RAMDirectory();

		// Returns true iff the named file exists in this directory.
		bool fileExists(const char_t* name) const;

		// Returns the time the named file was last modified.
		long_t fileModified(const char_t* name) const;

		// Returns the length in bytes of a file in the directory.
		long_t fileLength(const char_t* name) const;

#ifndef CLUCENE_LITE
		// Removes an existing file in the directory.
		virtual void renameFile(const char_t* from, const char_t* to);

		// Removes an existing file in the directory.
		virtual void deleteFile(const char_t* name, const bool throwError = true);

		// Creates a new, empty file in the directory with the given name.
		//	Returns a stream writing this file.
		virtual OutputStream& createFile(const char_t* name);

		// Construct a {@link Lock}.
		// @param name the name of the lock file
		LuceneLock* makeLock(const char_t* name);
#endif

		// Returns a stream reading an existing file.
		InputStream& openFile(const char_t* name);

		virtual void close();

	};
}}
#endif
