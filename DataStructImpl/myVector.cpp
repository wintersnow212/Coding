// https://zhuanlan.zhihu.com/p/65982080
/*
一）在检测到允许进行优化处理时，进行下列性能的优化：

１）避免T对象移动的优化：

自研库使用realloc的方式申请内存扩展空间，而不是使用常规的malloc的方式申请内存，这样在很多情况扩展时都不用拷贝数据，大幅度提升了性能．
这块的实际性能提升倍数取决于能够连续多少次在原地址处申请扩展内存成功，只要在原地址处能够成功扩展空间，就不需要移动T对象了，大大节约了开销．

２）减少T对象创建和析构的开销：

erase（） / insert（） 等函数内部实现有很多处需要移动T&&对象，可以采用memmove / memcpy /memset 方式优化，可大幅度提升性能（减少了对象创建和析构的开销）

（二）占用内存的优化：

在允许进行优化处理时，由于优化方案中T对象的移动速度很快，因此，可以增加T对象移动的次数，来减少空间的浪费，由于cache更友好，性能几乎不变，总体还是比std::vector快一倍．

１）在检测到允许进行优化处理时，将vector的扩展倍数，从gcc默认的２倍增长修改为1.5倍增长（即8,12,16,24,32,48,64...)，可以平均减少一半的浪费空间。
否则还是默认的２倍增长，以便提供与std库同样的性能．

2) 修改起始空间数量，减少小空间下内存扩展的开销: gcc版本std::vector默认有数据开始空间大小就是１，自研版本默认有数据开始空间大小是８，这样适当浪费一点小空间，来减少常规场景下的内存扩展次数，提高性能．

(三) 自身占用空间的优化，std::vector目前64位版本占用空间为24Byte， 拆分为haisql:: vector_big 和haisql::vector 两个版本，
haisql::vector限制size大小不超过20亿，占用空间为16Byte，比std库版本节约33%的自身内存占用。
主要原因是我们考虑一般不会有20亿条记录的vector场景存在，限制size和capacity都是4字节的unsigned int，可以节约出8字节的空间。
这块优化的思路是不为常规场景以外付出额外的代价。

*/
#include <iostream>
#include <cstring>
using namespace std;

#define REALLOC 0
template<typename T>
class myVec
{
public:
    myVec()
        : arr(nullptr)
        , _cap(0)
        , _size(0)
    {
        
    }
    
    myVec(int size)
        : _cap(size)
        , _size(size)
    {
        arr = new T[size];
        memset(arr, 0, sizeof(T) * size); 
    }
    
    ~myVec()
    {
        if (arr != nullptr)
        {
           /* Use realloc only with malloc, calloc or another realloc and 
              clean it up with free. In C++ use always new with delete and new[] 
              with delete[].
            */
            delete [] arr;     
#if REALLOC == 1
            free(arr);
#endif
            arr = nullptr;
        }  
    }
    
    // Copy Constructor
    myVec(const myVec& other)
        : _cap(other._cap)
        , _size(other._size)
    {
        arr = new T[other._cap];
        memcpy(arr, other.arr, sizeof(T)*other._size);
    }
    
    void push_back(const T& element);
    
    void pop_back()
    {
        _size--;
    }
    
    // Return by reference so that it can be used as lValue!!!!!
    T& operator[](int idx)
    {
        return arr[idx];
    }
    
    int size()
    {
        return _size;
    }

     void reserve(int newCap)
    {
#if REALLOC == 1
        arr = static_cast<T*>(realloc(arr, sizeof(T) * newCap));
#endif
        
        T* newArr;
        newArr = new T[newCap];

        if (newCap != 1)
        {
            copy (arr, arr + _size, newArr);
            // for(int i = 0; i < _size; i++)
            // {
            //     newArr[i] = arr[i];
            // }
        }

        // delete original dynamic array. if vArr is NULL, the vector was
        // originally empty and there is no memory to delete
        if (arr != nullptr)
        {
            /*
            C++告诉我们在回收用 new 分配的单个对象的内存空间的时候用 delete，回收用 
            new[] 分配的一组对象的内存空间的时候用 delete[]。
            关于 new[] 和 delete[]，其中又分为两种情况：(1) 为基本数据类型分配和回收
            空间；(2) 为自定义类型分配和回收空间。         
            */
            delete [] arr;
        }

        arr = newArr;
        _cap  = newCap;
        
    }
    
private:
    T* arr;
    int _cap;
    int _size;
};

/*
How long does it take to do a total of n push_back() operations, starting with an empty vector?
Let k be the smallest integer such that n ≤ 2k. For the sake of simplicity, we'll do the analysis as if we were actually going to do 2k pushes.
Let m be the current size() of the vector
if m is a power of 2, say, 2i then the array is full and we do O(m) work on the next call to push_back()
if m ≠ 2i, then we do O(1) work on next call
Total effort is
∑i=1k(O(2i)+∑j=12i-1O(1))
   =O(∑i=1k(2i))
   =O(1+2+4+...+2k)
   =O(2k+1-1)
   =O(2k+1)
Total effort is O(2k+1).
But we started with the statement that n=2k, so this is
=O(2n)=O(n)
So the true answer is that vector::push_back does have a worst case of O(n), but in special circumstances that cost may average (amortizes) to O(1) over a sequence of n calls.
In fact, if you were to look at the required behavior for vector::push_back listed in the C++ language standard, you would find that the required O(1) behavior, is, indeed, a requirement for amortized time, not a requirement on the worst-case time.
*/
template<typename T>
void myVec<T>::push_back(const T& element)
{
    if (_size == _cap)
    {
        if (_cap == 0)
        {
            reserve(1);
        }
        else
        {
            reserve(_cap * 2);
        }
    }

    arr[_size++] = element;
}

int main()
{
    int a = 1234;
    int* intP = &a;
    char* charP = reinterpret_cast<char*>(&a);
    string out;
    out.append(charP, sizeof(a));
    printf("%02x\n", charP);
    cout << intP << endl;
    int val = *(reinterpret_cast<const int*>(out.data()));
    cout << val << endl;
    cout << "---------vetcor 1---------" << endl;
    myVec<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.pop_back();
    v.push_back(5);
    
    // 这里体现了左值的重要性！！！
    v[v.size() - 1] = 100;
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << std::endl;
    }
    
     cout << "--------vetcor 2---------" << endl;
    myVec<int> v2(v);
    for (int i = 0; i < v2.size(); ++i)
    {
        std::cout << v2[i] << std::endl;
    }
    
    cout << "---------vetcor 3---------" << endl;
    myVec<int> v3(5);
    v3[1] = 2;
    for (int i = 0; i < v3.size(); ++i)
    {
        std::cout << v3[i] << std::endl;
    }
}    
