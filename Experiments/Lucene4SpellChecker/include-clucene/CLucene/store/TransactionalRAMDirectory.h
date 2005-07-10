#include "CLucene/StdHeader.h"
#ifndef _lucene_store_TransactionalRAMDirectory_
#define _lucene_store_TransactionalRAMDirectory_

#include <set>

#include "RAMDirectory.h"

#include "CLucene/util/VoidList.h"


using namespace std;
using namespace lucene::util;
namespace lucene { namespace store {

  // This transactional in-memory Directory was created to address a specific
  // situation, and was deliberately pared down to the simplest viable
  // implementation.  For the sake of simplicity, this implementation imposes
  // restrictions on what operations can be performed in the directory while a
  // transaction is in progress (documented in TransactionalRAMDirectory.cpp).
  //
  // Because the Lucene Directory interface itself is rather simplistic, it
  // would not be difficult to expand TransactionalRAMDirectory so that it
  // provided fully general transactionality.  However, the developer of this
  // original implementation was of the opinion that the last thing CLucene
  // needs is gratuitous features that exceed their required complexity and
  // haven't been rigorously tested.
  class TransactionalRAMDirectory: public RAMDirectory {
  private:
    typedef std::set<const char_t*, lucene::util::charCompare> FilenameSet;
    FilenameSet filesToRemoveOnAbort;

    typedef lucene::util::VoidMap<const char_t*, RAMFile*> FileMap;
    FileMap filesToRestoreOnAbort;

    bool transOpen;

    void transResolved();
    bool archiveOrigFileIfNecessary(const char_t* name);
    void unarchiveOrigFile(const char_t* name);

  public:
    TransactionalRAMDirectory();

    bool transIsOpen();
    void transStart();
    void transCommit();
    void transAbort();

    // Constrained operations:
    void deleteFile(const char_t* name, const bool throwError = true);
    void renameFile(const char_t* from, const char_t* to);
    OutputStream& createFile(const char_t* name);

    void close();
  };

}}
#endif // ndef _lucene_store_TransactionalRAMDirectory_
