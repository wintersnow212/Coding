#include <iostream>
#include <string.h>
using namespace std;

void swapStr(char* s)
{
    cout << s << endl;
    int len = strlen(s);
    int start = 0;
    int end  = len - 1;
    while (start < end) {
        swap(s[start++], s[end--]);
    }
}

/* 
1. 为什么要pass by reference
   这里一定要是pass by reference 
   或者是char** 因为不然pStr指向根本没有update; 因为没有指向malloc那块内存！！！

2. 为什么直接assign就可以了 不要allocate and strcpy
   因为感觉字符串指针比较特别 不是local varaible 而是在read only memory
   如果是字符串数组就不可以了 就是local variable 会被free 掉
*/
void Correct1(const char *& pChar)
{
    const char* pSource = "Tianyu Xia get offer";
    pChar = pSource;
}


void Correct2(char** pChar)
{
    
    const char* pSource = "Tianyu Xia get offer again";
    *pChar = (char*)malloc((strlen(pSource) + 1) * sizeof(char));

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(*pChar, pSource, strlen(pSource) + 1);
}

void Correct3(char*& pChar)
{
    const char* pSource = "Tianyu Xia get offer again and again";

    pChar = (char*)malloc((strlen(pSource) + 1) * sizeof(char));

    // size of pointer 只是4或者8 size of array才可以
    cout << sizeof(pSource) << endl;

    cout << strlen(pSource) << endl;

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(pChar, pSource, strlen(pSource) + 1);
    //pChar = pSource;
}


void Wrong1(char* pChar)
{
    const char* pSource = "Hello, Wrong";
    pChar = (char*)malloc(strlen(pSource) + 1);

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(pChar, pSource, strlen(pSource) + 1);
}


void Wrong2(char* const pChar)
{
    const char* pSource = "Hello, Wrong";
    pChar = (char*)malloc(strlen(pSource) + 1);

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(pChar, pSource, strlen(pSource) + 1);
}

int main()
{
    const char* pTest = "abcdefghi";
    char str[] = "abcdefghi";
    // This sizeof has null terminator
    cout << sizeof(str) << endl;
     // 这里只是pointer的size
    cout << sizeof(pTest) << endl;
    // 不包含null terminator
    cout << strlen(str) << endl;
    
    
    // 为什么 swapStr(pTest) 不可以
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
    
    /*
      指针本身是常量不可变
        (char*) const pContent;
      指针所指向的内容是常量不可变
        const (char) *pContent;
        (char) const *pContent;
      两者都不可变
        const char* const pContent;
        识别const到底是修饰指针还是指针所指的对象，还有一个较为简便的方法，也就是沿着*号划一条线：
        如果const位于*的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；
        如果const位于*的右侧，const就是修饰指针本身，即指针本身是常量。
        
        const char* 可以指向不同的地址 只是内容不能改变
        char * const 不可以指向不同地址
        const char* pStr = nullptr;
    */
    //char* const pStr = nullptr;
    const char* pStr = nullptr;
    Correct1(pStr);
    printf("%s\n", pStr);
    
    char* pStr2 = nullptr;
    Correct2(&pStr2);
    printf("%s\n", pStr2);
    free(pStr2);

    char* pStr3 = nullptr;
    Correct3(pStr3);
    printf("%s\n", pStr3);
    free(pStr3);
    

    char* pStrWrong = nullptr;
    Wrong1(pStrWrong);
    if (pStrWrong == nullptr)
    {
        cout << "Failed and the memory there will never be freed" <<endl;
    }
    
    char* const pStrWrong2 = nullptr;
    Wrong2(pStrWrong2);

    return 0;
}
