#include <vector>
#include <thread>
#include <functional>
#include <chrono>    // std::chrono::seconds
#include <iostream>  // std::cout

using namespace std;


struct TaskPool
{
    vector<std::function<void()>> tasks;
    
    // 1. Use std::function
    // Note however that it includes some additional overhead for the type erasure 
    // (probably a virtual function call).
    void AddTask(const std::function<void()>& task)  
    {
        // std::thread t(task);
        // tasks.push_back(t);
        tasks.push_back(task);
    }
    
    /*
    template<typename F>
    void f(F &lambda) {  ... }
    这里call template都不用<> !!!!
    Option #2 may, in some cases, be more efficient, as it can avoid a potential heap 
    allocation for the embedded lambda function object, but is only possible iff can be 
    placed in a header as a template function. It may also increase compile times and I-cache 
    footprint, as can any template. Note that it may have no effect as well, as if the lambda 
    function object is small enough it may be represented inline in the std::function object.
    
    */
    // template<typename Functor>
    // void AddTask(Functor fun)
    // {
    //     tasks.push_back(fun);
    // }
    
    void ExcuteTask(int numWorkers)
    {
        for (int n = 0; n < numWorkers; ++n)
        {
            tasks[n]();
        }
    }

};

bool print_num(int i)
{
    if (i > 0) {
        std::cout << i << '\n';
        return true;
    }
    else {
        return false;
    }
    
}
// To execute C++, please define "int main()"
int main() {
  
    TaskPool tPool;
    int taskNum = 5;
    for (int i = 0; i < taskNum; i++) {
        
        // Pass the lambda as argument
        tPool.AddTask([i]()
                     {
                        std::this_thread::sleep_for(std::chrono::milliseconds(i));
                        cout << "Hello World " << "paused " << i << " milliseconds" << endl;
                     });
        
        
    }
    
    tPool.ExcuteTask(taskNum);
    
    // https://en.cppreference.com/w/cpp/utility/functional/function
    // store a free function
    std::function<bool(int)> f_display = print_num;
    // 调用std::function
    f_display(42);
    
    // store a lambda
    std::function<bool()> f_display_42 = []() { return print_num(42); };
    f_display_42();
    
    return 0;
}
