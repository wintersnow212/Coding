
// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>
using namespace std;


// 函数模板
template<typename T>
bool equivalent(const T& a, const T& b){
    return !(a < b) && !(b < a);
}
// 类模板
template<typename T=int> // 默认参数
class bignumber
{
public:
    bignumber(T a) : _v(a) { }
    inline bool operator<(const bignumber& b) const; // 等价于 (const bignumber<T>& b)
private:
    T _v;
};

// 在类模板外实现成员函数
template<typename T>
bool bignumber<T>::operator<(const bignumber& b) const{
    return _v < b._v;
}

// Template hash
template <typename T1, typename T2>
class pair_hash 
{   
public:
    //std::size_t operator () (const std::pair<T1,T2> &p) const 
    std::size_t operator () (const pair<T1, T2> &p) const 
    {
        // 这里1.不能忘了define template type 2.不能忘了()
        // auto h1 = std::hash<T1>{}(p.first);
        // auto h2 = std::hash<T2>{}(p.second);
        // 感觉()是构建了一个hash object!!!
        auto h1 = std::hash<T1>()(p.first);
        auto h2 = std::hash<T2>()(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

typedef std::tuple<int, int, int> keyType;
struct key_hash
{
    std::size_t operator()(const keyType& k) const
    {
        // 这是基本写法但是这样写的话如果有多个type就得declare多个 hash variable
        std::hash<int> intHash; 
        //return std::hash<int>{}(get<0>(k)) ^ std::hash<int>{}(get<1>(k)) ^ std::hash<int>{}(get<2>(k));
        return intHash(get<0>(k)) ^ intHash(get<1>(k)) ^ intHash(get<2>(k));
    }
};

template <typename Key>
struct myHash
{
    std::size_t operator()(const Key& k) const
    {
      using std::hash;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      // return ((hash<string>()(k.first)
      //          ^ (hash<string>()(k.second) << 1)) >> 1)
      //          ^ (hash<int>()(k.third) << 1);
        
        return hash<Key>()(k);
    }
};

/* Function template
 One final note: Because the function argument passed in for type T could be a class type, and it’s generally not a good idea to pass classes by value, it would be better to make the parameters and return types of our templated function const references:
When to use return by reference:
When returning a reference parameter
When returning an element from an array that was passed into the function
When returning a large struct or class that will not be destroyed at the end of the function (e.g. one that was passed in)
When not to use return by reference:
When returning variables that were declared inside the function or parameters that were passed by value (use return by value)
When returning a built-in array or pointer value (use return by address)
*/
template<typename T>
const T& test(const T& x, const T& y)
{
    /*Operators, function calls, and function templates
Template functions will work with both built-in types (e.g. char, int, double, etc…) and classes, with one caveat. When the compiler compiles the template instance, it compiles it just like a normal function. In a normal function, any operators or function calls that you use with your types must be defined, or you will get a compiler error.*/
    return (x > y) ? x : y;
}

template<typename T1, typename T2>
bool test(const T1& x, const T2& y)
{
    return (x > y);
}


/*****************************
Template non-type parameters
A template non-type parameter is a special type of parameter that does not substitute for a type, 
but is instead replaced by a value. A non-type parameter can be any of the following:

A value that has an integral type or enumeration
A pointer or reference to a class object
A pointer or reference to a function
A pointer or reference to a class member function
std::nullptr_t
******************************/
template <class T, int size> // size is the non-type parameter
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


// To execute C++, please define "int main()"
int main() {
  
    unordered_map<pair<string, string>, int, pair_hash<string, string>>
        distMap = 
    //std::unordered_map<pair,int,pair_hash> unordered_map =
        {
     {{"C++", "C++11"}, 2011},
     {{"C++", "C++14"}, 2014},
     {{"C++", "C++17"}, 2017},
     {{"Java", "Java 7"}, 2011},
     {{"Java", "Java 8"}, 2014},
     {{"Java", "Java 9"}, 2017}};

    for (auto const &entry: distMap)
    {
         auto key_pair = entry.first;
         std::cout << "{" << key_pair.first << "," << 
             key_pair.second << "}, " << entry.second << '\n';
    }
    
    // Call the templated function
    // Using a function template is extremely straightforward -- you can use it just like any other function
    cout << test(3, 4) << endl;
    cout << test(3, 4.3) << endl;
    
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


/* 
Your previous Python 2 content is preserved below:

# This is a sandbox to experiment with CoderPad's execution capabilities.
# It's a temporary, throw-away session only visible to you.

def say_hello():
    print 'Hello, World'

for i in xrange(5):
    say_hello()

 */
