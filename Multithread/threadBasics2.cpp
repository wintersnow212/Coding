#include <thread>
#include <algorithm>
#include <iostream>
#include <functional>
#include <mutex>

template <typename Stream> 
class SyncStream 
{
public:
    SyncStream(Stream &stream) : stream_(&stream) 
    { 
        mutex_.lock(); 
    }

    ~SyncStream() { mutex_.unlock(); }
 
    template <typename T> SyncStream &operator<<(const T &rhs) 
    {
        (*stream_) << rhs;
        return *this;
    }
    
    SyncStream &operator<<(std::ostream &(*manip)(std::ostream &)) 
    {
        (*stream_) << manip;
        return *this;
    }
 
private:
    // 这里是static member 要在类外初始化
    static std::mutex mutex_;
    Stream *stream_;
};


// template <typename Stream> std::mutex-----type
// SyncStream<Stream> ---- 类名
template <typename Stream> std::mutex SyncStream<Stream>::mutex_;
 
template <typename Stream> 
SyncStream<Stream> synchronized(Stream &stream) 
{
    return SyncStream<Stream>(stream);
}


class WorkerThread
{
public:
    void operator()()     
    {
        // 默认的cout的原子性只作用到单个<<符号，也就是说输出并不一定是
        // 对于 synchronized(std::cout) << "This is thread " << id << "\n" 这样的表达式，
        // 临时对象synchronized(std::cout)的生存期是长于其所在整体表达式的生存期的，
        // 所以“synchronized”的destructor会在所有“<<”都执行完之后才执行。
        synchronized(std::cout) << "Worker Thread "<<std::this_thread::get_id()<<" is Executing"<<std::endl;
        //std::cout<<"Worker Thread "<<std::this_thread::get_id()<<" is Executing"<<std::endl;
    }
};

void thread_function()
{
    for(int i = 0; i < 10000; i++);
    
    std::cout<<"thread function Executing"<<std::endl;
}

int main()  
{
     
    // std::thread threadObj(thread_function);
    // for(int i = 0; i < 10000; i++);
    // std::cout<<"Display From MainThread"<<std::endl;
    // threadObj.join();    
    // std::cout<<"Exit of Main function"<<std::endl;

    
    std::vector<std::thread> threadList;
    for(int i = 0; i < 10; i++)
    {
        threadList.push_back( std::thread( WorkerThread() ) );
    }
    // Now wait for all the worker thread to finish i.e.
    // Call join() function on each of the std::thread object
    // 这里comment out join也能执行 但是会有错因为在thread delete的时候
    std::for_each(threadList.begin(),threadList.end(), std::mem_fn(&std::thread::join));
    synchronized(std::cout) << "wait for all the worker thread to finish"<<std::endl;
    //std::cout<<"wait for all the worker thread to finish"<<std::endl;
    //std::for_each(threadList.begin(),threadList.end(), std::mem_fn(&std::thread::join));
    std::cout<<"Exiting from Main Thread"<<std::endl;
    return 0;
}
