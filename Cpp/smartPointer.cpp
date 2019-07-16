#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

class A
{
public:
    A (int val):m_val(val)
    {
        
    }
    
    A (const A& a)
    {
        cout << "Copy constructor called" << endl;
    }
    
    int GetVal()
    {
        return m_val;
    }
private:
    int m_val;
};


//Pass the smart pointer by reference
void MyFunc2(unique_ptr<A>& arg)
{
    cout << arg->GetVal() << endl;
}

void uniqueTest2()
{
    unique_ptr<A> ptr = unique_ptr<A>(new A(1234));
    MyFunc2(ptr);
}

// Move the smart pointer into the function argument
// Note that in this case, the assertion will hold!
void MyFunc1(unique_ptr<A> arg)
{
    cout << arg->GetVal() << endl;
}

void uniqueTest1()
{
    unique_ptr<A> ptr = static_cast<unique_ptr<A>>(new A(1234));
    // 使用make_unique更好
    //unique_ptr<A> ptr = make_unique<A>(1234);
    
    
    // unique_ptr delete the copy constructor 
    // 所以error message是 
    // use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const    
    // std::unique_ptr<_Tp, _Dp>&)
    //MyFunc1(ptr);
    
    MyFunc1(move(ptr));
    
    // 这里的assert是成立的因为move就相当于把ownership给了MyFunc1的local copy了
    // local copy出了function scope是会delete的 所以这里也就被delete了
    assert(ptr == nullptr);
}

// It will invoke the shared_ptr's copy constructor 
// not A's 
void sharedTest1(shared_ptr<A> arg)
{
    cout << arg->GetVal() << endl;
}

// To execute C++, please define "int main()"
int main() 
{
    uniqueTest1();
    uniqueTest2();
    
    shared_ptr<A> sharedPtr = make_shared<A>(5678);
    sharedTest1(sharedPtr);
    return 0;
}