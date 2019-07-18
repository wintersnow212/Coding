// rvalue-references-move-semantics.cpp
// compile with: /EHsc
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

//https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
//https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
// 可能需要disable copy constructor

class MemoryBlock
{
public:

   // Simple constructor that initializes the resource.
   explicit MemoryBlock(size_t length)
      : _length(length)
      , _data(new int[length])
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

   // Copy constructor.  deep copy
   MemoryBlock(const MemoryBlock& other)
      : _length(other._length)
      , _data(new int[other._length])
   {
      std::cout << "In MemoryBlock(const MemoryBlock&). length = "
                << other._length << ". Copying resource." << std::endl;

      std::copy(other._data, other._data + _length, _data);
   }

   // Copy assignment operator.
   MemoryBlock& operator=(const MemoryBlock& other)
   {
        std::cout << "In operator=(const MemoryBlock&). length = "
                << other._length << ". Copying resource using copy assignment." << std::endl;

        if (this != &other)
        {
            // Free the existing resource.
             delete[] _data;

             _length = other._length;
             _data = new int[_length];
             std::copy(other._data, other._data + _length, _data);
        }
      
        return *this;
   }

   // Retrieves the length of the data resource.
   size_t Length() const
   {
      return _length;
   }
    
    // Move constructor.
    MemoryBlock(MemoryBlock&& other)
       : _data(nullptr)
       , _length(0)
    {
       std::cout << "In MemoryBlock(MemoryBlock&&). length = "
                 << other._length << ". Moving resource." << std::endl;

       // Copy the data pointer and its length from the
       // source object.
       _data = other._data;
       _length = other._length;

       // Release the data pointer from the source object so that
       // the destructor does not free the memory multiple times.
       other._data = nullptr;
       other._length = 0;
    }

    // Move assignment operator.
    MemoryBlock& operator=(MemoryBlock&& other)
    {
       std::cout << "In operator=(MemoryBlock&&). length = "
                 << other._length << "." << std::endl;

       if (this != &other)
       {
          // Free the existing resource.
          delete[] _data;

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
    

private:
   size_t _length; // The length of the resource.
   int* _data; // The resource.
};


int main()
{
   // Create a vector object and add a few elements to it.
   vector<MemoryBlock> v;
   v.push_back(MemoryBlock(25));
   v.push_back(MemoryBlock(75));

   // Insert a new element into the second position of the vector.
   v.insert(v.begin() + 1, MemoryBlock(50));
}
