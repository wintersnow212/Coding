#include <iostream>

/****************************************************************
*  Non-type template parameter
****************************************************************/
template<typename T, int size>
class StaticArray
{
private:
    // The non-type parameter controls the size of the array
    T m_array[size];
 
public:
    T* getArray();
    
    T& operator[](int index)
    {
        return m_array[index];
    }
};
 
// Showing how a function for a class with a non-type parameter is defined outside of the class
template <class T, int size>
T* StaticArray<T, size>::getArray()
{
    return m_array;
}


/****************************************************************************
*     typename keyword vs class keyword
*  Unlike type template parameter declaration, template template 
*  parameter 
*  declaration can only use the keyword class and not typename.
* (until C++17)
* The first one is in the case of dependent types. 
* typename is used to declare when you are referencing a nested type that 
* depends on another template parameter, such as the typedef in this example:
* 
* template<typename param_t>
* class Foo
* {
*     typedef typename param_t::baz sub_t;
* };
* The second one you actually show in your question, though you might not realize it:
* 
* template <template<typename, typename> class Container, typename Type>
* When specifying a template template, the class keyword MUST be used as above -- 
* it is not interchangeable with typename in this case (note: since C++17 both keywords are allowed in this case).
* 
* You also must use class when explicitly instantiating a template:

template class Foo<int>;
**********************************************************************/

/****************************************************************
*  template template parameter
****************************************************************/



/****************************************************************
*        Template specilalization
*  所谓模板特例化即对于通例中的某种或某些情况做单独专门实现，
*  最简单的情况是对每个模板参数指定一个具体值，这成为完全特例化（full specialization).
*  另外，可以限制模板参数在一个范围取值或满足一定关系等，这称为部分特例化
* （partial specialization
****************************************************************/

// However if the function’s observable behavior is consistent for all the T types with the differences 
// limited to implementation details, then you can proceed. 
// Let’s proceed with an example of this (conceptual only; not C++):
template<typename T>
void foo(const T& x)
{
  switch (typeof(T)) {  // Conceptual only; not C++
    case int:
      // ...implementation details when T is int
      break;
    case std::string:
      // ...implementation details when T is std::string
      break;
    default:
      // ...implementation details when T is neither int nor std::string
      break;
  }
}

// One way to implement the above is via template specialization. Instead of a switch-statement, 
// you end up breaking up the code into separate functions. 
// The first function is the default case — the code to be used when T is anything other than int or std::string:
template<typename T>
void foo(const T& x)
{
  // ...implementation details when T is neither int nor std::string
}

// 下面的这两个完全specialization后 我们遇到这样的type直接就进来了！！！！
// Next are the two specializations, first for the int case…
template<>
void foo<int>(const int& x)
{
  // ...implementation details when T is int
}

// …and next for the std::string case…
template<>
void foo<std::string>(const std::string& x)
{
  // ...implementation details when T is std::string
}
//That’s it; you’re done. The compiler will automagically select the correct specialization when it sees which T you are using.


template<typename T, int i> class cp00; // 用于模板型模板参数

// 通例
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP;

// 完全特例化specilalization 全部都是直接输入
template<>
class TMP<int, float, 2, cp00>;

// 第一个参数有const修饰
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP<const T1, T2, i, CP>;

/****************************************************************
 * template meta programming TMP的例子
 * http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html
 * 好处： The use of templates can be thought of as compile-time execution
 *  so it is easier to be optimized by the compiler
 ****************************************************************/
// non-typed template   
template <unsigned int n>
struct factorial {
    enum { value = n * factorial<n - 1>::value };
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

int main()
{
    // Replacement for 'if/else' statement:
    const bool flag = true;
    _name<flag == true>::f();
    std::cout << factorial<4>::value << std::endl;
    
     // declare an integer array with room for 12 integers
    StaticArray<int, 12> intArray;
 
    // Fill it up in order, then print it backwards
    for (int count=0; count < 12; ++count)
        intArray[count] = count;
 
    for (int count=11; count >= 0; --count)
        std::cout << intArray[count] << " ";
    std::cout << '\n';
 
    // declare a double buffer with room for 4 doubles
    StaticArray<double, 4> doubleArray;
 
    for (int count=0; count < 4; ++count)
        doubleArray[count] = 4.4 + 0.1*count;
 
    for (int count=0; count < 4; ++count)
        std::cout << doubleArray[count] << ' ';
     return 0;
}
