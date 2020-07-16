#include <iostream>
#include <atomic>
#include <mutex>
using namespace std;

/*************************************************************************
1. Enforces that only one instance of the class can be created.
2. Provides control over the allocation and destruction of the object.
3. Allows support for thread-safe access to the object’s global state.
4. Avoids polluting the global namespace.
• You don’t want clients to be able to create new instances. This can be done by declaring the default constructor to be private, 
  thus preventing the compiler from automatically creating it as public.
• You want the singleton to be non-copyable, to enforce that a second instance cannot be created. 
  As seen earlier, this can be done by declaring a private copy constructor and a private 
  assignment operator.
• You want to prevent clients from being able to delete the singleton instance. This can be done by 
  declaring the destructor to be private. (Note, however, that some compilers, such as Borland 5.5   
  Visual Studio 6, produce an error incorrectly if you try to declare a destructor as private.)
• The GetInstance() method could return either a pointer or a reference to the singleton class. 
  However, if you return a pointer, clients could potentially delete the object. You should therefore prefer returning a reference.
*************************************************************************/

// https://blog.csdn.net/janeqi1987/article/details/76147312
class Singleton
{
public:
    /*************************************************************************
    Best !!!!!!!!!
    
       这个应该最好 lazy init 同时return return reference not pointer
       clients could potentially delete the object. You should therefore 
       prefer returning a reference.
       The lifetime of function static variables begins the first time[0] the program flow 
       encounters the declaration and it ends at program termination. 
       This means that the run-time must perform some book keeping in order to destruct 
       it only if it was actually constructed.
    *************************************************************************/
    static Singleton& GetInstance()
    {
        // C++11 does guarantee that this is thread-safe:!!!!!!!!!!!!!!!!!
        // If control enters the declaration concurrently 
        // while the variable is being initialized, 
        // the concurrent execution shall wait for completion of the initialization.
        // called thread-safe funciton local static!!!!!
        // 在多线程同时调用的情况下static只会被初始化一次。
        // 也就是说，对于一个符合这个要求的C++11编译器来说，只需要基本结构就可以了。不需要lock
        // 也就是这个其实不是一定需要！！！！！！！！！！！！！
        // unique_lock<mutex> l(m);
        // Lazy initialization 只有在第一次使用的时候才需要初始化出一个singleton对象
        // 同时这里的static 1) end of program保证了会被destory 2) 一个class 只有一个！！！
        static Singleton instance;
        return instance;
    }
    
    
    static Singleton* GetInstancePointerDoubleLocking()
    {
        // Herb 说了解决double lock的根源就是用atomic
        //https://github.com/CppCon/CppCon2014/tree/master/Presentations/Lock-Free%20Programming%20(or%2C%20Juggling%20Razor%20Blades)
        
        // double locking 的本质其实就是为了避免在不需要构造的时候做无谓的lock
        if (m_instance == nullptr)
        {
            lock_guard<mutex> lock(m);
            cout << "Singleton Created" << endl;
            m_instance = new Singleton();
            
        }
        return m_instance;
    }
    
     // 这个很关键啊 不要忘了 disable copy 和 copy assignment
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    
    /// Return some class-specific single-instance state
    int GetState()
    {
        // this is where you could return the singleton's state.
        // I'm keeping it simple for this example though
        return 42;
    }
    
    static void DeleteSingleton()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
            m_instance = nullptr;
        } 
    }
    
    ~Singleton()
    {
        cout << "Singleton destroyed" << endl;
        // 真的是SB了 写了个死循环！！！！！！！ 解决方法写一个static Delete 函数！！！
        // You call the destructor, but the destructor also calls the destructor, so the destructor calls the destructor again... 
        // and again...
        // if (m_instance != nullptr)
        // {
        //     delete m_instance;
        //     m_instance = nullptr;
        // } 
    }
private:
    // Prevent construction of this class
    Singleton()
    {
        
    }
    
    
    static mutex m;
    //static atomic<Singleton*> m_instance;
    // 感觉如果function不是static的话我们不需要声明pointer为static？？
    // 因为我们的double lock 有check nullptr 不过感觉singlton static更对
    // 不用create object instance
    static atomic<Singleton*> m_instance;
    //atomic<Singleton*> m_instance;
};
// type + class name + :: + class variable
atomic<Singleton*> Singleton::m_instance{nullptr};
mutex Singleton::m;


// To execute C++, please define "int main()"
int main() {
    cout << "main() called" << endl;

    // singleton gets created here
    // Singleton &foo = Singleton::GetInstance();
    // foo.GetState();
    
    Singleton* foo = Singleton::GetInstancePointerDoubleLocking();
    foo->GetState();

    // returns previously allocated singleton here
    Singleton* foo2 = Singleton::GetInstancePointerDoubleLocking();
    foo2->GetState();
    
    
    Singleton::DeleteSingleton();
    // delete foo;
    // foo = nullptr;
    // delete foo2;
    // foo2 = nullptr;
    return 0;
}
