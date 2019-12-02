#include <iostream>
#include <mutex>
using namespace std;
/*************************************************************************
1. Enforces that only one instance of the class can be created.
2. Provides control over the allocation and destruction of the object.
3. Allows support for thread-safe access to the object’s global state.
4. Avoids polluting the global namespace.

• You don’t want clients to be able to create new instances. This can be done by declaring the default constructor to be private, thus preventing the compiler from automatically creating it as public.

• You want the singleton to be non-copyable, to enforce that a second instance cannot be created. As seen earlier, this can be done by declaring a private copy constructor and a private assignment operator.

• You want to prevent clients from being able to delete the singleton instance. This can be done by declaring the destructor to be private. (Note, however, that some compilers, such as Borland 5.5 and Visual Studio 6, produce an error incorrectly if you try to declare a destructor as private.)

• The GetInstance() method could return either a pointer or a reference to the singleton class. However, if you return a pointer, clients could potentially delete the object. You should therefore prefer returning a reference.


*************************************************************************/

// https://blog.csdn.net/janeqi1987/article/details/76147312
class Singleton
{
public:
    /// Return the single instance of this class
    static Singleton& GetInstance();
    
    
    static Singleton& GetInstanceDoubleLocking();

    /// Return some class-specific single-instance state
    int GetState();

private:
    // Prevent construction and copying of this class
    Singleton()
    {
        cout << "Singleton created" << endl;
    }
    ~Singleton()
    {
        cout << "Singleton destroyed" << endl;
        // 如果是pointer 就要delete 不要会内存泄露！！！
        delete m_instance;
        m_instance = nullptr;
    }
    
    Singleton(const Singleton &);
    const Singleton &operator =(const Singleton &);
    
    static Singleton* m_instance;
    static std::mutex m_mutex;
};
// type + class name + :: + class variable
Singleton* Singleton::m_instance = nullptr;
mutex Singleton::m_mutex;

// clients could potentially delete the object. You should therefore 
// prefer returning a reference.
Singleton& Singleton::GetInstance()
{
    // Thread safe
    unique_lock<mutex> l(m_mutex);
    // Lazy initialization
    static Singleton instance;
    return instance;
    
    // C++ 11
    // If control enters the declaration concurrently 
    // while the variable is being initialized, 
    // the concurrent execution shall wait for completion of the initialization.
    // 在多线程同时调用的情况下static只会被初始化一次。
    // 也就是说，对于一个符合这个要求的C++11编译器来说，只需要基本结构就可以了。
}

// Double locking 
Singleton& Singleton::GetInstanceDoubleLocking()
{
    if (m_instance == nullptr)
    {
        // 我们下面还需要一个if ( == nullptr)的check 
        // 同时这个lock要加在它的外面这样保证了 if check的atomic性
        // 但是需要 memory barrier 
        unique_lock<mutex> l(m_mutex);
        if (m_instance == nullptr)
        {
            // Lazy initialization
            m_instance = new Singleton();
        } 
    }

    return *m_instance;
}

int Singleton::GetState()
{
    // this is where you could return the singleton's state.
    // I'm keeping it simple for this example though
    return 42;
}

// To execute C++, please define "int main()"
int main() {
    cout << "main() called" << endl;

    // singleton gets created here
    Singleton &foo = Singleton::GetInstance();
    foo.GetState();

    // returns previously allocated singleton here
    Singleton &foo2 = Singleton::GetInstance();
    foo2.GetState();

    return 0;
}
