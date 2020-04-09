#include <iostream>
#include <cstring>
using namespace std;

class MyString
{
public:
    MyString()
        : m_pChar(nullptr)
        , m_pRefCnt(nullptr)
    {
        
    }
    
    MyString(const char* pInput)
        : m_pRefCnt(new int(1))
    {
        int len = strlen(pInput);
        
        m_pChar = new char[len + 1];
        
        strcpy(m_pChar, pInput);
    }
    
    MyString(const MyString& other)
        : m_pChar(other.m_pChar)
        , m_pRefCnt(other.m_pRefCnt)
    {
        (*m_pRefCnt)++;
    }
    
    MyString(MyString&& other)
        : m_pChar(other.m_pChar)
        , m_pRefCnt(other.m_pRefCnt)
    {
        other.m_pChar = nullptr;
        other.m_pRefCnt = nullptr;
    }
    
    char* GetStr()
    {
        return m_pChar;
    }
    
    ~MyString()
    {
        if (m_pRefCnt != nullptr)
        {
            (*m_pRefCnt)--;
            if (*(m_pRefCnt) == 0)
            {
                if (m_pChar != nullptr)
                {
                    // 这里double free了 虽然这个class自己有nullptr check
                    // 但是因为上面的浅拷贝 两个pointer指向同一块内存
                    // 这个double free 不是nullptr check能避免的 
                    // 因为我们要想着underlying memory
                    // 解决方案 借助shared pointer的refCnt
                    delete [] m_pChar;
                }
            }
        }
    }
    
private:
    char* m_pChar;
    int* m_pRefCnt;
};

// To execute C++, please define "int main()"
int main() {
    MyString mStr("Tianyu xia get offer");
    MyString mCpy(mStr);
    MyString mMove("Tianyu xia get offer again");
    cout << mStr.GetStr() << endl;
    cout << mMove.GetStr() << endl;
    return 0;
}
