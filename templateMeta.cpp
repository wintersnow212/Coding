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
template<unsigned int n>
struct factorial
{
    enum
    {
        // SB了enum里面没有; 是data member啊！！！所以应该最多是,
        value = n * (factorial<n-1>::value)
    };
};

// 完全实例化
template<>
struct factorial<0>
{
    enum
    {
        value = 1
    };
};


template<int N>
struct fib
{
    enum { val = fib<N-1>::val + fib<N-2>::val};
};

template<>
struct fib<0>
{
    enum {val = 0};
};

template<>
struct fib<1>
{
    enum {val = 1};  
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

// 这里利用了partial specification 去实现了type_traits
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

//这里不能用typename 很简单因为typename只适用于type啊 从字面意思！！！！
template<int T>
struct isEvenNumber
{
    static constexpr bool value = (T&1) ? false : true;
};




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
    
    // 调用meta template 必须要用const啊 毕竟是compile time的计算
    std::cout << factorial<4>::value << std::endl;
    std::cout << fib<6>::val << std::endl;
    
    
     if (isEvenNumber<16>::value == true)
    {
        cout << "Even number" << endl;
    }
    else
    {
        cout << "Odd number" << endl;
    }
}
