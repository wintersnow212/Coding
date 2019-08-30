#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
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
 

struct BoundedBuffer {
    int* buffer;
    int capacity;

    int front;
    int rear;
    int count;

    std::mutex lock;

    // 这里conditional varaible 有两个！！！
    std::condition_variable not_full;
    std::condition_variable not_empty;

    BoundedBuffer(int capacity) 
        : capacity(capacity)
        , front(0)
        , rear(0)
        , count(0) 
    {
        buffer = new int[capacity];
    }

    ~BoundedBuffer(){
        delete[] buffer;
    }

    void push(int data){
        std::unique_lock<std::mutex> l(lock);

        while (count == capacity)
        {
            not_full.wait(l);
        }
        //not_full.wait(l, [this](){return count != capacity; });

        buffer[rear] = data;
        rear = (rear + 1) % capacity;
        ++count;

        l.unlock();
        not_empty.notify_one();
    }

    int pop(){
        std::unique_lock<std::mutex> l(lock);

        while (count == 0)
        {
            not_empty.wait(l);
        }
        //not_empty.wait(l, [this](){return count != 0; });

        int result = buffer[front];
        front = (front + 1) % capacity;
        --count;

        l.unlock();
        not_full.notify_one();

        return result;
    }
};

void consumer(int id, BoundedBuffer& buffer){
    for(int i = 0; i < 50; ++i)
    {
        int value = buffer.pop();
        //std::cout << "Consumer " << id << " fetched " << value << std::endl;
        synchronized(std::cout) << "Consumer " << id << " fetched " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void producer(int id, BoundedBuffer& buffer){
    for(int i = 0; i < 75; ++i)
    {
        buffer.push(i);
        //std::cout << "Produced " << id << " produced " << i << std::endl;
        // 这里print 也会乱序
        synchronized(std::cout) << "Produced " << id << " produced " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
    BoundedBuffer buffer(200);

    std::thread c1(consumer, 0, std::ref(buffer));
    std::thread c2(consumer, 1, std::ref(buffer));
    std::thread c3(consumer, 2, std::ref(buffer));
    std::thread p1(producer, 0, std::ref(buffer));
    std::thread p2(producer, 1, std::ref(buffer));

    c1.join();
    c2.join();
    c3.join();
    p1.join();
    p2.join();

    return 0;
}
