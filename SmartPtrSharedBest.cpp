#include <iostream>
#include <string>
#include <memory>
using namespace std;
//https://lokiastari.com/blog/2015/01/15/c-plus-plus-by-example-smart-pointer-part-ii/index.html
/*********************************************************************
要很好的理解smart pointer的意义:
Give the ownership of heap allocated memory to stack varible,so that when it is out of scope, it will be freed autonmatically
所以pass in 一个new的object pointer 用自己的member pointer 指向同一块memory
到时候out of scope的时候就可以free掉它
**************************************************************************/


/*********************************************************************
Simple implementation: 不用使用refCnt struct 而是使用int* m_refCnt就可以了
特别注意这里是int* 不能是int!!!!!!
因为object 会在class 释放的时候自动被释放！！！！
**************************************************************************/
// 这里一定是template啊 不然 smart pointer 不知道是construct什么类型的object
template<typename T>
class SharedPtrSp
{
public:
    SharedPtrSp()
        : m_ptr(nullptr)
        , m_refCnt(nullptr)   
    {
        
    }
    // Smart pointer的背后意义就是把heap allocated object的ownership
    // 传给stack object 所以constructor的时候经常是pass in pointer
    SharedPtrSp(T* ptr) 
        : m_ptr(ptr)
        , m_refCnt(new int(1))
    {
        cout << "Created smart_ptr! Ref count is " << *m_refCnt << endl;
    }
    
    virtual ~SharedPtrSp()
    {
        if (m_refCnt != nullptr)
        {
            (*m_refCnt)--;
            cout << "Destroyed smart_ptr! Ref count is " << *m_refCnt << endl;
            if ((*m_refCnt) == 0)
            {
                // m_ptr的delete之后发生在m_refCnt == 0的时候！！！
                delete m_ptr;
                m_ptr = nullptr;
                delete m_refCnt;
                m_refCnt = nullptr;
            }
        }
    }
    
/*********************************************************************
    Copy constructor 简便写法!!! 
**************************************************************************/
//     SharedPtrSp (const SharedPtrSp& other)
//         : m_ptr(other.m_ptr)
//         , m_refCnt(other.m_refCnt)
//     {
        
//         (*m_refCnt)++;
//     }
    
    // Copy constructor 我们这里就是希望shadow copy!!!!
    SharedPtrSp (const SharedPtrSp& other)
    {
        // 这里就是很简单的他们都指向同一个resource
        m_ptr = other.m_ptr;
        // 这个是错误的！！！！因为这样你的other 就没有的到update!!!
        // const 没有关系我又没有改变m_refCnt的指向我只改变了里面的值
        //*m_refCnt = *(other.m_refCnt);
        // 这个是正确的！！！！
        // 因为我们想改变other里面m_refCnt的值
        m_refCnt = other.m_refCnt;
        (*m_refCnt)++;
        cout << "Let me See! " << *(other.m_refCnt) << endl;
        cout << "Copy constructor called! Ref count is " << *m_refCnt << endl;
    }
    
/*********************************************************************
    copy assignment是最容易错的！！！！！
**************************************************************************/   
    // 这里return type是return by reference!!!!!!!!!
    // Copy assignment operator
//     SharedPtrSp<T>& operator= (const SharedPtrSp& other)
//     {
//         // 不要写成*this == other !!!!
//         // 因为那样没有 对应的operator!!!
//         if (this == &other)
//         {
//             return *this;
//         }
        
        
//         T* oldPtr = m_ptr;
//         int* oldCnt = m_refCnt;
        
//         m_ptr = other.m_ptr;
//         m_refCnt = other.m_refCnt;
//         (*m_refCnt)++;
        
        
//         // 关键步骤是copy assignment和其他的不同！！！！
//         (*oldCnt)--;
//         if ((*oldCnt) == 0)
//         {
//             delete oldPtr;
//             oldPtr = nullptr;
//             delete oldCnt;
//             oldCnt = nullptr;
//         }
        
        
//         // 关键错误！！！！ 这里要先保存old 再delete!!!!
// //         if (m_ptr != nullptr)
// //         {
// //             (*m_refCnt)--;
// //             cout << "Destroyed smart_ptr! Ref count is " << *m_refCnt << endl;
// //             if ((*m_refCnt) == 0)
// //             {
// //                 delete m_ptr;
// //                 m_ptr = nullptr;
// //                 delete m_refCnt;
// //                 m_refCnt = nullptr;
// //             }
// //         }
        
// //         // 这一步和copy constructor 是一样的！！！！！
// //         m_ptr = other.m_ptr;
// //         m_refCnt = other.m_refCnt;
// //         (*m_refCnt)++;
        
//         cout << "Copy assignment called! Ref count is "
//              << *m_refCnt << endl;
        
//         return *this;
//     }
    
/*********************************************************************
    优化 copy and swap idom https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
**************************************************************************/
    SharedPtrSp<T>& operator= (const SharedPtrSp& other)
    {
        // constructor of tmp handles increment.
        SharedPtrSp tmp(other);
        
        // swap old object to temp 
        // 之后temp会在函数末尾free掉
        swap(m_ptr, tmp.m_ptr);
        swap(m_refCnt, tmp.m_refCnt);
        
        return *this;
    }
    
/*********************************************************************
    再一步优化 copy performed by initializing the (non-reference) 
    parameter i.e. copy performed by "pass by value"
**************************************************************************/
//     SharedPtrSp& operator= (SharedPtrSp other)
//     {
//         this->swap(other);
        
//         return *this;
//     }
    
//     // Always good to have a swap function
//     // Make sure it is noexcept
//     void swap(SharedPtrSp& other) noexcept
//     {
//         // 这里的swap 不能是直接的swap(other, *this) 
//         // 而是分别swap 每一个member
//         std::swap(m_ptr,  other.m_ptr);
//         std::swap(m_refCnt, other.m_refCnt);
//     }
    
    // Move constructor
    SharedPtrSp (SharedPtrSp&& other)
        : m_ptr(other.m_ptr)
        , m_refCnt(other.m_refCnt)
    {
        // 千万记住 这里的move 不能delete！！！！ 因为这里是transfer ownership
        // 这里不是delete memory!!!!!!!!!
        other.m_ptr = nullptr;
        other.m_refCnt = nullptr;
    }
    
    // Move assignment
    SharedPtrSp<T>& operator= (SharedPtrSp&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        delete m_ptr;
        delete m_refCnt;
        
        m_ptr = other.m_ptr;
        m_refCnt = other.m_refCnt;
        
        other.m_ptr    = nullptr;
        other.m_refCnt = nullptr;
        
        return *this;
    }
    
    // overload operator ->
    T* operator-> ()
    {
        return m_ptr;
    }
    
    T& operator* ()
    {
        return *m_ptr;
    }
private:
    T*  m_ptr;
    int* m_refCnt;
};


class AirCraft
{
private:
  
    std::string m_Model;
public:
  
    AirCraft() :m_Model("Generic Model")
    {
        cout << "Generic model aircraft created" << endl;
    }
    
    AirCraft(const string& modelName) :m_Model(modelName)
    {
        cout << "Aircraft type " << m_Model << "is created!" << endl;
    }
      
    void SetAirCraftModel(const string& modelName)
    {
        cout << "Aircraft model changed from  " << m_Model << " to " << modelName << endl;
        m_Model = modelName;
    }
      
    ~AirCraft()
    {
        cout << "Destroying Aircraft of model:" << m_Model << "!" << endl;
    }
};

// Make share 太难了 不管了
// namespace summer
// {
//     template<typename _Tp, typename... _Args>
//     inline shared_ptr<_Tp> make_shared(_Args&&... __args)
//     {
//       typedef typename std::remove_cv<_Tp>::type _Tp_nc;
//       return std::allocate_shared<_Tp>(std::allocator<_Tp_nc>(),
//                        std::forward<_Args>(__args)...);
//     }
// }


void testSimple()
{
    // Create two aircraft objects. 
    SharedPtrSp<AirCraft> rP(new AirCraft("F-22 Raptor"));
    
    
    // SharedPtr<AirCraft> rP2 = summer::make_shared<AirCraft>("F-22 Raptor2");
    
    SharedPtrSp<AirCraft> hP(new AirCraft("F-14 Hornett")); 
    
    // 如果没有这个copy assignment 的就会有两个object
    // 有了这个copy assignment 就会调用我们override 的 assignment operator
    // rP now points to "F14-Hornett".
    // Ref count for hornett is 2. 原来的"B2 Spirit" is destroyed.
    // 就是rp现在assign为hP的值了 所以 rP原来的就应该被destroyed 
    rP = hP; 
   
    //Copy Constructor
    SharedPtrSp<AirCraft> copyRp = rP;
    
    // * operator
    (*rP).SetAirCraftModel("B2 Spirit"); 
    
    //-> operator
    copyRp->SetAirCraftModel("B2 Bomber");
    
    
    // 这里这样是不会call copy constructor 或者copy assignment
    SharedPtrSp<AirCraft> uP = new AirCraft("F-22 Tiger");
}

int main()
{
    cout << "-------------Simple Smart Pointer--------------" << endl;
    testSimple();
    cout << endl;
    return 0;
}
