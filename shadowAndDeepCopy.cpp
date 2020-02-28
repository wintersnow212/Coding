#include <iostream>
#include <cstring>
using namespace std;

class MyString
{
public:
    MyString()
        : m_data(nullptr)
        , m_len(0)
    {

    }
    MyString(const char* data)
    {
        // 这里一定要+1！！！！
        // strlen: Returns the length of the given byte string not including null terminator;
        m_len = strlen(data) + 1;
        //m_len = strlen(data);
        
        // 这里必须要class 自己hold memory 不然要是shadow copy的pointer被free了呢
        // Allocate a buffer equal to this length
        m_data = new char[m_len];
        strcpy(m_data, data);
        memcpy(m_data, data, sizeof(char) * m_len);
        //cout << m_data << endl;
        // Do I need to add null terminator here??? No !!!
    }

    /******************************************************************************
    Shadow copy
 ******************************************************************************/
    // MyString(const MyString& other)
    //     : m_data(other.m_data)
    //     , m_len(other.m_len)
    // {

    // }

    /*****************************************************************************************************
    Deep copy --- copy constructor
    
    Copy constructor is called when a new object is created from an existing     
    object, as a copy of the existing object (see this G-Fact). 
    And assignment operator is called when an already initialized object is 
    assigned a new value from another existing object.
    
    三种情况下会调用copy constructor
    1.    A newly-created object is initialized to the value of an existing object.
    2.    An object is passed to a function as a non-reference object
    3.    An object is return from a function by value
    
    所以 如果disable 了 copy constructor 就不能return object了
    
    C++ will provide a default copy constructor and default assignment operator that do a shallow copy.
    默认的是浅拷贝  所以自己写的那个copy constructor以及overload = operator就是深拷贝 
    Doing deep copies requires that we write our own copy constructors and overloaded assignment operators.
    ******************************************************************************************************/
    MyString(const MyString& other)
    {
        cout << "Copy constructor called" << endl;
        // copy assignment 才需要free!!!!!!!!
        // if (m_data != nullptr)
        // {
        //     delete [] m_data;
        //     m_data = nullptr;
        // }
        m_len  = other.m_len;
        m_data = new char[m_len];

        memcpy(m_data, other.m_data, sizeof(char) * m_len);
    }

    /******************************************************************************
    Return by reference
    1.可以作为左值的原因
      Return value:此时生成的是变量a的一个拷贝，即生成了一个临时变量，当这个变量使用完毕
      之后，变量就被销毁了，所以这种返回不能用作左值运算，如：set() = 5;这是不正确的。the 
      return value of a function is an l-value if and only if it is a reference 
      (C++03)
    2.千万不能返回要被销毁的变量的引用 比如局部变量
    如果非要
    如果非要返回一个局部变量的引用，可以new 类型(初值) 申请一个堆内存的临时变量，这样只要不   
    delete释放，
    那么在程序退出之前内存就不会被释放，直到不再使用时便可delete掉.
    和copy construtor 的区别
    The purpose of the copy constructor and the assignment operator are almost 
    equivalent -- both copy one object to another. 
    However, the copy constructor initializes new objects, 
    whereas the assignment operator replaces the contents of existing objects.
    a b; //constructor
    a c; //constructor
    b = c; //copy assignment
    c = a(b); //copy constructor, then copy assignment
    a* b = new a(); //constructor called
    a* c; //nothing is called
    c = b; //still nothing is called
    c = new a(*b); //copy constructor is called
    
    if a functions receives as argument, passed by value, an object of a class
    void foo(MyClass a);
    foo(a);
    When a function returns (by value) an object of the class
    MyClass foo ()
    {
        MyClass temp;
        ....
        return temp; // copy constructor called
    }
*******************************************************************************/

//     MyString& operator = (const MyString& other)
//     {
//         cout << "Copy Assignment called" << endl;
//         // Copy ans Swap idom
//         MyString cpy = other;
//         swap(m_data, cpy.m_data);
//         swap(m_len, cpy.m_len);

//         return *this;
//     }
    
    MyString& operator = (MyString other)
    {
        cout << "Copy Assignment called" << endl;
        swap(m_data, other.m_data);
        swap(m_len, other.m_len);

        return *this;
    }

//     MyString& operator = (const MyString & source)
//     {
//         cout << "Deep = operator overload is called" << endl;
//         // Check for self-assignment
//         // 注意 这里不能是*this  == source 因为没有== operator
//         if (this == &source)
//             return *this;

//         // first we need to deallocate any value that this string is holding!
//         // 这一步非常关键！！！！！
//         if (m_data != nullptr)
//         {
//             delete[] m_data;
//             m_data = nullptr;
//         }
        
//         // because m_length is not a pointer, we can shallow copy it
//         m_length = source.m_length;

//         // m_data is a pointer, so we need to deep copy it if it is non-null
//         if (source.m_data)
//         {
//             // allocate memory for our copy
//             m_data = new char[m_length];

//             // do the copy
//             memcpy(m_data, source.m_data, strlen(source.m_data) + 1);
//             //memcpy(m_data, source.m_data, sizeof(char) * m_length);
//         }
//         else
//             m_data = nullptr;

//         return *this;
//     }
    
    
    // Move constructor
    MyString (MyString&& other)
        : m_data(other.m_data)
        , m_len (other.m_len)
    {
        // move 是相当于 让另外一个指针指向他 
        // 所以当然不能delete了， delete之后原来指向他的pointer怎么办！！！
        other.m_data = nullptr;
        other.m_len  = 0;
    }

    ~ MyString()
    {
        if (m_data != nullptr)
        {
            // 如果是用new char [] 就得用delete [] m_data
            delete [] m_data;
            m_data = nullptr;
        }
    }
    char* getData()
    {
        return m_data;
    }

    int getLen()
    {
        return m_len;
    }
private:
    char* m_data;
    int m_len;
};

int main()
{
    MyString test ("Hello World");
    {
        MyString localTemp(test);
    }
    // Copy is a local variable, so it gets destroyed here.  
    // The destructor deletes copy's string, which leaves hello with a dangling pointer

    cout << test.getData() << endl;
    MyString assign("Hi Earch");
    // MyString assign;
    // assign = test;
    // cout << assign.getData() << endl;
    // 这里要想调用copy assignment 必须是两个已经created的object
    // empty object i.e.  MyString assign; 也算是created!!!!
    test = assign;
    cout << test.getData() << endl;
    
    
    return 0;
}
