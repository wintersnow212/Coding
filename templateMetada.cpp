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


// template<typename T> struct is_pointer_helper     : std::false_type {};
// template<typename T> struct is_pointer_helper<T*> : std::true_type {};
// template<typename T> struct my_is_pointer : is_pointer_helper<typename std::remove_cv<T>::type> {};

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
bool my_is_pointer(const T&)
{
    return is_pointer_type<T>::value;
}

int main()
{
    typedef int* A;
    cout << "Check if it is the pointer type " << is_pointer_type<A>::value << endl;
    
    // Replacement for 'if/else' statement:
    const bool flag = true;
    _name<flag == true>::f();
    
    
    cout << SizeOfT<int>::value << endl;
    
    
    std::cout << factorial<4>::value << std::endl;
}
