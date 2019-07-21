#include <iostream>
using namespace std;

class Foo
{
public:
    // single parameter constructor, can be used as an implicit conversion
    explicit Foo (int foo) : m_foo (foo) 
    {

    }

    int GetFoo () { return m_foo; }

private:
  int m_foo;
};

void DoBar (Foo foo)
{
    foo.GetFoo ();
}


//https://www.tutorialcup.com/cplusplus/polymorphism.htm#static-cast
class Base
{
public:
    /*
    Default constructor
    如果有自己的构造函数 这个时候就没有compiler-defined的default constructor了 
    就要自己写默认构造函数了, 不写如果你
    */
    Base():
        parentPublic(5),
        parentSharedVal(4)       
    { 
        /*
        This constructor yields the same end result as the one above, but it
        will often be more efficient. The assignment-based version first called
        default constructors to initialize theName, theAddress, and thePhones,
        then promptly assigned new values on top of the default-constructed
        ones. All the work performed in those default constructions was
        therefore wasted. The member initialization list approach avoids that
        problem, because the arguments in the initialization list are used as
        constructor arguments for the various data members. In this case,
        theName is copy-constructed from name, theAddress is copy-constructed
        from address, and thePhones is copy-constructed from phones.
        For most types, a single call to a copy constructor is more efficient —
        sometimes much more efficient — than a call to the default constructor
        followed by a call to the copy assignment operator.       
           
        */
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
        cout << "----------virtual in base------------" << endl;
        cout << "Base::f()\n"; 
        // 这个v() 是会call children 因为
        // 1. 这个指针是 Base* b = static_cast<Base*>(&d1);
        // 2. v是virtual function 被inherit了
        v();
        cout << "----------virtual in base------------" << endl;
    }
    
    virtual void v()
    { 
        cout << "Base::v()\n"; 
    }
    
    virtual void speak(int i) 
    {
        cout << "Base speak" << endl;
    }
    
    /*
        派生类一定要定义函数体
        但是这样做以后抽象类就不能被实例化
        既不能 Base b(1) 也不能 Base* b = new Base(1)
    */
    //virtual void speak(int i) = 0; 
    
    
    int parentPublic;
protected:
    void parentShared()
    {
        cout << "Base::parentShared\n"; 
    }
    
    int parentSharedVal;
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
        static_cast<Base*>(this)->f();
    }
    
    void childOwn()
    {
        parentShared();
        cout << parentSharedVal << endl;
        cout << "Derived1::childOwn\n"; 
    }

    void callBaseCopyConstructor()
    {
        cout << "Call Base Copy Constructor\n";  
        Base(*this);
        cout << "Call Base Copy Constructor\n";  
    }
    virtual void v()
    { 
        cout << "Derived1::v()\n"; 
    }
    
    
    /*
    Virtual function 需要保证override的parameter必须相同
    加一个override来确保 避免出现以下情况！！！！
    struct Base 
    {
        virtual void some_func(float);
    };
    struct Derived : Base 
    {
        virtual void some_func(int);
    };
    */
    virtual void speak(int i) override
    {
        cout << "----------Call the parent class from child------------" << endl;
        cout << "Derived1 Speak" << endl;
        // 这样是死循环不停的call 自己
        //speak(i);
        Base::speak(i);
        cout << "----------Call the parent class from child------------" << endl;
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
    
    virtual void speak(int i) override
    {
        cout << "Derived2 Speak" << endl;
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
    1. 首先你是一个base的指针!!! poly一定是用指针实现的！！！
    2. 然后指向derived class
    */
    //Base* b = new Derived1();
    Derived1 d1;
    // 其实accesss specifier 是表示access scope 
    // private只能在own class scope; protected可以在children class scope
    // public 可以在
    //cout << d1.parentSharedVal << endl;
    cout << d1.parentPublic << endl;
    
    
     /* 
        down-cast and up-cast
        Overall, upcast always works but downcast has less working case 
        e.g. dogs and cats are animal, but animal is not just dogs or animals
        An "up-cast" is always valid with both static_cast and dynamic_cast
        and also without any cast, as an "up-cast" is an implicit conversion.
        
        You could cast from a point reference (or pointer) to a subpoint reference (or pointer), if the referred object were 
        actually of type subpoint: 
        dynamic_cast will only work if point is polymorphic (that is, if it has a virtual function).
        Static_cast downcast don’t have this requirement
        这也是dynamic cast好的地方 有检测 exclusively used for handling polymorphism.
    */ 
    Base* b = static_cast<Base*>(&d1);
    b->speak(2);
    
    //Derived2* d2 = new Derived2();
    // Child 可以调用base's function 但是不是other way around
    //b->childOwn();
    d1.childOwn();
    d1.callBaseCopyConstructor();
    b->f();
    b->v();
    
    
    cout << "----------Pass the class object by value------------" << endl;
    test(*b);
    cout << "----------Pass the class object by value------------" << endl;
    
    Derived2 d2;
   
    b = &d2;
    b->f();
    // 这里有问题的原因是因为d2不是malloc的！！！
    // 但是这样就会有内存泄露了吧 因为D1没有释放
    // 所以用stack allocation
    //delete b;
    
    // 这样是不会call assignment operator的 
    // 因为只是指针的改变啊 并不是object的创建

    cout << "----------Assignment Op------------" << endl;
    Base* b2 = new Base();
    b = b2;
    
    Base b3;
    //Base::Assignment operator
    *b2 = b3;
    delete b2;
    cout << "----------Assignment Op-----------" << endl;
    
    cout << "----------explicit constructor------------" << endl;
    //DoBar (42);
   
    return 0;
}
