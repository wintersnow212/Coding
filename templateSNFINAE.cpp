#include <iostream>
#include <experimental/type_traits>
using namespace std;
/****************************************************************
 SFINAE and enable_if
 
 关于这个 enable_if 是怎么工作的，语法为什么这么丑，我来解释一下：

首先，substitution只有在推断函数类型的时候，才会起作用。推断函数类型需要参数的类型，所以，typename std::enable_if<std::is_integral<T>::value>::type 这么一长串代码，就是为了让enable_if参与到函数类型中；

其次，is_integral<T>::value返回一个布尔类型的编译器常数，告诉我们它是或者不是一个integral
enable_if<C>的作用就是，
如果这个C值为True，那么type就会被推断成一个void或者是别的什么类型，让整个函数匹配后的类型变成 void inc_counter<int>(int & counterInt, vo
id* dummy = nullptr); 如果这个值为False，那么enable_if<false>这个特化形式中，压根就没有这个::type，于是substitution就失败了 —— 所以这个函数原型根本就不会被产生出来。
****************************************************************/

// template <bool, typename T = void>
// struct enable_if
// {
    
// };

// template <typename T>
// struct enable_if<true, T> 
// {
//     typedef T type;
// };

template <typename T, 
          typename std::enable_if<std::is_integral<T>::value,T>::type* = nullptr>
void do_stuff(T t) {
  std::cout << "do_stuff integral\n";
    // an implementation for integral types (int, char, unsigned, etc.)
}

template <typename T,
          typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>

void do_stuff(T t) {
    // an implementation for class types
    std::cout << "do_stuff for class \n";
}

// universal reference
template <typename T>
void f(T&& param)
{
    cout << "param" << endl;
}

class Test
{
    
};

/****************************************************************
Detection idiom  is also a way to SFINAE
****************************************************************/
struct Yes { typedef void type; };
class Huh 
{ 
    // default is private so cannot access
    typedef void type; 
}; 
struct No { /* no ’type’ member typedef */ };

template <typename T>
    using has_type = typename T::type;

// static_assert(std::experimental::is_detected<has_type, Yes>::value );
// static_assert(!std::experimental::is_detected<has_type, Huh>::value );
// static_assert(!std::experimental::is_detected<has_type, No>::value );



int main()
{
    int a = 0;
    do_stuff(a);
    Test t;
    do_stuff(t);
    
    if (std::experimental::is_detected<has_type, Yes>::value == 1)
    {
        cout << "Yes, has type" << endl;
    }
    
    if (std::experimental::is_detected<has_type, No>::value == false)
    {
        cout << "No, there is no type" << endl;
    }
    
    if (std::experimental::is_detected<has_type, Huh>::value == false)
    {
        cout << "Huh, doesn't have type because it is private" << endl;
    }
}
