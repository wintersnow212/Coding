#include <iostream>
#include <string.h>
using namespace std;

void swapStr(char* s)
{
    cout << s << endl;
    int len = strlen(s);
    int start = 0;
    int end  = len - 1;
    char temp;
    while (start < end)
    {
        temp = s[start];
        s[start] = s[end];
        s[end] = temp;
        start++;
        end--;
    }
    
}

// 1. 为什么要pass by reference
// 2. 为什么直接assign就可以了 不要allocate and strcpy
void test(const char*& p)
{
    // 字符串不是local varibale 是存在在read only 的memory里面！！！！
    const char* pStr = "Hello\n";
    // int len = strlen(pStr) + 1;
    // p = (char*)malloc(sizeof(char) * len);
    // //memcpy(p, pStr, sizeof(char) * len);
    // strcpy(p, pStr);
    p = pStr;
    p = "Hello\n";
}

int main()
{
    const char* pStr = "abcdefghi";
    char str[] = "abcdefghi";
    // This sizeof has null terminator
    cout << sizeof(str) << endl;
     // 这里只是pointer的size
    cout << sizeof(pStr) << endl;
    // 不包含null terminator
    cout << strlen(str) << endl;
    
    
    // 为什么 swapStr(str) 就可以
    /*
        字符串数组可以修改
        字符数组实现一般是这样的：
        char s[10] = "whatever";
        也相当于
        char s[10] = {'w','h','a','t','e','v','e','r','\0'};
        会在stack分配10个字节给数组s，然后把"whatever"拷贝进去。stack是可读写的，所以你可以修改字符数组的值。
        字符数组名是一个常量指针 也就是是一个const char*
        字符串指针指向内容不能修改
        因为你想啊 这个指针很神奇哎 都没有allocate了就可以使用了 必有绝妙之处！！！
        在内存空间里是分配到“只读内存区”
    */   
    swapStr(str);
    
    
    const char* s;
    //char* s;
    test(s);
    printf("%s\n", s);
    return 0;
}
