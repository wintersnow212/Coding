#include <iostream>
#include <cstring>
#include <vector>
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

void reverse(char* s)
{
    int len   = strlen(s);
    cout << len << endl;
    int start = 0;
    int end   = len-1;
    
    while (start < end)
    {
        // cout << s[start] << endl;
        // cout << s[end] << endl;
        swap(s[start++], s[end--]);
        // char temp = s[start];
        // s[start++] = s[end];
        // s[end--] = temp;
    }
}
void strLenTest(char* p)
{
    // 这里可计算的原因 因为strlen是一个库函数 它原理其实是去找null terminator
    // 并不是简单的sizeof
    cout << "string size is " << strlen(p) << endl;
}

int getValue()
{
    int a = 3;
    return a;
}

// 错误！！ reference to local variable ‘a’ returned
// 因为你out of scope后 就会被free了 所以return reference to local varible
// 没有任何意义
int& getReference()
{
//     int a = 3;
//     return a;
    
    // New 的使用
    // 表示 allocate a single int and init it to 100
    // constructor call
    //int* a = new int(5);
    
    // 表示 allocate a array of 5 adjacent integers
    int* a = new int[5];
    *a = 1;
    
    return *a;
}

void getPointer(vector<int*>& ptrVec)
{
    int test = 100;
    // 错误 这里是pointer to local variable!!!! 
    // out of scope 就会被free了 所以存它的地址没有任何意义
    int* pRet = &test;
    ptrVec.push_back(pRet);
    
    // 可以改成static 但是staic 的问题在于每次call getPointer 它的改变都会保存下来
    // 因为static是和life time 有关
    // static int testVar = 100;
    // int* pRet = &testVar;
    // ptrVec.push_back(pRet);
}

// To execute C++, please define "int main()"
int main() {
    // 错误！！ 正常function return 是不能assign 给lValue的 因为function return 是temporary no name的rValue
    //int& ref = getValue();
    
    int& ref = getReference();
    delete [](&ref);
    
    vector<int*> ptrVec;
    getPointer(ptrVec);
    cout << "Test the freed garbage memory" <<endl;
    cout << *ptrVec[0] << endl;
    
//     cout << "Test the out of bound" <<endl;
//     int arr[] = {1,2,3,4,5}; 
//     printf("arr [0] is %d\n", arr[0]); 
      
//     // arr[10] is out of bound 这个在C 的compiler里面竟然是可以的！！！
//     printf("arr[10] is %d\n", arr[10]); 
    
    
//     arr[10] = 11; 
//     printf("arr[10] is %d\n",arr[10]); 
    
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
    
    struct Node
    {
        Node(int v)
            : val(v)
            , next(nullptr)
        {

        }
        
        bool operator== (const Node& other)
        {
            if (val == other.val && next == other.next)
            {
                return true;
            }
            
            return false;
        }
        int   val;
        Node* next;
    };
    
    Node* ptr1 = new Node(1);
    Node* ptr2 = ptr1;
    
    if (ptr2 == ptr1)
    {
        cout << "We are comparing the address that pointer points to" << endl;
    }
    
    
    Node obj1(1);
    Node obj2(1);
    
    if (obj1 == obj2)
    {
        cout << "We are comparing the objects and we need to overload the == " << endl;
    }
    
     /*
        The difference here is that

        char *s = "Hello world";
        will place "Hello world" in the read-only parts of the memory, and making s a pointer to that makes any writing operation on this memory illegal.

        While doing:

        char s[] = "Hello world";
        puts the literal string in read-only memory and copies the string to newly allocated memory on the stack. Thus making

        s[0] = 'J';
        legal.
    */
     char s[] = "hello word";
    //char* s = "hello word";
    reverse(s);
    cout << s << endl;
    
    return 0;
}
