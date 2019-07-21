#include <iostream>
#include <cstring>
#include <assert.h>
using namespace std;

// 错误1 这里一定要是pass by reference 
// 或者是char** 因为不然pStr指向根本没有update; 
// 因为没有指向malloc那块内存！！！！！！！
void Correct(char*& pChar)
{
    pChar = (char*)malloc(1000);

    const char* pSource = "Hello， Tianyu";

    // size of pointer 只是4或者8 size of array才可以
    cout << sizeof(pSource) << endl;

    cout << strlen(pSource) << endl;

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(pChar, pSource, strlen(pSource) + 1);
}

void Correct2(char** pChar)
{
    *pChar = (char*)malloc(1000);

    const char* pSource = "Hello again, Tianyu";

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(*pChar, pSource, strlen(pSource) + 1);
}


void Wrong(char* pChar)
{
    pChar = (char*)malloc(1000);

    const char* pSource = "Hello, Wrong";

    // 错误2 这里要+1 因为strlen是不包括null termintor的
    strncpy(pChar, pSource, strlen(pSource) + 1);
}

// 1. array pointer 不能pass by reference 因为这也是数组名和pointer
//    最大的不同 数组名是定值 但是pointer是可以改变的
// 2. 这里pass by value 就是create了一个数组名的copy 所以用 sizeof 就再也
//    计算不出array size了
void arraySize(int* p)
{
    cout << "Array size is " << (sizeof(p) / sizeof(int)) << endl;
}


// 1. array pointer 不能pass by reference 因为这也是数组名和pointer
//    最大的不同 数组名是定值 但是pointer是可以改变的
// 2. 这里pass by value 就是create了一个数组名的copy 所以用 sizeof 就再也
//    计算不出array size了
void arraySize2(int p[])
{
    cout << "Array size is " << (sizeof(p) / sizeof(int)) << endl;
}

void strLenTest(char* p)
{
    // 这里可计算的原因 因为strlen是一个库函数 它原理其实是去找null terminator
    // 并不是简单的sizeof
    cout << "string size is " << strlen(p) << endl;
}

// To execute C++, please define "int main()"
int main() {
    char* pStr = nullptr;
    Correct(pStr);
    cout << pStr << endl;
    free(pStr);
    char* pStr2 = nullptr;
    Correct2(&pStr2);
    cout << pStr2 << endl;
    free(pStr2);

    char* pStrWrong = nullptr;
    Wrong(pStrWrong);
    if (pStrWrong == nullptr)
    {
        cout << "Failed and the memory there will never be freed" <<endl;
    }
    

    int pArray[5] = {};
    cout << "Array size is " << (sizeof(pArray) / sizeof(int)) << endl;
    arraySize(pArray);
    arraySize2(pArray);
    
    
    char* pStrTest = "Test";
    
    strLenTest(pStrTest);
    // 这里错误的原因因为pStrTest只是一个pointer啊！！！！
    cout << "wrong string size with null terminator is "
         << sizeof(pStrTest)/sizeof(char) << endl; 
    
    char pStrTest2[] = "Test";
    strLenTest(pStrTest2);
    cout << "string size with null terminator is "
         << sizeof(pStrTest2)/sizeof(char) << endl;
    
    
    /*
    体现了指针的妙用 指针只是一个地址variable 用来reference memory
    如果没有backed memory没有任何意义， 
    这里我们把指针p指向了a的memory 然后通过p修改memory 
    那么a的值必然会改变
    */
    int a = 5;
    int* p = &a;
    *p = 10;
    cout << a << endl;
    
    return 0;
}