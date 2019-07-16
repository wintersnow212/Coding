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
    */
    MyString& operator = (const MyString & source)
    {
        cout << "Deep = operator overload is called" << endl;
        // check for self-assignment
        if (this == &source)
            return *this;

        // first we need to deallocate any value that this string is holding!
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
