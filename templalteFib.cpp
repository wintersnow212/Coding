#include <iostream>
using namespace std;

// https://stackoverflow.com/questions/19630570/what-does-it-mean-to-odr-use-something
// https://stackoverflow.com/questions/37259807/static-constexpr-int-vs-old-fashioned-enum-when-and-why
template <int N>
struct fib
{
    //enum{ val = fib<N-1>::val + fib<N-2>::val};
    static constexpr int val = fib<N-1>::val + fib<N-2>::val;
};

template <>
struct fib<0>
{
    //enum {val = 0};
    static constexpr int val = 0;
};

template <>
struct fib<1>
{
    //enum {val = 1}; 
    static constexpr int val = 1;
};

// To execute C++, please define "int main()"
int main() {
  
    cout << fib<5>::val << endl;
    return 0;
}
