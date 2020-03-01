// http://willzhang4a58.github.io/2016/07/rwlock/
// https://users.cs.duke.edu/~chase/cps210-archive/slides/moresync6.pdf
// 1. 一个 int status variable 表示有多少正在read 或者正在write的user; 0 表示没人读也没人写
// 2. 两个variable 一个表示等待读的user 另外一个表示等待写的user
// 3. 两个condition variable 一个是read的 一个是write的
// 4. 两个condition variable都是建立在一个mutex上

#include <iostream>
#include <shared_mutex>
using namespace std;

// https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
/*
POSIX standard pthread_rwlock_t and associated operations[10]
ReadWriteLock[11] interface and the ReentrantReadWriteLock[6] locks in Java version 5 or above
Microsoft System.Threading.ReaderWriterLockSlim lock for C# and other .NET languages[12]
std::shared_mutex read/write lock in C++17[13]
boost::shared_mutex and boost::upgrade_mutex locks in Boost C++ Libraries[14]
SRWLock, added to the Windows operating system API as of Windows Vista.[15]
sync.RWMutex in Go[16]
Phase fair reader–writer lock, which alternates between readers and writers[17]
std::sync::RwLock read/write lock in Rust[18]
Poco::RWLock in POCO C++ Libraries
mse::recursive_shared_timed_mutex in the SaferCPlusPlus library is a version of std::shared_timed_mutex that supports the recursive ownership semantics of std::recursive_mutex.
txrwlock.ReadersWriterDeferredLock Readers/Writer Lock for Twisted[19]
*/
// Since C++ 17 (VS2015) 
typedef std::shared_mutex Lock;
typedef std::unique_lock< Lock >  WriteLock;
typedef std::shared_lock< Lock >  ReadLock;

// For older compiler
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;

Lock myLock;

void ReadFunction()
{
    ReadLock r_lock(myLock);
    //Do reader stuff
}

void WriteFunction()
{
     WriteLock w_lock(myLock);
     //Do writer stuff
}


//http://willzhang4a58.github.io/2016/07/rwlock/
// Kleiman-Shah read write lock
// Why need the status?? This is used to record the current readers
class RWLock {
public:
    RWLock() : _status(0), _waiting_readers(0), _waiting_writers(0) {}
    RWLock(const RWLock&) = delete;
    RWLock(RWLock&&) = delete;
    RWLock& operator = (const RWLock&) = delete;
    RWLock& operator = (RWLock&&) = delete;

    void rdlock() {
        std::unique_lock<std::mutex> lck(_mtx);
        _waiting_readers += 1;
        _read_cv.wait(lck, [&]() { return _waiting_writers == 0 && _status >= 0; });
        _waiting_readers -= 1;
        _status += 1;
    }

    void wrlock() {
        std::unique_lock<std::mutex> lck(_mtx);
        _waiting_writers += 1;
        _write_cv.wait(lck, [&]() { return _status == 0; });
        _waiting_writers -= 1;
        _status = -1;
    }

    void unlock() {
        std::unique_lock<std::mutex> lck(_mtx);
        if (_status == -1) {
            _status = 0;
        } else {
            _status -= 1;
        }
        if (_waiting_writers > 0) {
            if (_status == 0) {
                _write_cv.notify_one();
            }
        } else {
            _read_cv.notify_all();
        }
    }

private:
    // -1    : one writer
    // 0     : no reader and no writer
    // n > 0 : n reader
    // 这里status 特别重要！！！ 就是表明都多少正在使用中的readers和writers
    int32_t _status;
    // 这下面的两个variable 表示正在等待的waiter
    int32_t _waiting_readers;
    int32_t _waiting_writers;
    std::mutex _mtx;
    // 两个conditional variable
    std::condition_variable _read_cv;
    std::condition_variable _write_cv;
};
