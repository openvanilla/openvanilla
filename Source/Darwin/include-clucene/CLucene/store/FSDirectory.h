#ifndef _lucene_store_FSDirectory_
#define _lucene_store_FSDirectory_

#include <sys/timeb.h>
#include <vector>

#include "CLucene/StdHeader.h"
#include "Directory.h"
#include "Lock.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/util/StringBuffer.h"

#ifdef HAVE_DIRENT
# include <dirent.h>
#else
# include "CLucene/util/dirent.h"
#endif

#ifdef _WIN32
# include <io.h>
# include <direct.h>
# include <sys/stat.h>

# ifdef _LARGE_FILE_SUPPORT
#  define fileSeek _lseeki64
#  define fileTell _telli64
# else
#  define fileSeek _lseek
#  define fileTell _tell
# endif

#else
# include <unistd.h>
# include <sys/stat.h>
# define O_RANDOM 0
# ifdef O_BINARY
#  undef O_BINARY //reduces compile errors
# endif
# define O_BINARY 0
# define _close ::close

# ifdef _LARGE_FILE_SUPPORT
#  define fileSeek lseek64
# else
#  define fileSeek lseek
# endif
//ftell (and probably soon ftell64) are POSIX standard functions, but tell and
//tell64 are not, so we define fileTell in terms of fileSeek.
# define fileTell(fhandle) fileSeek(fhandle, 0, SEEK_CUR)

# define _read read
# define _write write
#endif


using namespace std;
namespace lucene{ namespace store{
  static lucene::util::VoidMap< const char_t*,Directory*> DIRECTORIES(true,DELETE_TYPE_DELETE_ARRAY,false,DELETE_TYPE_NONE);
    STATIC_DEFINE_MUTEX(DIRECTORIES_MUTEX);

  class FSInputStream:public InputStream {
  protected:
    int_t fhandle;
    DEFINE_MUTEX(file_mutex);
  public:
    bool isClone;

    FSInputStream(const char_t* path);
    FSInputStream(FSInputStream& clone);
    ~FSInputStream();

    InputStream& clone();
    void close();

  protected:
    // Random-access methods
    void seekInternal(const long_t position);
    // InputStream methods
    void readInternal(l_byte_t* b, const int_t offset, const int_t len);

  };



#ifndef CLUCENE_LITE
  class FSOutputStream: public OutputStream {
  private:
    int_t fhandle;
  public:
    FSOutputStream(const char_t* path);
    ~FSOutputStream();

    OutputStream& clone();

    // output methods:
    void flushBuffer(const l_byte_t* b, const int_t size);
    void close();

    // Random-access methods
    void seek(const long_t pos);
    long_t Length();

  };
#endif // CLUCENE_LITE




  class FSDirectory:public Directory{
  public:
    // This cache of directories ensures that there is a unique Directory
    // instance per path, so that synchronization on the Directory can be used to
    // synchronize access between readers and writers.
    //
    // This should be a WeakHashMap, so that entries can be GC'd, but that would
    // require Java 1.2.  Instead we use refcounts...
  private:
    char_t directory[CL_MAX_DIR];
    int_t refCount;



      DEFINE_MUTEX (FSDIR_CREATE);
    void create();

    void priv_getFN(char_t* buffer, const char_t* name) const;
    int_t priv_getStat(const char_t* name, struct Struct_Stat* ret) const;
  public:
    FSDirectory(const char_t* path, const bool createDir);
    ~FSDirectory();

    // Returns an array of strings, one for each file in the directory.
    void list(char_t**& list, int_t& size);

    // Returns true iff a file with the given name exists.
    bool fileExists(const char_t* name) const;

    const char_t* getDirName() const;


    // Returns the directory instance for the named location.
    //
    // <p>Directories are cached, so that, for a given canonical path, the same
    // FSDirectory instance will always be returned.  This permits
    // synchronization on directories.
    //
    // @param file the path to the directory.
    // @param create if true, create, or erase any existing contents.
    // @return the FSDirectory for the named file.
    static FSDirectory& getDirectory(const char_t* file, const bool create);

    // Returns the time the named file was last modified.
    long_t fileModified(const char_t* name) const;

    // Returns the time the named file was last modified.
    //static
    static long_t fileModified(const char_t* dir, const char_t* name);

    // Returns the length in bytes of a file in the directory.
    //static
    long_t fileLength(const char_t* name) const;

    // Returns a stream reading an existing file.
    InputStream& openFile(const char_t* name);

#ifndef CLUCENE_LITE
    // Removes an existing file in the directory.
    void deleteFile(const char_t* name, const bool throwError=true);

    DEFINE_MUTEX(FSDIR_RENAME);
    // Renames an existing file in the directory.
    void renameFile(const char_t* from, const char_t* to);

    // Creates a new, empty file in the directory with the given name.
    //	Returns a stream writing this file.
    OutputStream& createFile(const char_t* name);

    // Construct a {@link Lock}.
    // @param name the name of the lock file
    LuceneLock* makeLock(const char_t* name);
#endif // CLUCENE_LITE

    void refInc();

    DEFINE_MUTEX(FSDIR_CLOSE);
    // Closes the store to future operations.
    void close();

        DEFINE_MUTEX(LOCK_MUTEX);
  };

#ifndef CLUCENE_LITE
  class FSLock:public virtual LuceneLock{
  public:
          // const char_t* fname;
          char_t* fname; // removed const because the free() wouldn't compile otherwise. --RGR
    FSLock ( const char_t* name );
    ~FSLock();
    bool obtain();
    void release();
  };
#endif // CLUCENE_LITE

}}
#endif
