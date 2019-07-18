//
//  main.cpp
//  playGround
//
//  Created by summerrain on 6/9/19.
//  Copyright © 2019 summerrain. All rights reserved.
//

#include <iostream>
#include <string>
// rvalue-references-move-semantics.cpp
// compile with: /EHsc
#include <vector>
#include <algorithm>

using namespace std;

//https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
//https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
// 可能需要disable copy constructor
using namespace std;

void usingException() {
    
    bool error1 = false;
    bool error2 = true;
    
    
    if(error1) {
        throw "Something went wrong.";
    }
    
    if(error2) {
        throw string("Something else went wrong.");
    }
    
    if (true)
        throw invalid_argument("MyFunc argument too large.");
    
}

class PointerTest
{
public:
    PointerTest() {
        cout << "normal construct" << endl;
    }
    
    // the C++ standard says that non-const references cannot bind to temporary objects
    PointerTest(const PointerTest& p)
    {
        cout << "copy construct" << endl;
    }
    
    void greet() {
        cout << "Hello" << endl;
    }
    
    ~PointerTest() {
        cout << "destructor" << endl;
    }
};

void testUniquePtr(unique_ptr<PointerTest>& ptr)
{
    ptr->greet();
}

void testUniquePtr2(unique_ptr<PointerTest> ptr)
{
    ptr->greet();
}
/*
Pass the shared_ptr by value. This invokes the copy constructor, increments the
reference count, and makes the callee an owner. There's a small amount of overhead
in this operation, which may be significant depending on how many shared_ptr objects
you're passing. Use this option when the implied or explicit code contract between
the caller and callee requires that the callee be an owner.

 
Pass the shared_ptr by reference or const reference. In this case, the reference count
isn't incremented, and the callee can access the pointer as long as the caller doesn't
go out of scope. Or, the callee can decide to create a shared_ptr based on the reference,
and become a shared owner. Use this option when the caller has no knowledge of the callee,
or when you must pass a shared_ptr and want to avoid the copy operation for performance reasons.
 
 */
void testSharedPtr(shared_ptr<PointerTest>& ptr)
{
    ptr->greet();
}

// Pass by value will create temp object then assign to it
// which will invoke the copy constructor
// Once it is out of the scope its destructor will be called
void testClass(PointerTest p)
{
    p.greet();
}


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


int main() {
    // @note unique_ptr 错误1 底层是construct 是 explicit 阻止了implicit conversion
    // 所以不能 = new PointerTest() i.e. 尝试convert PointerTest* to unique_ptr type
    //unique_ptr<PointerTest> u_ptr = new PointerTest();
    //unique_ptr<PointerTest> u_ptr (new PointerTest());
    //unique_ptr<PointerTest> u_ptr2 = std::make_unique<PointerTest>();
    //u_ptr->greet();
    //u_prt2->greet();
//    testUniquePtr(u_ptr2);
//    testUniquePtr2(move(u_ptr2));

    // Create a vector object and add a few elements to it.
    vector<MemoryBlock> v;
    v.push_back(MemoryBlock(25));
    v.push_back(MemoryBlock(75));
    // Insert a new element into the second position of the vector.
    v.insert(v.begin() + 1, MemoryBlock(50));
//    shared_ptr<PointerTest> s_ptr = std::make_shared<PointerTest>();
//    testSharedPtr(s_ptr);
    
//    PointerTest* p = new PointerTest();
//    testClass(*p);
//    delete p;
//    try {
//        usingException();
//    }
//    catch(int e) {
//        cout << "Error code: " << e << endl;
//    }
//    catch(char const * e) {
//        cout << "Error message: " << e << endl;
//    }
//    catch(string &e) {
//        cout << "string error message: " << e << endl;
//    }
//
//    catch (invalid_argument& e)
//    {
//        cout << e.what() << endl;
//        return -1;
//    }
//
    
    return 0;
}
