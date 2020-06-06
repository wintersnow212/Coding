#include <iostream>
#include <cstdarg>
using namespace std;
 
void myTest(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    while (*fmt != '\0')
    {
        if (*fmt == 'd')
        {
            int i = va_arg(args, int);
            
            cout << i << endl;
        }
        else if (*fmt == 'c')
        {
            /*
            If you pass an integral value with conversion rank 
            smaller than that of int (e. g. char, bool or short) to 
            a function taking a variable number of arguments, it will be 
            converted to int. Presumably the reason for this has its roots in 
            performance, where it was (and in fact, often it still is nowadays) 
            better to pass values aligned to a machine word boundary.        
            */
            int i = va_arg(args, int);
            
            char c = static_cast<char>(i);
            cout << c << endl;
        }
        else if (*fmt == 'f')
        {
            double d = va_arg(args, double);
            cout << d << endl;
        }
        
        ++fmt;
    }
    
    va_end(args);
}


int main()
{
    // 这里其实恨不智能 当然也无法让fmt自动判断出我们要print的类型
    // 这里我们用dcff 自定义了data type!!!!!
    myTest("dcff", 3, 'a', 1.999, 42.5);
}
