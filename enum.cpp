#include <iostream>
using namespace std;


/****************************************************************
Unscoped enum
****************************************************************/
// When an unscoped enumeration is a class member, its enumerators may be accessed using class member access operators . and ->:
struct X
{
    enum direction { left = 'l', right = 'r' };
};

// enum that takes 16 bits
enum smallenum: std::int16_t
{
    a,
    b,
    c
};
 
 
// color may be red (value 0), yellow (value 1), green (value 20), or blue (value 21)
enum color
{
    red,
    yellow,
    green = 20,
    blue
};
 
// altitude may be altitude::high or altitude::low
enum class altitude: char
{ 
     high='h',
     low='l', // C++11 allows the extra comma
}; 
 
// the constant d is 0, the constant e is 1, the constant f is 3
enum
{
    d,
    e,
    f = e + 2
};
 
//enumeration types (both scoped and unscoped) can have overloaded operators
std::ostream& operator<<(std::ostream& os, color c)
{
    switch(c)
    {
        case red   : os << "red";    break;
        case yellow: os << "yellow"; break;
        case green : os << "green";  break;
        case blue  : os << "blue";   break;
        default    : os.setstate(std::ios_base::failbit);
    }
    return os;
}
 
std::ostream& operator<<(std::ostream& os, altitude al)
{
    return os << static_cast<char>(al);
}
 
// To execute C++, please define "int main()"
int main() {
    X x;
    X* p = &x;

    int a = X::direction::left; // allowed only in C++11 and later
    int b = X::left;
    int c = x.left;
    int d = p->left;
    
    
    color col = red;
    altitude alt;
    alt = altitude::low;
 
    std::cout << "col = " << col << '\n'
              << "a = "   << alt   << '\n'
              << "f = "   << f   << '\n';
    return 0;
}