#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

class A
{
public:
    A (int val):m_val(val)
    {
        cout << "constructor called" << endl;
    }
    
    A (const A& a)
    {
        cout << "Copy constructor called" << endl;
    }
    
    ~A()
    {
        cout << "Destructor called" << endl;
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
    //unique_ptr<A> ptr = static_cast<unique_ptr<A>>(new A(1234));
    // 使用make_unique更好 因为safety exception 
    unique_ptr<A> ptr = make_unique<A>(1234);
    
    
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
    
    // Reference counting 
    // make shared --- allocate control block of memory for reference count
    // construct them together
    cout << "--------Shared pointer------------" << endl;
    
    // allocate a Resource object and have it owned by std::shared_ptr

    shared_ptr<A> sharedPtr = make_shared<A>(5678);
    {
        // use copy initialization to make another std::shared_ptr pointing to the same thing
        std::shared_ptr<A> ptr2(sharedPtr); 
        

        // Weak pointer will not increase reference count
        // this is just like the reference but its memory is not persist
        std::weak_ptr<A> weakPtr = ptr2;    
            
    } // ptr2 goes out of scope here, but nothing happens
 
    sharedTest1(sharedPtr);
    cout << "--------Shared pointer------------" << endl;
    return 0;
}
