#include <iostream>
#include <string>
using namespace std;

struct Test
{
    Test(string s)
        : str(s)
    {
        cout << "constructor" << endl;
    }
    
    Test (const Test& other)
    {
        cout << "copy constructor" << endl;
        str = other.str;
    }
    
    Test (Test&& other)
    {
        cout << "move construcotr" << endl;
        str = other.str;
        other.str.clear();
    }
    
    string str;
};

class TalkPresenter
{
public:
    // pass by value 是可以接收rvalue 只是要求caller去move!!! 不然就会产生copy
    // 比如unique pointer
    void process_name(Test name)
    {
        cout << name.str << endl;
    }
    
    void set_name(Test&& name)
    {
        //process_name(name);
        //cout << name.str << endl;
        
        // 1.  这里的name其实是l value 
        // 可以想成只要有name就是l value 
        // 上面的&& 只是表示可以bind to rvalue不是说这里的name是r value
        
      
        // 2. static_cast<std::string&&>(name)
        // move本质上就是static cast
        //process_name(std::move(name));
        //cout << name.str << endl;
        
        // std::forward<T>()。与std::move()相区别的是，
        // move()会无条件的将一个参数转换成右值，而forward()则会保留参数的左右值类型。
        
        process_name(std::forward<Test>(name));
        cout << name.str << endl;

    }
};


int main() {
    TalkPresenter t;
    
    t.set_name(Test("hahaha"));
    return 0;
}
