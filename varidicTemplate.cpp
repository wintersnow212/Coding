#include <unordered_map>
#include <string>
#include <iostream>
 
class Base
{
public:
    typedef std::unordered_map<std::string, void*> registry_map;
 
    virtual ~Base() = default;
 
    static registry_map & registry()
    {
        static registry_map impl;
        return impl;
    }
    
    template<typename ...T>
    static Base* instantiate(std::string const & name, T&&...args)
    {
        auto it = registry().find(name);
        if ( it == registry().end()) 
            return 0;
        
        typedef Base* (*create_type)(T...);
        auto create_fun = reinterpret_cast<create_type>(it->second);
        return create_fun(args...);
    }
    virtual void f() = 0;
};

// 专门用来负责register的
struct Registrar
{
    template<typename F>
    Registrar(std::string name, F func)
    {
        Base::registry()[name] = reinterpret_cast<void*>(func);
    }
};
 
class FirstExample : public Base
{
public:
    static Base * create() 
    { 
        return new FirstExample; 
    }
    
    virtual void f() override { std::cout << "FirstExample" << std::endl; }
private:
    static Registrar registrar;
};
// 这样不用call register 自动就register了！！！ 太屌！！！！
Registrar FirstExample::registrar("FirstExample", FirstExample::create);
 
class SecondExample : public Base
{
    static Registrar registrar;
    int a;
    const char *b;
public:
    SecondExample(int a, const char *b) 
        : a(a)
        , b(b) 
    {}
    
    static Base * create(int a, const char *b) 
    { 
        return new SecondExample(a,b); 
    }
    virtual void f() override 
    { 
        std::cout << "SecondExample. a = " << a << ", b = " << b << std::endl; 
    }
};
 
Registrar SecondExample::registrar("SecondExample", SecondExample::create);

class ThridExample : public Base
{
public:
//     ThridExample (const std::string& str)
//         : m_str(str)
//     {
    
//     }
    ThridExample (const char * str)
        : m_str(str)
    {
    
    }
    
    // static Base* create(const std::string str)
    // {
    //     return new ThridExample(str);
    // }
    static Base* create(const char * str)
    {
        return new ThridExample(str);
    }
    
    virtual void f() override
    {
        std::cout << "ThirdExample. string = " << m_str << std::endl; 
    }
private:
    static Registrar registrar;
    //std::string m_str;
    const char * m_str;
};

Registrar ThridExample::registrar("ThirdExample", ThridExample::create);

// https://adl1995.github.io/using-variadic-templates-with-lambda-expressions-in-c-for-constrained-optimization.html

void print(void)
{

}

//C++ varaidic templates require writing a base case and a recursive case
template <typename First, typename ...Rest>
void print(const First& first, Rest&&... Args)
{
    std::cout << first << std::endl;
    std::cout << "cut" << std::endl;
    print(Args...);
}

int main()
{
    Base * p = Base::instantiate("FirstExample");
    Base * q = Base::instantiate("SecondExample", 10, "Mahfuza");
    Base * r = Base::instantiate("ThirdExample", "test");
    p->f();
    q->f();
    r->f();
    delete p;
    delete q;
    delete r;
    auto variadic_generic_lambda = [] (auto... param)
    {
        print(param...);
    };

    variadic_generic_lambda(42, "lol", 4.3);
}

 
