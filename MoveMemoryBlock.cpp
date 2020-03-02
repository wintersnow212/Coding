#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;


/****************************************************************************************
//https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
//https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
// 可能需要disable copy constructor
// copy&&move construct 不用free 但是感觉copy assignment 和move assignment需要

A move constructor is called:

when an object initializer is std::move(something)
when an object initializer is std::forward<T>(something) and T is not an lvalue reference type (useful in template programming for "perfect forwarding")
when an object initializer is a temporary and the compiler doesn't eliminate the copy/move entirely
when returning a function-local class object by value and the compiler doesn't eliminate the copy/move entirely
when throwing a function-local class object and the compiler doesn't eliminate the copy/move entirely

****************************************************************************************/

template<typename T>
class MemoryBlock
{
public:

   // Simple constructor that initializes the resource.
   explicit MemoryBlock(size_t length)
      : _length(length)
      , _data(new T[length])
   {
      std::cout << "In MemoryBlock(size_t). length = "
                << _length << "." << std::endl;
   }

   // Destructor.
   ~MemoryBlock()
   {
      std::cout << "In ~MemoryBlock(). length = "
                << _length << ".";

      if (_data != nullptr)
      {
         std::cout << " Deleting resource.";
         // Delete the resource.
         delete[] _data;
      }

      std::cout << std::endl;
   }

   // Copy constructor.
   MemoryBlock(const MemoryBlock& other)
      : _length(other._length)
      , _data(new T[other._length])
   {
      std::cout << "In MemoryBlock(const MemoryBlock&). length = "
                << other._length << ". Copying resource." << std::endl;

      std::copy(other._data, other._data + _length, _data);
   }

    // Copy assignment operator.
    // 可以 用copy and swap 不过这里就不看了把
    MemoryBlock& operator=(const MemoryBlock& other)
    {
        std::cout << "In operator=(const MemoryBlock&). length = "
                << other._length << ". Copying resource." << std::endl;

        if (this != &other)
        {
            // Free the existing resource.
            delete[] _data;

            _length = other._length;
            _data = new T[_length];
            //std::copy(other._data, other._data + _length, _data);
            memcpy(_data, other._data, _length * sizeof(T));
        }
        return *this;
    }
    
    // Move constructor. Unlike the default copy constructor, 
    // the compiler does not provide a default move constructor
    // 特别注意这里move使用的rValue reference  原因是要把temporary object(i.e. other)的        
    // ownership transfer 给自己
     // Move semantics works because it enables resources to be transferred from 
    // temporary objects that cannot be referenced elsewhere in the program.
    // A move constructor enables the resources owned by an rvalue object to 
    // be moved into an lvalue without copying.
    
    // 实例：
    // f ("foo");
    // To see why, consider the first call to f() in the above example. 
    // Here is what will happen: a temporary (rvalue) string will be initialized with 
    // "foo" and then passed to f(). 
    // f(), in turn, will create a copy of the passed temporary and,
    // once it returns, the temporary will be destroyed.
    // 你pass in lValue的时候只要用const& 就避免copy了呀
    // 综上所述 提升的地方就在于当你是pass in temporart object的时候可以避免copy
    MemoryBlock (MemoryBlock&& other)
        :_length(other._length)
        ,_data(other._data)
        
    {
        std::cout << "In MemoryBlock(MemoryBlock&&). length = "
         << other._length << ". Moving resource." << std::endl;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other._data   = nullptr;
        other._length = 0;
    }

    /* 涨姿势了！！！！！！
       If you provide both a move constructor and a move assignment operator 
       for your class, you can eliminate redundant code by writing the move 
       constructor to call the move assignment operator. The following example 
       shows a revised version of the move constructor that calls the move 
       assignment
    */
    // Move constructor.
//     MemoryBlock(MemoryBlock&& other)
//        : _length(0)
//        , _data(nullptr)
          
//     {
//         std::cout << "In MemoryBlock(MemoryBlock&&). length = "
//         << other._length << ". Moving resource." << std::endl;
//         *this = std::move(other);
//     }
    
    // Move assignment
    MemoryBlock& operator=(MemoryBlock&& other)
    {
        if (this != &other)
        {
            // 这里要是不free 就move的话 那么之前的resource就leak了吧
            // 因为你指针现在不指向之前的resouce了 就没有去free了！！！！
            // Free the existing resource.
            delete[] _data;
            
            // 这里和move constructor一样！！！！！
            // Copy the data pointer and its length from the
            // source object.
            _data = other._data;
            _length = other._length;

            // Release the data pointer from the source object so that
            // the destructor does not free the memory multiple times.
            other._data = nullptr;
            other._length = 0;
        }
        
        return *this;
    }
    
    // Retrieves the length of the data resource.
    size_t Length() const
    {
        return _length;
    }

private:
   size_t _length; // The length of the resource.
   T* _data; // The resource.
};

int main()
{
    // Create a vector object and add a few elements to it.
    vector<MemoryBlock<int>> v;
    v.push_back(MemoryBlock<int>(25));
    v.push_back(MemoryBlock<int>(75));

    // Insert a new element into the second position of the vector.
    v.insert(v.begin() + 1, MemoryBlock<int>(50));
    return 0;
}
