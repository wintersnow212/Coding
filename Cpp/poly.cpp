#include <iostream>
using namespace std;

class Base
{
public:
    /*
    Default constructor
    如果有自己的构造函数 这个时候就没有compiler-defined的default constructor了 
    就要自己写默认构造函数了, 不写如果你
    */
    Base()              
    { 
        cout << "Base::Base()\n"; 
    }
    
    Base(const Base& b)
    {
        cout << "Base::Copy constructor\n";
    }
    
    Base& operator = (const Base &a) 
    { 
        cout<<"Base::Assignment operator\n"; 
        return *this; 
    }  
    virtual ~Base()         
    { 
        cout << "Base::~Base()\n"; 
    }
    
    void f()
    { 
        cout << "Base::f()\n"; 
    }
    virtual void v()
    { 
        cout << "Base::v()\n"; 
    }
};

class Derived1 : public Base
{
public:
    Derived1()
    { 
        cout << "Derived1::Derived1()\n"; 
    }
    
    virtual ~Derived1()
    { 
        cout << "Derived1::~Derived1()\n"; 
    }
    
    void f()
    { 
        cout << "Derived1::f()\n"; 
    }

    virtual void v()
    { 
        cout << "Derived1::v()\n"; 
    }
};

class Derived2 : public Base
{
public:
    Derived2()
    { 
        cout << "Derived2::Derived2()\n"; 
    }
    
    virtual ~Derived2()
    { 
        cout << "Derived2::~Derived2()\n"; 
    }
    
    void f()
    { 
        cout << "Derived2::f()\n"; 
    }

    virtual void v()
    { 
        cout << "Derived2::v()\n"; 
    }
};

/*
1. 这里会call copy constructor因为pass by value 
   建立一个Base的copy 如果是pass by reference 就不会invoke了
2. 因为不是pointer type所以没有polymophrim
3. local copy出了scope以后就会被free 所以destructor会被call
*/
void test(Base base)
{
    base.v();
}

int main()
{
    /*
    首先你是一个base的指针!!! poly一定是用指针实现的！！！
    然后指向derived class
    */
    //Base* b = new Derived1();
    Derived1 d1;
    Base* b = &d1;
    b->f();
    b->v();
    test(*b);
    Derived2 d2;
    //Derived2* d2 = new Derived2();
    b = &d2;
    b->f();
    // 这里有问题的原因是因为d2不是malloc的！！！
    // 但是这样就会有内存泄露了吧 因为D1没有释放
    // 所以用stack allocation
    //delete b;
    
    // 这样是不会call assignment operator的 
    // 因为只是指针的改变啊 并不是object的创建
    cout << "----------test------------" << endl;
    Base* b2 = new Base();
    b = b2;
    
    Base b3;
    //Base::Assignment operator
    *b2 = b3;
    delete b2;
    cout << "----------test-----------" << endl;
    return 0;
}
