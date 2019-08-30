#include <iostream>
#include <shared_mutex>
using namespace std;

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
    int32_t _status;
    int32_t _waiting_readers;
    int32_t _waiting_writers;
    std::mutex _mtx;
    std::condition_variable _read_cv;
    std::condition_variable _write_cv;
};
