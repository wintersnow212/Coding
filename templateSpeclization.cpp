#include <iostream>
using namespace std;
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
// template<typename T>
// void foo(const T& x)
// {
//   switch (typeof(T)) {  // Conceptual only; not C++
//     case int:
//       // ...implementation details when T is int
//       break;
//     case std::string:
//       // ...implementation details when T is std::string
//       break;
//     default:
//       // ...implementation details when T is neither int nor std::string
//       break;
//   }
// }

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

/****************************************************************************
*     typename keyword vs class keyword
*  Unlike type template parameter declaration, template template 
*  parameter  declaration can only use the keyword class and not typename.(until C++17)
* 
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
// 这个是模板型模板参数； 这里一定要用class 而不是template
// 通例
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP;

// 完全特例化specilalization 全部都是直接输入
template<typename T, int i> class cp00; 
template<>
class TMP<int, float, 2, cp00>;

// 第一个参数有const修饰
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP<const T1, T2, i, CP>;

int main()
{
     // declare an integer array with room for 12 integers
    StaticArray<int, 12> intArray;
 
    // Fill it up in order, then print it backwards
    for (int count=0; count < 12; ++count)
        intArray[count] = count;
 
    for (int count=11; count >= 0; --count)
        std::cout << intArray[count] << " ";
    std::cout << '\n';
 
}
