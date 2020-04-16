#include <iostream>
using namespace std;

template<typename T>
class SharedPtr
{
public:
    SharedPtr(T* ptr)
        : m_ptr(ptr)
        , m_refCnt(new int(1))
    {
        
    }
    
    // copy ctr
    SharedPtr(const SharedPtr& other)
        : m_ptr(other.m_ptr)
        , m_refCnt(other.m_refCnt)
    {
        (*m_refCnt)++;
    }
    
    // copy assignment
    SharedPtr& operator= (const SharedPtr& other)
    {
        SharedPtr cpy(other);
        swap(m_refCnt, cpy.m_refCnt);
        swap(m_ptr, cpy.m_ptr);
        
        return *this;
    }
    
    // Move ctr
    SharedPtr(SharedPtr&& other)
        : m_ptr(other.m_ptr)
        , m_refCnt(other.m_refCnt)
    {
        other.m_ptr    = nullptr;
        other.m_refCnt = nullptr;
    }
    
    // Move assignment
    SharedPtr& operator= (SharedPtr&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        delete m_refCnt;
        delete m_ptr;
        
        m_refCnt = other.m_refCnt;
        m_ptr = other.m_ptr;
        
        other.m_ptr    = nullptr;
        other.m_refCnt = nullptr;
        
        return *this;
    }
    
    T* operator ->()
    {
        return m_ptr;
    }
    
    T& operator*()
    {
        return *m_ptr;
    }
    ~SharedPtr()
    {
        if (m_refCnt != nullptr)
        {
            (*m_refCnt)--;
            
            if ((*m_refCnt) == 0)
            {
                delete m_ptr;
                m_ptr = nullptr;
                delete m_refCnt;
                m_refCnt = nullptr;
            }
        }
    }
private:
    T* m_ptr;
    int* m_refCnt;
};

class Test
{
public:
    Test()
    {
    }
    
    Test (string str)
    {
        m_str = str;
    }
    
    void setStr(const string& str)
    {
        m_str = str;
    }
    
    string getStr()
    {
        return m_str;
    }
private:
    string m_str;
};

// To execute C++, please define "int main()"
int main() {
    
    SharedPtr<Test> sp(new Test("Good Luck,"));
    
    cout << sp->getStr();
    SharedPtr<Test> spCpy(sp);
    spCpy->setStr("Tianyu Xia! ");
    cout << spCpy->getStr();
    
    SharedPtr<Test> spAssign(new Test("Get Google Offer!"));
    sp = spAssign;
    cout << sp->getStr() << endl;
    return 0;
}

