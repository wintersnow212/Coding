// rvalue-references-move-semantics.cpp
// compile with: /EHsc
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

//https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
//https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
// 可能需要disable copy constructor
// copy&&move construct 不用free 但是感觉copy assignment 和move assignment需要

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
   // 但是特别注意smart pointer不是deep copy 是shadow copy
   // 这里也可以看到copy constructor和copy assignment的区别 copy constructor是 construct
   // new object from existing object
   // 而copy assignment operator是 assign two existing objects
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
   
    MemoryBlock(MemoryBlock&& other)
        : _length(0)
        ,_data(nullptr)
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

// Another example from learn c++
template<class T>
class Auto_ptr4
{
    T* m_ptr;
public:
    Auto_ptr4(T* ptr = nullptr)
        :m_ptr(ptr)
    {
    }
 
    ~Auto_ptr4()
    {
        delete m_ptr;
    }
 
    // Copy constructor
    // Do deep copy of a.m_ptr to m_ptr
    Auto_ptr4(const Auto_ptr4& a)
    {
        std::cout << "Copy constructor" << std::endl;
        m_ptr = new T;
        *m_ptr = *a.m_ptr;
    }
 
    // Move constructor
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr4(Auto_ptr4&& a)
        : m_ptr(a.m_ptr)
    {
        std::cout << "move constructor" << std::endl;
        a.m_ptr = nullptr; // we'll talk more about this line below
    }
 
    // Copy assignment
    // Do deep copy of a.m_ptr to m_ptr
    Auto_ptr4& operator=(const Auto_ptr4& a)
    {
        std::cout << "Copy assignment" << std::endl;
        // Self-assignment detection
        if (&a == this)
            return *this;
 
        // Release any resource we're holding
        delete m_ptr;
 
        // Copy the resource
        m_ptr = new T;
        //*m_ptr = *a.m_ptr;
        memcpy(m_ptr, a.m_ptr, sizeof(Auto_ptr4));
 
        return *this;
    }
 
    // Move assignment
    // Transfer ownership of a.m_ptr to m_ptr
    Auto_ptr4& operator=(Auto_ptr4&& a)
    {
        std::cout << "move assignment" << std::endl;
        // Self-assignment detection
        if (&a == this)
            return *this;
 
        // Release any resource we're holding
        delete m_ptr;
 
        // Transfer ownership of a.m_ptr to m_ptr
        m_ptr = a.m_ptr;
        a.m_ptr = nullptr; // we'll talk more about this line below
 
        return *this;
    }
 
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    bool isNull() const { return m_ptr == nullptr; }
};
 
class Resource
{
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};
 
Auto_ptr4<Resource> generateResource()
{
    Auto_ptr4<Resource> res(new Resource);
    return res; // this return value will invoke the move constructor
}

int main()
{
    
    std::string str = "Hello";
    std::vector<std::string> vec;
 
    // uses the push_back(const T&) overload, which means 
    // we'll incur the cost of copying str
    vec.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    vec.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << vec[0]
                                         << "\", \"" << vec[1] << "\"\n";
    
    
    
    // Create a vector object and add a few elements to it.
    vector<MemoryBlock> v;

    //
    v.push_back(MemoryBlock(25));
    v.push_back(MemoryBlock(75));

    // Insert a new element into the second position of the vector.
    v.insert(v.begin() + 1, MemoryBlock(50));
    
    
    Auto_ptr4<Resource> mainres;
    mainres = generateResource(); // this assignment will invoke the move assignment
}