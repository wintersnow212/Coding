/* https://zhuanlan.zhihu.com/p/65982080
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
#include <exception>
using namespace std;

template<typename T>
class MyVector
{
public:
    MyVector()
        : data(nullptr)
        , cap(0)
        , idx(-1)
    {
        
    }
    
    MyVector(int size)
        : data (new T[size])
        , cap(size)
        // 关键这里应该是idx = size - 1 因为idx只是用于push_back
        , idx(size-1)
    {
        memset(data, 0, sizeof(T) * cap); 
    }
    
    // Copy constructor
    MyVector(const MyVector& other)
        : cap(other.cap)
        , idx(other.idx)
    {
        /* 这里的data 不是nullptr
           其实是uninitialized wild pointer!!!!!!!
           因为你没有通过constructor啊 而是直接call copy ctr
           There is no previous value for input assigned in the target of 
           the copy construction in your copy-ctor. I.e. input doesn't point 
           to anything yet (how could it? you're just-now creating the 
           target object). Thus, no leak.
        */
        //T* delData = data;
        printf("%02x\n", data);
        cout << "I am wild pointer" << endl;
            
        data = new T[cap];
        memcpy(data, other.data, sizeof(T)*(other.idx + 1));
        //copy (other.data, other.data + other.idx + 1, data);
        // if (delData != nullptr)
        //     delete [] delData;
    }
    
    MyVector(MyVector&& other)
    {

    }
    
    void push_back(const T& val);
    
    void pop_back();
    
    T back();
    
    int size() {return idx + 1;}
    
    T& operator [](int i)
    {
        if (i >= cap)
        {
            throw "out of bound access";
        }
        
        return data[i];
    }
    
    ~MyVector()
    {
        if (data != nullptr)
        {
            delete [] data;
        }
    }
private:
    void grow();
    
    T* data;
    int cap;
    int idx;
};

/*
1. Allocate a new block of memory that is some multiple of the container's current capacity. In most implementations, vector and string capacities grow by a factor of two each time. i.e. their capacity is doubled each time the container must be expanded.
2. Copy all the elements from the container's old memory into its new memory.
3. Destroy the objects in the old memory.
4. Deallocate the old memory
*/
template<typename T>
void MyVector<T>::grow()
{
    // 这里的起始情况不能忘了！！！！
    if (cap == 0)
    {
        cap = 1;
    }
    else
    {
        cap *= 2;
    }
    
    T* newData = new T[cap];
    
    // delete original dynamic array. if vArr is NULL, the vector was
    // originally empty and there is no memory to delete
    if (data != nullptr)
    {
        /*
        memcpy的单位是 byte!!!! 但是不能work with custom object
        因为memcpy不会invoke copy constrctor
        这样导致的问题就是 multiple objects will be referring to the same 
        dynamically allocated memory, and more than one destructor will try to 
        release it.
        
        The problem with using memcpy is that it bypasses copy constructors. 
        This is OK only when your class is composed of primitives.

        However, Pen class has non-primitive data members of type std::string.           These objects require a call of copy constructor to be copied. memcpy 
        does not perform any calls to copy constructors, which leads to internal 
        representations of std::string becoming shared, which in turn causes 
        undefined behavior on destruction.

        Copying with a loop, on the other hand, invokes a copy constructor, so 
        your code runs without a problem.

        C++ Standard Library provides a utility function for copying ranges     
        called std::copy. Using this function avoids the problem that you see,
        because it invokes copy constructors as needed.
        */
        //memcpy(newData, data, (idx+1) * sizeof(T));
        copy (data, data + idx + 1, newData);
        // 这里没有destrory old objcet
        delete [] data;
        
        // 最简单就可以直接使用for loop!!!!!
        // for (int i = 0; i < idx + 1; ++i)
        // {
        //     newData[i] = data[i];
        //     data[i].~T();
        // }
        
        //delete [] data;
    }
    
    data = newData;
}

/*
vector 的push_back 是worst O(n);  average/amortized O(1)

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
void MyVector<T>::push_back(const T& val)
{
    if (idx == cap - 1)
    {
        grow();
    }
    data[++idx] = val;
}

template<typename T>
void MyVector<T>::pop_back( )
{
    if (idx == -1)
        throw "vector is empty, nothing to pop";
    
    // 这里要显示调用destructor来避免memory leak吗！！！！
    data[idx].~T();
    
    idx--;
}


template<typename T>
T MyVector<T>::back( )
{
    if (idx == -1)
        throw "out of bound";
    
    return data[idx];
}

// To execute C++, please define "int main()"
int main() {
    
    int a = 1234;
    int* intP = &a;
    char* charP = reinterpret_cast<char*>(&a);
    string out;
    out.append(charP, sizeof(a));
    printf("%02x\n", charP);
    cout << intP << endl;
    int val = *(reinterpret_cast<const int*>(out.data()));
    cout << val << endl;
    
    try
    {
        cout << "--------vetcor 1---------" << endl;
        MyVector<int> v;
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
        MyVector<int> v2(v);
        for (int i = 0; i < v2.size(); ++i)
        {
            std::cout << v2[i] << std::endl;
        }

        cout << "---------vetcor 3---------" << endl;
        MyVector<int> v3(6);
        v3[1] = 2;
        for (int i = 0; i < v3.size(); ++i)
        {
            std::cout << v3[i] << std::endl;
        }
        
        v3.pop_back(); v3.pop_back(); v3.pop_back();
        v3.pop_back(); v3.pop_back(); v3.pop_back();
        v3.pop_back();
        
    } catch (const char* msg) { 
        cout << msg <<endl; 
        return -1;
    }
    return 0;
}
