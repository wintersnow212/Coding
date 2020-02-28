#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;

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
    
    // Move constructor
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
