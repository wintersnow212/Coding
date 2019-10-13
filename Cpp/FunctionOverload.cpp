#include<iostream>
using namespace std;


// What’s the meaning of, Warning: Derived::f(char) hides Base::f(double)? ¶ Δ
class B {
public:
    int f(int i) { cout << "f(int): "; return i+1; }
    // ...
};
class D : public B {
public:
    // 理论上来说derived class 是可以call base class 非privated函数的
    // 所以当然call pd->f(2) 理论上应该是call base的函数
    // 但是由于c++ 在derived class中function overload造成的hiding
    // http://www.parashift.com/c++-faq-lite/hiding-rule.html
    // 所以如果没有using B::f; 就不会call到base
    using B::f;
    double f(double d) { cout << "f(double): "; return d+1.3; }
    // ...
};
int main()
{
    D* pd = new D;
    cout << pd->f(2) << '\n';
    cout << pd->f(2.3) << '\n';
    delete pd;
}
