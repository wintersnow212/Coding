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
    static Base * instantiate(std::string const & name, T&&...args)
    {
        auto it = registry().find(name);
        if ( it == registry().end()) return 0;
        typedef Base* (*create_type)(T...);
        auto create_fun = reinterpret_cast<create_type>(it->second);
        return create_fun(args...);
    }
    virtual void f() = 0;
};
 
struct Registrar
{
    template<typename F>
    Registrar(std::string name, F func)
    {
        Base::registry()[name] = reinterpret_cast<void*>(func);
    }
};
 
class DerivedExample : public Base
{
    static Registrar registrar;
public:
    static Base * create() { return new DerivedExample; }
    virtual void f() override { std::cout << "DerivedExample" << std::endl; }
};
 
Registrar DerivedExample::registrar("DerivedExample", DerivedExample::create);
 
class AnotherExample : public Base
{
    static Registrar registrar;
    int a;
    const char *b;
public:
    AnotherExample(int a, const char *b) : a(a), b(b) {}
    static Base * create(int a, const char *b) { return new AnotherExample(a,b); }
    virtual void f() override { std::cout << "AnotherExample. a = " << a << ", b = " << b << std::endl; }
};
 
Registrar AnotherExample::registrar("AnotherExample", AnotherExample::create);
 
int main()
{
    Base * p = Base::instantiate("DerivedExample");
    Base * q = Base::instantiate("AnotherExample", 10, "Mahfuza");
    p->f();
    q->f();
 
    delete p;
    delete q;
}
 
