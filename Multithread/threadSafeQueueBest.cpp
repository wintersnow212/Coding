#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <condition_variable>

using namespace std;
 
template <typename StreamType> 
class SyncStream 
{
public:
    /*
     这里 stream 是不能copy的
     error: use of deleted function 
     ‘std::basic_ostream<_CharT, _Traits>::basic_ostream(const     
     std::basic_ostream<_CharT, _Traits>&) [with _CharT = char; _Traits =    
     std::char_traits<char>]’
     SyncStream(Stream &stream) : stream_(stream)
    */
    SyncStream(StreamType &stream) : stream_(&stream) 
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
    StreamType* stream_;
};


// template <typename Stream> std::mutex-----type
// SyncStream<Stream> ---- 类名
template <typename StreamType> std::mutex SyncStream<StreamType>::mutex_;
 
template <typename StreamType> 
SyncStream<StreamType> synchronized(StreamType &stream) 
{
    return SyncStream<StreamType>(stream);
}
 
template<int cap>
struct BoundedBuffer {
    int* buffer;
    int capacity;

    int wrt;
    int rd;
    int count;

    std::mutex mu;

    // 这里conditional varaible 有两个！！！
    std::condition_variable pushCv;
    std::condition_variable popCv;

    BoundedBuffer(int capacity) 
        : capacity(capacity)
        , wrt(0)
        , rd(0)
        , count(0) 
    {
        buffer = new int[capacity];
    }

    ~BoundedBuffer(){
        delete[] buffer;
    }

    void push(int data){
        std::unique_lock<std::mutex> lock(mu);

        while (count == capacity)
        {
            popCv.wait(lock);
        }
        // capture list 里面的this还不能少 不然count不知道哪来
        //not_full.wait(l, [this](){return count != capacity;});
       
        buffer[wrt] = data;
        wrt = (wrt + 1) % capacity;
        ++count;

        lock.unlock();
        pushCv.notify_one();
    }
    
    int pop(){
    
        // 所有的lock都是build around the underlying  mutex
        std::unique_lock<std::mutex> lock(mu);
        // 这里用while loop 而不是if 也解决了spurious wakeup issue
        // spurious wakeup : a thread might be awoken from its waiting state even though no 
        // thread signaled the condition variable
        // 所以我们需要while loop 再检查一遍
        // https://www.justsoftwaresolutions.co.uk/threading/condition-variable-spurious-wakes.html
        // https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-5:-Condition-Variables
        while (count == 0)
        {
            /* 
          这里的 
          1. 首先 我们在这wait了 就会立马release mutex 不然别人怎么notify你呢
          2. suspend the execution of the thread.
          3. When the condition variable is notified, the mutex is atomically reacquired
          4. 在出function scope destory lock object的时候 If the object currently owns a lock 
             on the managed mutex object,its unlock member is called before destroying object.
          */
            pushCv.wait(lock);
        }
        //not_empty.wait(l, [this](){return count != 0; });

        int result = buffer[rd];
        rd = (rd + 1) % capacity;
        --count;
        /* why the “pop”s don’t need “mlock.unlock();
          All unlocking is taken care of by the condition_variable’s wait function and by the unique_lock’s destructor.
        Manual unlocking is done before notifying, to avoid waking up the waiting thread only to block again 
        因为这里not_full wait的时候在等待这里的lock 释放了  
        the waiting thread might wake up immediately it is notified, and before the mutex is     
        unlocked, in which case it will have to block when the mutex is reacquired on the exit 
        from wait. By rewriting the function so that the notification comes after the mutex is 
        unlocked, the waiting thread will be able to acquire the mutex without blocking
        也就是如果不manually unlock的话 wait thread 在被notify后准备re-acquire lock时会被迫等待！！
      */
  
        lock.unlock();
        popCv.notify_one();

        return result;
    }
    
    int size()
    {
        return count;
    }
};

//void consumer(int id, BoundedBuffer& buffer){
void consumer(int id, BoundedBuffer<200>& buffer){
    while (buffer.size() > 0)
    {
        int value = buffer.pop();
        //std::cout << "Consumer " << id << " fetched " << value << std::endl;
        synchronized(std::cout) << "Consumer " << id << " fetched " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        //std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

//void producer(int id, BoundedBuffer& buffer){
void producer(int id, BoundedBuffer<200>& buffer){
    for(int i = 0; i < 25; ++i)
    {
        buffer.push(i);
        //std::cout << "Produced " << id << " produced " << i << std::endl;
        // 这里print 也会乱序
        synchronized(std::cout) << "Produced " << id << " produced " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(){
    //BoundedBuffer buffer(200);
    BoundedBuffer<200> buffer(200);
    
    std::thread p1(producer, 0, std::ref(buffer));
    std::thread p2(producer, 1, std::ref(buffer));
    std::thread p3(producer, 2, std::ref(buffer));
    std::thread c1(consumer, 0, std::ref(buffer));
    std::thread c2(consumer, 1, std::ref(buffer));
    std::thread c3(consumer, 2, std::ref(buffer));
    

    c1.join();
    c2.join();
    c3.join();
    p1.join();
    p2.join();
    p3.join();

    return 0;
}
