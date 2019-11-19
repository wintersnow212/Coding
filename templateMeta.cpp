#include <iostream>
using namespace std;

/****************************************************************
 * template meta programming TMP的例子
 * http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html
 * 好处： The use of templates can be thought of as compile-time execution
 *  Move computations from run-time to compile-time
 *  Types of metaprograms
    1.Compile-time numeric calculations
    2.Code generation using numeric calculations 
    3.“Self-adapting” code
    4.Type manipulations
    5.Checking types for certain properties — Converting types to other types
 ****************************************************************/


/****************************************************************
Compile-time numeric calculations
****************************************************************/
template <unsigned int n>
struct factorial 
{
    enum 
    { 
        value = n * factorial<n - 1>::value 
    };
};

// 完全实例化
template <> // The following is a template class with no templated parameters
struct factorial<0>  // template specialization 完全特例化
{
    enum { value = 1 };
};

/****************************************************************
if (condition) 
   statement1;
else 
   statement2;
****************************************************************/
// Class declarations
template<bool C>
class _name 
{ 
};

template<>
class _name<true> 
{
public:
    static inline void f()
    { std::cout << "TRUE" << std::endl; }         // true case
};

template<>
class _name<false> {
public:
    static inline void f()
    { std::cout << "FALSE" << std::endl; }         // false case
};


/****************************************************************
 Small types check utility
****************************************************************/
template <typename T> 
struct SizeOfT 
{ 
    enum { value = sizeof(T) };
};

template <typename T>
struct is_pointer_type
{
    enum { value = false };
};

template <typename T>
struct is_pointer_type<T*>
{
    enum { value = true };
};

template <typename T>
bool _is_pointer(const T&)
{
    return is_pointer_type<T>::value;
}


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
int main()
{
    typedef int* A;
    cout << "Check if it is the pointer type " << is_pointer_type<A>::value << endl;
    
    int a = 0;
    do_stuff(a);
    Test t;
    do_stuff(t);

    // Test t;
    // do_stuff<Test>(t);
    
    // Replacement for 'if/else' statement:
    const bool flag = true;
    _name<flag == true>::f();
    
    
    cout << SizeOfT<int>::value << endl;
    
    
    std::cout << factorial<4>::value << std::endl;
}
