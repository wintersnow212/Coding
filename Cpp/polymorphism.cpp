#include <iostream>
#include <vector>
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
    Base()
        : parentPublic(5)
        , parentProtectedVal(4)       
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
    
    Base(int x)
        : parentProtectedVal(x)
    {
        
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
        cout << endl; 
        cout << "--------Base call derived virtual function------start" << endl;
        // 这个v() 是会call children 因为
        // 1. 这个指针是 Base* b = static_cast<Base*>(&d1);
        // 2. v是virtual function 被inherit了
        v();
        cout << "-------Base call derived virtual function-------end" << endl;
        cout << endl; 
    }
    
    virtual void v()
    { 
        cout << "Base::v()\n";
    }
    
    virtual void speak(int i) 
    {
        cout << "Base speak" << endl;
    }
    
    
    void childOveride()
    {
        cout << "-----------Child Override Parent---------------------start\n"; 
        cout << "Base::parentShared " << parentProtectedVal << endl; 
        cout << "-----------Child Override Parent---------------------end\n"; 
        cout << endl;  
    }
    
    /*
        派生类一定要定义函数体
        但是这样做以后抽象类就不能被实例化
        既不能 Base b(1) 也不能 Base* b = new Base(1)
    */
    //virtual void speak(int i) = 0; 
    
    
    int parentPublic;
protected:

    int parentProtectedVal;
};

class Derived1 : public Base
{
public:
    Derived1()
        // If derived call do nothing the default base class constructor will be called
        // If you want to do something you can call base class constructor
        : Base(6)
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
    
    // void childOwn()
    // {
    //     cout << "-----------Parent Shared---------------------start\n"; 
    //     parentShared();
    //     cout << "-----------Parent Shared---------------------end\n"; 
    //     cout << endl;  
    // }

    void callBaseCopyConstructor()
    {
        cout << "----------Call Base Copy Constructor---------start\n";  
        Base(*this);
        cout << "----------Call Base Copy Constructor---------end\n";  
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
        cout << "Derived1 Speak" << endl;
        // // 这样是死循环不停的call 自己
        // //speak(i);
        // cout << "------Call the parent class from child--------start" << endl;
        // Base::speak(i);
        // cout << "------Call the parent class from child---------end" << endl;
        // cout << endl;  
    }
};

class Derived2 : public Base
{
public:
    Derived2()
        : Base(8)
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
        cout << endl;  
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
        dynamic_cast will only work if pointer is polymorphic (that is, if it has a virtual function).
        Static_cast downcast don’t have this requirement
        这也是dynamic cast好的地方 有检测 exclusively used for handling polymorphism.
    */ 
    
    /* Polymorphism 不一定是要指针 也可以是reference
    1. Poly不一定是base pointer 可以是reference!!
    2. 然后指向derived class
    */
    Base* b = static_cast<Base*>(&d1);
    Base& bRef = d1;
    Base  bObj = d1;
    cout << endl;
    cout << "-------Polymorphism dervied class pointer or ref---start" << endl;
    b->speak(2);
    bRef.speak(2);
    cout << "-------Polymorphism dervied class pointer or ref---end" << endl;
    cout << endl;
    cout << "-------Not Polymorphism base object ---start" << endl;
    bObj.speak(2);
    cout << "-------Not Polymorphism base object ---end" << endl;
    cout << endl;
    //d1.callBaseCopyConstructor();
    
    b->f(); 

    cout << "-------Base pointer will call base‘s virtual-----start" << endl;
    Base* baseObj = new Base();
    baseObj->v();
    cout << "-------Base pointer will call base‘s virtual-----end" << endl;
    cout << endl; 
    
    cout << "----------Pass the class object by value---------start" << endl;
    test(*b);
    cout << "----------Pass the class object by value---------end" << endl;
    
    Derived2 d2;
    
    cout << endl;
    // 这里有点像factory 或者observer pattern
    vector<Base*> baseList;
    baseList.push_back(&d1);
    baseList.push_back(&d2);
    for (auto b : baseList)
    {
        // Child 可以调用base's function 但是不是other way around
        b->childOveride();
    }
    
    b = &d2;
    b->f();
    // 这里有问题的原因是因为d2不是malloc的！！！
    // 但是这样就会有内存泄露了吧 因为D1没有释放
    // 所以用stack allocation
    //delete b;
    
    // 这样是不会call assignment operator的 
    // 因为只是指针的改变啊 并不是object的创建

    cout << "----------Copy Assignment Operator-----------start" << endl;
    Base* b2 = new Base();
    b = b2;
    
    Base b3;
    //Base::Assignment operator
    *b2 = b3;
    delete b2;
    cout << "----------Copy Assignment Operator-----------end" << endl;
    cout << endl;
    //DoBar (42);
   
    
    return 0;
}
