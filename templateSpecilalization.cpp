#include <iostream>
#include <string>
using namespace std;

// Function specialization can not be partial speciliazation
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



/****************************************************************
*        Template specilalization
*  所谓模板特例化即对于通例中的某种或某些情况做单独专门实现，
*  最简单的情况是对每个模板参数指定一个具体值，这成为完全特例化（full specialization).
*  另外，可以限制模板参数在一个范围取值或满足一定关系等，这称为部分特例化
* （partial specialization
****************************************************************/


/****************************************************************
    这个是完全实例化 Full specialization
****************************************************************/
// primary template
template<typename T>
struct checkHelper
{
    // 其实这里完全不需要call啊 这也是full specilization的体现
    //enum {value = checkHelper<T>::value};
};
template<>
struct checkHelper<int>
{
    enum {value = true};
};

template<>
struct checkHelper<string>
{
    enum {value = false};
};

template<typename T>
bool isInt()
{
    // ...implementation details when T is neither int nor std::string
    return checkHelper<T>::value;
}

/****************************************************************
    这个是局部实例化 partial specialization
****************************************************************/
// 这个也是实现了type traits
// 这里不能写成下面的形式 struct isPointer<T> 因为那样是partical specialization 
// 写成这样的话 我们就缺少full specification了
// primary template
template<typename T>
struct isPointer
{
    // 用enum就可以直接:: access value了
    enum {value = false};
};

// 这里我们说是partial的原因在于仍然是generic的typename T
template<typename T>
struct isPointer<T*>
{
    // The name of an unscoped enumeration may be omitted
    enum {value = true};
};

// 利用constexpr的表达方式 而不是利用enum
template <typename T>
struct is_pointerChecker
{
    static constexpr bool value {false};
};

template <typename T>
struct is_pointerChecker<T*>
{
    //static constexpr bool value {true};
    static const bool value = true;
};

// https://www.hackerrank.com/challenges/cpp-class-template-specialization/problem
enum class Fruit { apple, orange, pear };
enum class Color { red, green, orange };

template<typename T>
struct Traits
{
    //static std::string name(int index) { return "unknown"; }
};

// 这是可以take input argument 的static function
template <>
struct Traits<Fruit> 
{
    static string name(int idx) {
        switch (idx) {
            case 0: return "apple";
            case 1: return "orange";
            case 2: return "pear";
        }
        return "unknown";
    }    
};

template< >
struct Traits<Color>
{
    static string name(int idx) {
        switch (idx) 
        {
            case 0: return "red";
            case 1: return "green";
            case 2: return "orange";
        }
        return "unknown";
    }
};

int main()
{
    if (isInt<string>() == true)
    {
        cout << "it is int" << endl;
    }
    else
    {
        cout << "it is not int" << endl;
    }
    // declare an integer array with room for 12 integers
    StaticArray<int, 12> intArray;
    
    typedef int* T;
    
    if (isPointer<T>::value == true)
    {
        cout << "it is pointer type" << endl;
    }
    else
    {
        cout << "it is not pointer type" << endl; 
    }
    
    if (is_pointerChecker<T>::value == true)
    {
        cout << "it is pointer type" << endl;
    }
    else
    {
        cout << "it is not pointer type" << endl; 
    }
    
    // Fill it up in order, then print it backwards
    for (int count=0; count < 12; ++count)
        intArray[count] = count;
 
    for (int count=11; count >= 0; --count)
        std::cout << intArray[count] << " ";
    std::cout << '\n';
    
    

    for (int i=0; i!= 3; ++i) {
        int index1; std::cin >> index1;
        int index2; std::cin >> index2;
        cout << Traits<Color>::name(index1) << " ";
        cout << Traits<Fruit>::name(index2) << "\n";
    }
 
}

/****************************************************************************
 Partial specializtion的概念
****************************************************************************/
//Suppose there exists a KeyValuePair class with two template parameters, as follows.
template <typename Key, typename Value>
class KeyValuePair {};


// The following is an example of a class that defines an explicit full template specialization of KeyValuePair by pairing integers with strings. The class type retains the same name as the original version.
template <>
class KeyValuePair<int, std::string> {};

// The next is an example of partial specialization of KeyValuePair with the same name as the original version and one specialized template parameter.
// 这里就是Key 保持generic 但是Value specifized to std::string!!!!
template <typename Key>
class KeyValuePair<Key, std::string> {};

/****************************************************************************
 这个是模板型模板参数； 这里一定要用class 而不是template
****************************************************************************/
template<class T1, class T2, int I>
class A {};            // primary template
 
// 下面的这些都是上面primiar template的specification
// 其实写法也很好想！！！ 我们那个specify了 就不需要哪个的template type了！！！
template<class T, int I>
class A<T, T*, I> {};  // #1: partial specialization where T2 is a pointer to T1
 

template<class T, class T2, int I>
class A<T*, T2, I> {}; // #2: partial specialization where T1 is a pointer
 
template<class T>
class A<int, T*, 5> {}; // #3: partial specialization where T1 is int, I is 5,
                        //     and T2 is a pointer
 
template<class X, class T, int I>
class A<X, T*, I> {};   // #4: partial specialization where T2 is a pointer