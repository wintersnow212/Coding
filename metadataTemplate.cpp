#include <iostream>

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

// template specilalization
template<typename T, int i> class cp00; // 用于模板型模板参数
// 通例
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP;

// 完全特例化
template<>
class TMP<int, float, 2, cp00>;

// 第一个参数有const修饰
template<typename T1, typename T2, int i, template<typename, int> class CP>
class TMP<const T1, T2, i, CP>;

/****************************************************************
 * template meta programming TMP的例子
 * http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html
 ****************************************************************/
// non-typed template   
template <unsigned int n>
struct factorial {
    enum { value = n * factorial<n - 1>::value };
};
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
