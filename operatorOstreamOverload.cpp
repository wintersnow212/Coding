#include <iostream>

using namespace std;

class Person {
public:
    Person(const string& first_name, const string& last_name) : first_name_(first_name), last_name_(last_name) {}
    const string& get_first_name() const {
      return first_name_;
    }
    const string& get_last_name() const {
      return last_name_;
    }
    
    // Method 2 这种是declare friend 的方法 
    /*
    Friend 函数类重载
    用friend的原因是我们外面要access private member来调用operator
    As we know friend functions are special type of functions that are not 
    member of the class but can access member variables and can be called 
    without using any objec
    如果是重载双目操作符（即为类的成员函数），就只要设置一个参数作为右侧运算量，而左侧运算量就
    是对象本身
    而 >> 或<< 左侧运算量是 cin或cout 而不是对象本身，所以不满足后面一点，
    就只能申明为友元函数
    */
    friend std::ostream& operator << (std::ostream& stream, const Person& other);
private:
    string first_name_;
    string last_name_;
};

/*
    It turns out that there are three different ways to overload operators: 
    1. the member function way,
    2. the friend function way, 
    3. the normal function way. 
    In this lesson, we’ll cover the friend function way    
*/
// Method 1 这种是不declare friend 的方法 利用public member function
// std::ostream& operator<<(std::ostream& stream, const Person& other)
// {
//     stream  << other.get_first_name() << " " << other.get_last_name();
    
//     return stream;
// }

// 这里是function argument第二个是class object
ostream& operator<<(ostream& stream, const Person& other)
{
    stream << other.first_name_ << " " << other.last_name_;
    
    // 这里是return stream
    return stream;
}

int main() {
    string first_name = "Tianyu"; 
    string last_name = "Xia"; 
    string event = "Get Offer";
    
    auto p = Person(first_name, last_name);
    cout << p << " " << event << endl;
    return 0;
}
