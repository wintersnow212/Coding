#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
 
class ThreadPool;
  
// our worker thread objects
class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};
  
// the actual thread pool
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F>
    void enqueue(F f);
    ~ThreadPool();
private:
    friend class Worker;
 
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
 
    // the task queue is not fixed size 
    // 所以其实相当于producer是non-blocking的
    // 但是consumer是blocking
    std::deque< std::function<void()>> tasks;
 
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// 这个是friend的原因是因为我们要access thread pool class的东西！！！！
void Worker::operator()()
{
    std::function<void()> task;
    while(true)
    {
        {   // acquire lock
            std::unique_lock<std::mutex> lock(pool.queue_mutex);
             
            // look for a work item
            while(!pool.stop && pool.tasks.empty())
            { // if there are none wait for notification
                pool.condition.wait(lock);
            }
 
            if(pool.stop) // exit if the pool is stopped
                return;
 
            // get the task from the queue
            // 这里才会为执行task做准备
            task = pool.tasks.front();
            pool.tasks.pop_front();
 
        }   // release lock
 
        // execute the task
        // 执行task
        task();
    }
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0; i < threads;++i)
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
        auto lambdaFunc = [this]()
                        {
                            std::function<void()> task;
                            while(true)
                            {
                                {   // acquire lock
                                    std::unique_lock<std::mutex> lock(queue_mutex);

                                    // look for a work item
                                    while(!stop && tasks.empty())
                                    //while(tasks.empty())
                                    { // if there are none wait for notification
                                        condition.wait(lock);
                                    }

                                    if(stop) // exit if the pool is stopped
                                        return;

                                    // get the task from the queue
                                    // 这里才会为执行task做准备
                                    task = tasks.front();
                                    tasks.pop_front();

                                }   // release lock

                                // execute the task
                                // 执行task
                                task();
                            }

                        };
        workers.push_back(std::thread(lambdaFunc));
    }
        
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


// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
    { // acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);
         
        // add the task
        // 这里只是push task 但是不会invoke std::thread
        // 这样thread不会去执行task
        tasks.push_back(std::function<void()>(f));
    } // release lock
     
    // wake up one thread
    condition.notify_one();
}

int main()
{
    ThreadPool pool(4);

    for (int i = 0; i < 8; ++i) 
    {
        pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            });
    }

    // 这里的main thread不会被block
    std::cout << "From main thread" << std::endl;

    // for(auto && result: results)
    //     std::cout << result.get() << ' ';
    // std::cout << std::endl;
    
    return 0;
}
