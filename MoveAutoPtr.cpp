/// rvalue-references-move-semantics.cpp
// compile with: /EHsc
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;


/****************************************************************************************
 Another example from learn c++ 
 https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment
****************************************************************************************/
template<class T>
class Auto_ptr4
{
    T* m_ptr;
public:
    Auto_ptr4(T* ptr = nullptr)
        :m_ptr(ptr)
    {
    }
 
    ~Auto_ptr4()
    {
        delete m_ptr;
    }
 
    // Copy constructor
    // Do deep copy of a.m_ptr to m_ptr
    Auto_ptr4(const Auto_ptr4& a)
    {
        std::cout << "Copy constructor" << std::endl;
        m_ptr = new T;
        memcpy(m_ptr, a.m_ptr, sizeof(Auto_ptr4));
        //*m_ptr = *a.m_ptr;
    }
 
    // Copy assignment
    // Do deep copy of a.m_ptr to m_ptr
    Auto_ptr4& operator=(const Auto_ptr4& a)
    {
        std::cout << "Copy assignment" << std::endl;
        // Self-assignment detection
        if (&a == this)
            return *this;
 
        // Release any resource we're holding
        delete m_ptr;
 
        // Copy the resource
        m_ptr = new T;
        //*m_ptr = *a.m_ptr;
        memcpy(m_ptr, a.m_ptr, sizeof(Auto_ptr4));
 
        return *this;
    }
    
    // Move constructor
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr4(Auto_ptr4&& a)
        : m_ptr(a.m_ptr)
    {
        std::cout << "move constructor" << std::endl;
        a.m_ptr = nullptr; // we'll talk more about this line below
    }
 
    // Move assignment
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr4& operator=(Auto_ptr4&& a)
    {
        std::cout << "move assignment" << std::endl;
        // Self-assignment detection
        if (&a == this)
            return *this;
 
        // Release any resource we're holding
        delete m_ptr;
 
        // Transfer ownership of a.m_ptr to m_ptr
        m_ptr = a.m_ptr;
        a.m_ptr = nullptr; // we'll talk more about this line below
 
        return *this;
    }
 
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    bool isNull() const { return m_ptr == nullptr; }
};
 
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};
 
Auto_ptr4<Resource> generateResource()
{
    Auto_ptr4<Resource> res(new Resource);
    return res; // this return value will invoke the move constructor
}

/****************************************************************************************
 Perfect forwarding
 
 "perfect forwarding", avoids excessive copying, 
 and avoids the template author having to write multiple overloads for lvalue and rvalue references
 
****************************************************************************************/
class Test {
    int * arr{nullptr};
public:
    Test():arr(new int[5000]{1,2,3,4}) { 
        cout << "default constructor" << endl;
    }
    Test(const Test & t) {
        cout << "copy constructor" << endl;
        if (arr == nullptr) arr = new int[5000];
        memcpy(arr, t.arr, 5000*sizeof(int));
    }
    Test(Test && t): arr(t.arr) {
        cout << "move constructor" << endl;
        t.arr = nullptr;
    }
    ~Test(){
        cout << "destructor" << endl;
        delete [] arr;
    }
};
template <typename T>
void func(T t) {
    cout << "in func" << endl;
}

// Here is universal reference
template <typename T>
void relay(T&& t) {
    cout << "in relay" << endl;
    cout << "-----------not using the forward-----------" << endl;
    func(t);
    cout << "---------- using the forward-----------" << endl;
    // perfect forward
    // 这时，我们需要std::forward<T>()。与std::move()相区别的是，
    // move()会无条件的将一个参数转换成右值，而forward()则会保留参数的左右值类型
    func(std::forward<T>(t));
}

Test createTest() {
    return Test();
}


int main()
{
    // Test perfect forwarding!!!
    relay(Test());
    cout << "--------------------End of test Perfect forwarding-------------" << endl;
    
    std::string str = "Hello";
    std::vector<std::string> vec;
 
    // uses the push_back(const T&) overload, which means 
    // we'll incur the cost of copying str
    vec.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    vec.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << vec[0]
                                         << "\", \"" << vec[1] << "\"\n";
    
    
    Auto_ptr4<Resource> mainres;
    mainres = generateResource(); // this assignment will invoke the move assignment
}
