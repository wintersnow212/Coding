#include <cassert>
#include <iostream>
#include <cstring> // for strlen()
#include <cassert> // for assert()
#include <iostream>

using namespace std;
class MyString
{ 
public:
    MyString(const char* source ="")
    {
        assert(source != nullptr); // make sure source isn't a null string
 
        // Find the length of the string
        // Plus one character for a terminator
        m_length = std::strlen(source) + 1;
        
        // Allocate a buffer equal to this length
        m_data = new char[m_length];
        
        // Copy the parameter string into our internal buffer
        for (int i=0; i < m_length; ++i)
            m_data[i] = source[i];
    
        // Make sure the string is terminated
        m_data[m_length-1] = '\0';
    }
    
    
    // Deep copy --- copy constructor
    /*
    Copy constructor is called when a new object is created from an existing     
    object, as a copy of the existing object (see this G-Fact). 
    And assignment operator is called when an already initialized object is 
    assigned a new value from another existing object.
    
     /*
    三种情况下会调用copy constructor
    1.    A newly-created object is initialized to the value of an existing object.
    2.    An object is passed to a function as a non-reference object
    3.    An object is return from a function by value
    
    所以 如果disable 了 copy constructor 就不能return object了
    
    C++ will provide a default copy constructor and default assignment operator that do a shallow copy.
    默认的是浅拷贝  所以自己写的那个copy constructor以及overload = operator就是深拷贝 
    Doing deep copies requires that we write our own copy constructors and overloaded assignment operators.
    */
    MyString(const MyString& source)
    {
         m_length = source.m_length;
 
        // m_data is a pointer, so we need to deep copy it if it is non-null
        if (source.m_data)
        {
            // allocate memory for our copy
            m_data = new char[m_length];

            // do the copy
            for (int i=0; i < m_length; ++i)
                m_data[i] = source.m_data[i];
        }
        else
        {
            m_data = nullptr;
        }

    }

    // Deep Overload = operator
    // Return by reference
    /*
    1.可以作为左值的原因
        Return value:此时生成的是变量a的一个拷贝，即生成了一个临时变量，当这个变量使用完毕之后，变量就被销毁了，
        所以这种返回不能用作左值运算，如：set() = 5;这是不正确的。the return value of a function is an l-value if and 
        only if it is a reference (C++03)

    2.千万不能返回要被销毁的变量 比如局部变量
    如果非要
    如果非要返回一个局部变量的引用，可以new 类型(初值) 申请一个堆内存的临时变量，这样只要不delete释放，
    那么在程序退出之前内存就不会被释放，直到不再使用时便可delete掉.

    和copy construtor 的区别
    The purpose of the copy constructor and the assignment operator are almost equivalent -- both copy one object to another. 
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
    */
    MyString& operator = (const MyString & source)
    {
        cout << "Deep = operator overload is called" << endl;
        // check for self-assignment
        // 注意 这里不能是*this  == source 因为没有== operator
        if (this == &source)
            return *this;

        // first we need to deallocate any value that this string is holding!
        // 这一步非常关键！！！！！
        delete[] m_data;

        // because m_length is not a pointer, we can shallow copy it
        m_length = source.m_length;

        // m_data is a pointer, so we need to deep copy it if it is non-null
        if (source.m_data)
        {
            // allocate memory for our copy
            m_data = new char[m_length];

            // do the copy
            for (int i=0; i < m_length; ++i)
                m_data[i] = source.m_data[i];
        }
        else
            m_data = nullptr;

        return *this;
    }
    ~MyString() // destructor
    {
        // We need to deallocate our string
        // 如果是用new char [] 就得用delete [] m_data
        delete [] m_data;
    }
 
    char* getString() { return m_data; }
    int getLength() { return m_length; }
    
private:
    char *m_data;
    int m_length;
};

int main()
{
    MyString hello("Hello, world!");
    
    {
        MyString copy = hello; // use default copy constructor
    }
    
    // copy is a local variable, so it gets destroyed here.  
    //The destructor deletes copy's string, which leaves hello with a dangling pointer
 
    std::cout << hello.getString() << '\n'; // this will have undefined behavior
    
    MyString assign;
    assign = hello;
    
    
    return 0;
}
