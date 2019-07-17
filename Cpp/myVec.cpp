#include <iostream>

template<typename T>
class myVec
{
public:
    myVec()
    {
        cap  = 0;
        size = 0;
        vArr = nullptr;
    }
    
    ~myVec()
    {
        if (vArr != nullptr)
        {
            delete [] vArr;
        }
    }
    //return by reference so that it can be used as lValue
    T& operator[ ](unsigned int i);
    /*
    How long does it take to do a total of n push_back() operations, starting
    with an empty vector?

    Let k be the smallest integer such that n ≤ 2k. For the sake of simplicity,     we'll do the analysis as if we were actually going to do 2k pushes.

    Let m be the current size() of the vector

    if m is a power of 2, say, 2i then the array is full and we do O(m) work on     the next call to push_back()

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
    */
    void push_back(const T& item);
    void reserve(int n, bool copy);
    void pop_back();
    
private:
    int cap;
    int size;
    T* vArr;
};

template<typename T>
T& myVec<T>::operator[ ](unsigned int i)
{
    return vArr[i];
}

template<typename T>
void myVec<T>::reserve(int n, bool copy)
{
    T* newArr;
    newArr = new T[n];
    
    if (newArr == nullptr)
    {
        throw"memory allocation failure";
    }
    
    if (copy == true)
    {
        for(int i = 0; i < size; i++)
        {
            newArr[i] = vArr[i];
        }
    }
    
    // delete original dynamic array. if vArr is NULL, the vector was
    // originally empty and there is no memory to delete
    if (vArr != nullptr)
    {
        delete [] vArr;
    }
    
    vArr = newArr;
    cap  = n;
}

template<typename T>
void myVec<T>::push_back(const T& item)
{
    if (cap == size)
    {
        if (cap == 0)
        {
            reserve(1, false);
        }
        else
        {
            reserve(2 * cap, true);
        }
        
    }
    
    vArr[size] = item;
    size++;
}


template<typename T>
void myVec<T>::pop_back()
{
    size--;
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
    
    myVec<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.pop_back();
    v.push_back(5);
    
    for (int i = 0; i < 4; ++i)
    {
        std::cout << v[i] << std::endl;
    }
}
