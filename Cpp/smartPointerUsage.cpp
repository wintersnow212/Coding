#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

class A
{
public:
    A (string name):m_val(name)
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
    
    string GetVal()
    {
        return m_val;
    }
    
    friend bool partnerUp(shared_ptr<A>& p1, shared_ptr<A> p2)
    {
        if (p1 == nullptr || p2 == nullptr)
        {
            return false;
        }
        
        p1->m_partner = p2;
        p2->m_partner = p1;
        
        std::cout << p1->m_val << " is now partnered with " << 
            p2->m_val << "\n";
 
        return true;
    }
    shared_ptr<A> m_partner;
private:
    string m_val;
    
};


//Pass the smart pointer by reference
void MyFunc2(unique_ptr<A>& arg)
{
    cout << arg->GetVal() << endl;
}

void uniqueTest2()
{
    unique_ptr<A> ptr = unique_ptr<A>(new A("TX"));
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
    unique_ptr<A> ptr = make_unique<A>("TX");
    
    
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

class Resource
{
public:
    //std::shared_ptr<Resource> m_ptr; // initially created empty
    std::weak_ptr<Resource> m_ptr;
    
    
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

void weakPtrTest()
{
    // 这个更好 简单的例子 weak ptr 可以用来解决cyclical reference
    // cyclical reference issue can even happen with a single std::shared_ptr
    // Weak pointer will not increase reference count
    // this is just like the reference but its memory is not persist
    cout << "--------weak Pointer------------" << endl;
    auto ptr1 = std::make_shared<Resource>();
 
    ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it
    
    //std::weak_ptr<A> weakPtr = ptr2;    
    
    cout << "--------weak Pointer------------" << endl;
}
/*
这里其实是return by value
When a value is returned by value, a copy of that value is returned to the caller
Return by value is the most appropriate最合适的 when returning variables that were declared inside the function


Because return of certain expressions, 
such as local automatic variables, are explicitly defined to return a moved object, 
if the moving operator is available.
So:

return p;
is more or less similar to:

return std::move(p);

But note that this will not work for example with a global variable.

std::unique_ptr<int> g(new int(3));
std::unique_ptr<int> ptr() {
    return g;  //  error!!!
}
*/
unique_ptr<int> returnSmartPointer()
{
    std::unique_ptr<int> p(new int(3));
    return p;
}

/* return by address 就要小心
Return by address is often used to return dynamically allocated memory to the caller:
*/
int* doubleValue(int x)
{
    int value = x * 2;
    return &value; // return value by address here
} // value destroyed here


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

    shared_ptr<A> YZ = make_shared<A>("YZ");
    {
        // use copy initialization to make another std::shared_ptr pointing to the same thing
        std::shared_ptr<A> ptr2(YZ); 
            
    } // ptr2 goes out of scope here, but nothing happens
 
    sharedTest1(YZ);
    

    auto TX = make_shared<A>("TX");
    

    //partnerUp(YZ, TX);
    cout << "--------Shared Pointer------------" << endl;
    
    // That’s it. No deallocations took place.
    
    
    weakPtrTest();
    return 0;
}
