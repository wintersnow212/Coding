// http://progsch.net/wordpress/?p=81
// http://progsch.net/wordpress/?p=106
/*****************************************************
Thread Pools
Thread pools allow you to decouple task submission and execution. 
You have the option of exposing an executor’s configuration while deploying an application or switching one executor for 
another seamlessly.
A thread pool consists of homogenous worker threads that are assigned to execute tasks. 
Once a worker thread finishes a task, it is returned to the pool. 
Usually, thread pools are bound to a queue from which tasks are dequeued for execution by worker threads.
A thread pool can be tuned for the size of the threads it holds. 
A thread pool may also replace a thread if it dies of an unexpected exception. 
Using a thread pool immediately alleviates from the ails of manual creation of threads. 
Important notes about thread pools:
There’s no latency when a request is received and processed by a thread because no time is lost in creating a thread.
The system will not go out of memory because threads are not created without any limits
Fine tuning the thread pool will allow us to control the throughput of the system. We can have enough threads to 
keep all processors busy but not so many as to overwhelm the system.
The application will degrade gracefully if the system is under load.
******************************************************/
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <string>

using namespace std;
std::mutex mu;

// 这里是如何使用std::function!!!!!
using Func = function<void(int)>;

// 应该还有更好的print helper function!!!!!
void printHelper(const std::string& str, int i)
{
    // mutex 不用放在local scope 
    // 要么global scope;要么class scope被shared
    //std::mutex mu;
    std::unique_lock<std::mutex> l(mu);
    
    std::cout << str << " " << i << std::endl;
}

// the actual thread pool
class ThreadPool {
public:
    ThreadPool(size_t);
    // template<typename F>
    // void enqueue(F f);
    void enqueue(Func f);
    ~ThreadPool();
private:
    //friend class Worker;
 
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
 
    // the task queue is not fixed size 
    // 所以其实相当于producer是non-blocking的
    // 但是consumer是blocking
    std::deque<Func> tasks;
 
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0; i < threads; ++i)
    {
        /*******************************************************
           workers.push_back(std::thread(Worker(*this)));
           operator()() overloads the call operator. 
           The first “()” indicates the operator to overload, 
           the second is the argument list of the overload. 
           So a object of type Worker can be called like it was a function (syntactically speaking). 
           So we can then pass a Worker object to a thread constructor which the thread will start executing. 
           In the updated version on github the Worker class is gone btw. and replaced by a lambda.
        // capture member variable via this pointer!!!!!! 避免了friend class
        // 貌似是working的！！！
        //auto lambdaFunc = [ThreadPool = ThreadPool]()
        ******************************************************************/
        auto lambdaFunc = [this, i]()
                        {
                            Func task;
                            while(true)
                            {
                                // 这里放在外面可以保证所有用这个queue_mutex的thread不会被interrupt
                                //std::unique_lock<std::mutex> lock(queue_mutex);
                                
                                {   // acquire lock
                                    std::unique_lock<std::mutex> lock(queue_mutex);

                                    // look for a work item
                                    while(!stop && tasks.empty())
                                    //while(tasks.empty())
                                    { // if there are none wait for notification
                                        condition.wait(lock);
                                    }

                                    if (stop) // exit if the pool is stopped
                                        return;

                                    // get the task from the queue
                                    // 这里才会为执行task做准备
                                    task = tasks.front();
                                    tasks.pop_front();

                                }   // release lock

                                // execute the task
                                // 执行task
                                // 这里是输入function arg的！！！！
                                task(i);
                            }

                        };
        
        // 这里是先在构造函数里面create 4个worker thread
        // 但是我们可以看到 因为我们lambad的在while(empty())的时候等待
        // 只有在enqueue的时候我们的task 被push了function 我们才有任务执行
        workers.push_back(std::thread(lambdaFunc));
    }
}

// add new work item to the pool
void ThreadPool::enqueue(Func f)
{
    { // acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);
         
        // add the task
        // 这里只是push task 但是不会invoke std::thread
        // 这样thread不会去执行task
        tasks.push_back(f);
    } // release lock
     
    // wake up one thread
    condition.notify_one();
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // stop all threads
    stop = true;
    condition.notify_all();
     
    // join them
    for(size_t i = 0; i < workers.size();++i)
        workers[i].join();
}


int main()
{
    ThreadPool pool(6);

    for (int i = 1; i < 7; ++i) 
    {
        pool.enqueue([](int val){
                //std::cout << "hello " << i << std::endl;
                printHelper("hello", val);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                printHelper("world", val);
            });
    }

    // 这里的main thread不会被block 因为这里是单独的另外的thread!!!
    printHelper("From main thread", 0);
    
    return 0;
}
