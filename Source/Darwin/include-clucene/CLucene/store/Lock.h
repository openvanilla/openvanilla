#include "CLucene/StdHeader.h"
#ifndef _lucene_store_Lock_
#define _lucene_store_Lock_

namespace lucene{ namespace store{
  class LuceneLock{
  public:
    // Attempt to obtain exclusive access.
    //
    // @return true iff exclusive access is obtained
    virtual bool obtain() = 0;

    // Release exclusive access.
    virtual void release() = 0;

    virtual ~LuceneLock()
    {
    }
  };


  // Utility class for executing code with exclusive access.
  class LuceneLockWith {
  private:
    const static int_t sleepInterval = 10;
    const static int_t maxSleeps = 1000;

  protected:
    LuceneLock* lock;
    // Code to execute with exclusive access.
    virtual void* doBody() = 0;

  // Constructs an executor that will grab the named lock.
  public:
    virtual ~LuceneLockWith() {} // Abstract class needs a virtual destructor.

    //Calls {@link #doBody} while <i>lock</i> is obtained.  Blocks if lock
    //cannot be obtained immediately.  Retries to obtain lock once per second
    //until it is obtained, or until it has tried ten times.
    void* run();
  };
}}
#endif
