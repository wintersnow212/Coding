#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

//Definition for an interval.
struct Interval 
{
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

// Function call operator overload
class myFunctor {
public:
    // Function Objects (Functors) - 
    // C++ allows the function call operator() to be overloaded, 
    // such that an object instantiated from a class can be "called"
    // like a function
    bool operator() (const Interval& a, const Interval& b) const
    {
        return a.end > b.end;
    }
};

int minMeetingRooms(vector<Interval>& intervals) {
    int len = intervals.size();
    if (len == 0)
    {
        return 0;
    }

    // 注意这里两个地方take in 不同的object 
    // sort是take in function objet 所以光是compEnd是不够的
    // priority_queue take in的是type 所以CompEnd就够了
    sort(intervals.begin(), intervals.end(), myFunctor());
    priority_queue<Interval, vector<Interval>, myFunctor> pq;
    return 0;
}


/*
    It turns out that there are three different ways to overload operators: 
    1. the member function way,
    2. the friend function way, 
    3. the normal function way. 
    In this lesson, we’ll cover the friend function way    
*/

struct Complx
//class complx
{
    double real;
    double imag;
    // If you have own constructor, then compiler will not generate 
    // Then you need to define your own
    Complx(){ }
    
    Complx(double r, double i )
    {
        real = r;
        imag = i;
    }
    
    
    // Copy constructor
    // Should be deep copy I just don't have the pointer object this time
    Complx (const Complx& other)
        : real(other.real)
        , imag(other.imag)
    {
        
    }
    /*
        Assignment Operator =
        这个其实也是copy assignment
        In summary, the guidelines for the assignment operator are:
        1. Take a const-reference for the argument (the right-hand side of the 
        assignment).
        2.Return a reference to the left-hand side, to support safe and 
        reasonable operator chaining. (Do this by returning *this.)
        3.Check for self-assignment, by comparing the pointers (this to &rhs).
    */
    Complx& operator=(const Complx& other)
    {
        // YOU MUST CHECK FOR SELF-ASSIGNMENT!
        // Compare the two objects' addresses
        if (this == &other)
        {
            return *this;
        }
        
        // 1.  Deallocate any memory that MyClass is using internally
        // 2.  Allocate some memory to hold the contents of rhs
        // 3.  Copy the values from rhs into this instance
        // 4.  Return *this
        // 这里没有memory 操作但是有的话要小心
        real = other.real;
        imag = other.imag;
        
        return *this;
    }
    
    /*
    Compound Assignment Operators += -= *=
    */
    Complx& operator+= (const Complx& other)
    {
        real += other.real;
        imag += other.imag;
        
        return *this;
    }
    
    /*
    这里就不需要像像上面的assignmen那样return class object reference(i.e. *this)
    Binary Arithmetic Operators + - *
    */
    Complx operator+ (const Complx& other) const
    {
        // Complx result;
        // result.real = (this->real + other.real);
        // result.imag = (this->imag + other.imag);
        // return result;
        Complx result(*this);
        result += other;
        return result;
    }
    
    // complx& never return local varibale by reference
    // complx& operator* (const complx& other) const
    Complx operator* (const Complx& other) const
    {
        Complx result;
        result.real = (this->real * other.real);
        result.imag = (this->imag * other.imag);
        return result;
    }
    
    /*
        Comparison Operators == and !=
        这里不需要const的原因是caller 不是const type
    */
    bool operator==(const Complx& other) const
    {
        
        return (this->real == other.real && this->imag == this->imag);
    }
    
    bool operator> (const Complx& other) const
    {
        if (this->real == other.real)
        {
            return this->imag > other.imag;
        }
        
        return this->real > other.real;
    }
    
    // 感觉operator 的override 有两种方式
    // 一种是override在 class definition 里面
    // 另外一种是在外面
    /*
    For all operators where you have to choose to either implement them as 
    a member function or a non-member function, use the following rules of thumb to decide:
    If it is a unary operator, implement it as a member function.
    If a binary operator treats both operands equally (it leaves them unchanged), 
    implement this operator as a non-member function.
    If a binary operator does not treat both of its operands equally (usually it will change its left operand), 
    it might be useful to make it a member function of its left operand’s type, 
    if it has to access the operand's private parts.
    
    
    
    Unary operators need only one operand to perform the task or operation.
    ++        Increment Operator
    --        Decrement Operator
    &        Address Of Operator
    -        Unary Minus Operators
    ~        (One’s Compliment) Negation Operator
    !        Logical NOT
    
    Binary Operators
    +    Binary Plus Operator
    -    Binary Minus Operator
    ==    Equal to Operator
    <    Less than Operator
    
    Ternary operators
    Ternary operators required three operands to perform the operation.
    e.g ? & :
*/
    
    bool operator== (const Complx& other)
    {
        return this->real == other.real && this->imag == other.imag;
    }
};

// bool operator== (const complx& a, const complx& b)
// {
//     return a.real == b.real && a.imag == b.imag;
// }


// 这里不用的原因是因为struct default是public
std::ostream& operator<<(std::ostream& stream, const Complx& other)
{
    stream << other.real << "." << other.imag << endl;
    
    return stream;
}


class Cents
{
public:
    Cents(int cents)
        : m_cents(cents)
    {
            
    }
    /*
    It turns out that there are three different ways to overload operators: 
    1. the member function way,
    2. the friend function way, 
    3. the normal function way. 
    In this lesson, we’ll cover the friend function way    
    */
    
    /*
    Friend 函数类重载
    用friend的原因是我们外面要access private member来调用operator
    As we know friend functions are special type of functions that are not 
    member of the class but can access member variables and can be called 
    without using any objec
    如果是重载双目操作符（即为类的成员函数），就只要设置一个参数作为右侧运算量，而左侧运算量就
    是对象本身
    而 >> 或<< 左侧运算量是 cin或cout 而不是对象本身，所以不满足后面一点，
    就只能申明为友元函数
    */
    friend bool operator>(const Cents &c1, const Cents &c2)
    {
        return (c1.m_cents > c2.m_cents);
    }
    
    // bool operator>(Cents &c1, Cents &c2)
    // {
    //     return (c1.m_cents > c2.m_cents);
    // }
    
    /*
    成员函数重载：可通过this指针访问本类的成员，可以少写一个参数，但是表达式左边的第一个参数必
    须是类对象，通过该类对象来调用成员函数。
     这里需要const的原因是caller 是const type   */
    // bool operator>(const Cents& other) const
    // {
    //     return m_cents > other.m_cents;
    // }
    
    void test (Cents &c1, Cents &c2)
    {
        cout << c1.m_cents << endl;
        cout << c2.m_cents << endl;
    }
private:
    int m_cents;
};

int main()
{
    Complx x(4,4);
    Complx y(4,6);
    Complx z = x + y; // calls complx::operator+()
    // cout << z.real << endl;
    // cout << z.imag << endl;
    cout << z << endl;
    Complx m = x * y;
    // cout << m.real << endl;
    // cout << m.imag << endl;
    cout << m << endl;
    if (x > y)
    {
        cout << "correct" << endl;
    }
    else
    {
        cout << "wrong" << endl;
    }
    
    if (x == y)
    {
        cout << "equal" << endl;
    }
    else
    {
        cout << "not equal" << endl;
    }
    
    Cents n(5);
    Cents d(10);
    if (n > d)
    {
        cout << "larger" << endl;
    }
    else
    {
        cout << "smaller" << endl;
    }
}    
