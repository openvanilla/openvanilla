#ifndef _LuceneThreads_h
#define  _LuceneThreads_h
#ifndef LUCENE_DISABLE_MULTITHREADING
class mutex
{
private:
  int locked;
public:
    mutex() { locked=0; }
    ~mutex() {  }
    void lock() {
    while ( locked != 0 )
      processSleep(1);

    locked = 1; //wait till unlocked, then lock
  }
    void unlock() {
# ifdef _DEBUG
    if ( locked == 1 ){
      locked = 0;
    } else
      _THROWC( "Mutex wasn't locked");
# else
    locked = 0;
# endif // _DEBUG
  }
};

class mutexGuard
{
private:
    mutex& mrMutex;
public:
    mutexGuard( mutex& rMutex ) :
    mrMutex(rMutex)
  {
    mrMutex.lock();
  }
    ~mutexGuard()
  {
    mrMutex.unlock();
  }
};

#define LOCK_MUTEX(theMutex) { mutexGuard theMutexGuard(theMutex);
#define UNLOCK_MUTEX(x) }

# define DEFINE_MUTEX(x) mutex x
# define STATIC_DEFINE_MUTEX(x) static mutex x

#else // DSR:2004.07.04: Restore ability to build with multithreading disabled:

  #define LOCK_MUTEX(theMutex)
  #define UNLOCK_MUTEX(x)

  # define DEFINE_MUTEX(x)
  # define STATIC_DEFINE_MUTEX(x)

#endif
#endif
