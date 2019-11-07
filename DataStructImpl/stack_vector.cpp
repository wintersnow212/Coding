#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>

using namespace std;
/*这个stack的 template 是建立在内部有一个vector 的STL的基础上的
在c里面没有vector 就要构建两个结构体
struct node
{
	int val;
	
	
}node_t;
*/
template <typename T>
class Stack { 
  private: 
    vector<T> elems;     // elements 

  public: 
    void push(T const&);  // push element 
    void pop();               // pop element 
    T top() const;            // return top element 
    bool empty() const{       // return true if empty.
        return elems.empty(); 
    } 
}; 

template <typename T>
void Stack<T>::push (T const& elem) 
{ 
    // append copy of passed element 
    elems.push_back(elem);    
} 

template <typename T>
void Stack<T>::pop () 
{ 
    if (elems.empty()) { 
        throw out_of_range("Stack<>::pop(): empty stack"); 
    }
	// remove last element 
    elems.pop_back();         
} 

template <typename T>
T Stack<T>::top () const 
{ 
    if (elems.empty()) { 
        throw out_of_range("Stack<>::top(): empty stack"); 
    }
	// return copy of last element 
    return elems.back();      
} 

int main() 
{ 
/*try and catch very good*/
    try { 
        Stack<int>         intStack;  // stack of ints 
        Stack<string> stringStack;    // stack of strings 

        // manipulate int stack 
        intStack.push(7); 
        cout << intStack.top() <<endl; 

        // manipulate string stack 
        stringStack.push("hello"); 
        cout << stringStack.top() << std::endl; 
        stringStack.pop(); 
        stringStack.pop(); 
    } 
    catch (exception const& ex) { 
        cerr << "Exception: " << ex.what() <<endl; 
        return -1;
    } 
} 