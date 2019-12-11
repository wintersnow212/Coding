#include <vector>
#include <thread>
#include <functional>
#include <chrono>    // std::chrono::seconds
#include <iostream>  // std::cout
#include <mutex>
#include <queue>
#include <condition_variable>

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


struct ThreadPool
{
    vector<std::thread> threads;
    size_t cap;
    std::mutex _mutex;
    std::condition_variable full;
    std::condition_variable empty;
    queue<std::function<void()>> taskQueue;
    ThreadPool(int numThreads)
        : cap(numThreads)
    {
        
    }
    // 1. Use std::function
    void AddTask(const std::function<void()>& task)  
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (threads.size() == cap)
        //while (taskQueue.size() == cap)
        {
            full.wait(lock);
        }
        
        // taskQueue.push(task);
        
        //std::thread t(std::ref(task));
        std::thread t(task);
        // 如果 try to copy construct a thread, 
        // you'll get an error because the copy constructor is deleted. 
        //You can however move construct a thread:
        threads.push_back(std::move(t));
        //threads.push_back(std::thread(task));
        //threads.back().join();
        //threads.pop_back();
//         方法2 感觉这个会隐式调用了move
//         threads.push_back(std::thread(task));
        
        empty.notify_one();
    }
    
    void ExecuteTask()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (threads.size() == 0)
        //while (taskQueue.size() == cap)
        {
            empty.wait(lock);
        }
        
        if (threads.back().joinable())
        {
            threads.back().join();
            threads.pop_back();
        }
        
        full.notify_one();
    }
    ~ThreadPool()
    {
           
    }
};

bool print_num(int i)
{
    if (i > 0) 
    {
        std::cout << i << '\n';
        return true;
    }
    else 
    {
        return false;
    }
}


void funct(string& s)
{
    std::cout << s << endl;
    
    s = "Function Child thread say hello to main";
}

class Fuctor
{
public:
    void operator()(string& s)
    {
        std::cout << s << endl;
    
        s = "Functor Child thread say hello to main";
    }

};

void threadPoolAdd(ThreadPool& tPool)
{
    int taskNum = 10;
    for (int i = 0; i < taskNum; i++) {
        
        // Pass the lambda as argument
        tPool.AddTask([i]()
                     {
                        std::this_thread::sleep_for(std::chrono::seconds(i));
                        synchronized(std::cout) << "produce thread " << std::this_thread::get_id()
                        << " paused " << i << " seconds" << std::endl;
                     });
    }
}

void threadExcute(ThreadPool& tPool)
{
    int taskNum = 10;
    for (int i = 0; i < taskNum; i++) 
    {
        tPool.ExecuteTask();
    }
}

// To execute C++, please define "int main()"
int main() {
    
    /*
    Here in this code snippet we are creating three threads:
    Thread t1 is passed a function to execute
    Thread t2 is passed a functor to execute
    Thread t3 is passed a lambda to execute 
    
    */
    for (int i = 0; i < 2; ++i)
    {
        string s = "Main thread say hello to child";
        
        if (i == 0)
        {
            // 这里一定要std::ref!!!
            // 因为 The std::thread constructor copies the supplied values, 
            // without converting to the expected argument type (which is reference type in this     
            // case, see  funct()).!!!! 
            // So we need to wrap the arguments that really needs to be references in std::ref.
            std::thread t1(funct, ref(s));
            t1.join();
        }
        else
        {
            // 卧槽这错误 C++说whenever the statement 可以当做function
            // 就会被当做function
            Fuctor fun;
            std::thread t2(fun, ref(s));
            t2.join();
        }
        
        cout << s << endl;
    }
       
    std::vector<std::thread> threadList;
    for(int i = 0; i < 5; i++)
    {
        threadList.push_back(std::thread ([i]()
                             {
                                    std::this_thread::sleep_for(std::chrono::seconds(i));
                                    synchronized(std::cout) << "hello thread " << std::this_thread::get_id()
                                    << " paused " << i << " seconds" << std::endl;
                              }));
        
        // 在这立马 thread Id 就不会变化 因为会把它当做thread结束了
        // 就不会spawn一个new thread
        //thread.join();
    }
    // Now wait for all the worker thread to finish i.e.
    // Call join() function on each of the std::thread object
    // 这里comment out join也能执行 但是会有错因为在thread delete的时候
    //std::for_each(threadList.begin(),threadList.end(), std::mem_fn(&std::thread::join));
    std::for_each(threadList.begin(), threadList.end(), [](std::thread& th){ th.join(); });
    // 这里一定要是& 因为thread delete了copy constructor
    // for (auto& t : threadList)
    // {
    //     t.join();
    // }
   
    ThreadPool tPool(5);
    std::thread producer(threadPoolAdd, ref(tPool));
        
    std::thread consumer(threadExcute, ref(tPool));
    cout << "main thread execute" << endl;
    producer.join();
    consumer.join();
    
    
    return 0;
}