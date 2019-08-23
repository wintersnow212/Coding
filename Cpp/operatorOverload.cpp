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
    bool operator() (Interval a, Interval b)
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

struct complx
{
    double real;
    double imag;
    // If you have own constructor, then compiler will not generate 
    // Then you need to define your own
    complx()
    {
        
    }
    complx( double r, double i )
    {
        real = r;
        imag = i;
    }
     
    complx operator+ (const complx& other) const
    {
        complx result;
        result.real = (this->real + other.real);
        result.imag = (this->imag + other.imag);
        return result;
    }
    
    // complx& never return local varibale by reference
    // complx& operator* (const complx& other) const
    complx operator* (const complx& other) const
    {
        complx result;
        result.real = (this->real * other.real);
        result.imag = (this->imag * other.imag);
        return result;
    }
    
    // 这里不需要const的原因是caller 不是const type
    bool operator> (const complx& other)
    {
        if (this->real == other.real)
        {
            return this->imag > other.imag;
        }
        
        return this->real > other.real;
    }
};

// 这里不用的原因是因为struct default是public
std::ostream& operator<<(std::ostream& stream, const complx& other)
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
    complx x(4,4);
    complx y(4,6);
    complx z = x + y; // calls complx::operator+()
    // cout << z.real << endl;
    // cout << z.imag << endl;
    cout << z << endl;
    complx m = x * y;
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
