#include <algorithm>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <vector>

/*
题目的描述很简单，就是实现一个机制使得可以很容易地在多线程情况下serialize输出流（例如cout / cerr）—— 估计大多数debug过多线程程序的人都遇到过这样的需求，而且这个问题目前Stack Overflow上并没有比较好的答案。

举个例子，如果让两个线程同时循环执行下面这句输出语句：
--
std::cout << "This is thread " << id << std::endl;
--

那么默认的cout的原子性只作用到单个<<符号，也就是说输出并不一定是
--
This is thread 1
This is thread 2
...
--
这样一句一句的打出来，而可能会变成
--
This is thread This is thread 1
2
...
--
这样。也就是说一个线程的一整句话还没打完，另一个线程的输出就插进来了。

那么，为了比较优雅地处理这种多线程情况下完整输出一句话的需求，我们希望设计一个类似Java下面synchronized关键字的机制，使得每个线程只要用下列语句：
--
synchronized(std::cout) << "This is thread " << id << "\n";
--
就能保证输出时单个statement的完整性。

题目就是问这个synchronized该如何实现？

*/


using namespace std;
 
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

#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
using namespace std;
// 这里的mutex是global scope是关键
// otherwise 就像下面ofstream class一样 让mutex变成class scope
mutex mut;

void protectedPrint(string msg, int i)
{
     /*********************************************
     1. 直接用mutex 不好的原因是因为如果下面的cout throw exception 不好
     2. The difference is that you can lock and unlock a std::unique_lock.    
        std::lock_guard
        will be locked only once on construction and unlocked on destruction.
     *********************************************/
    lock_guard<std::mutex> guard(mut); // RAII 保证mutex unlock！！！
    //mu.lock();
    cout << msg << i << endl;
    //mu.unlock();
}

class StreamOutput
{
public:
    void protectedPrint(string msg, int i)
    {
         /*********************************************
         1. 直接用mutex 不好的原因是因为如果下面的cout throw exception 不好
         2. The difference is that you can lock and unlock a std::unique_lock.    
            std::lock_guard
            will be locked only once on construction and unlocked on destruction.
         *********************************************/
        lock_guard<std::mutex> guard(mu); // RAII 保证mutex unlock！！！
        //mu.lock();
        //cout << msg << i << endl;
        //mu.unlock();
        f << msg << i << endl;
    }
private:
    std::mutex mu;
    ofstream f;
};


void func(StreamOutput& sf)
{
    for (int i = 0; i > -15; --i)
    {
        protectedPrint(string("From Func "), i);
        //cout << "From func " << i << endl;
    }
}

int main() 
{
    const int n = 100;
    std::vector<std::future<void>> futures(n);
    
    
    // 也就是对于 synchronized(std::cout) << "This is thread " << id << "\n" 
    // 这样的表达式，临时对象synchronized(std::cout)的生存期是长于其所在整体表达式的
    // 生存期的，所以“synchronized”的destructor会在所有“<<”都执行完之后才执行。
    for (int i = 0; i < n; ++i) 
    {
        futures[i] =
            std::async(std::launch::async, [i]() 
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                synchronized(std::cout) << "thread id " << i << std::endl;
                // 对比下面没有syn的例子
                //std::cout << "thread id " << i << std::endl;
            });
    }
      
    for (const auto &future : futures) 
    {
        
        future.wait();
    }
    
    StreamOutput sf;
    std::thread t1(func, std::ref(sf));
    for (int i = 0; i < 15; ++i)
    {
        protectedPrint(string("From Main "), i);
        //cout << "From Main" << i << endl;
    }
    
    t1.join();
 
    return 0;
}
