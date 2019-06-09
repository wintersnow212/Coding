//
//  main.cpp
//  playGround
//
//  Created by summerrain on 6/9/19.
//  Copyright © 2019 summerrain. All rights reserved.
//

#include <iostream>
#include <string>
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

    shared_ptr<PointerTest> s_ptr = std::make_shared<PointerTest>();
    testSharedPtr(s_ptr);
    
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
