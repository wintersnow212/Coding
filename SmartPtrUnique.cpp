#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Give the ownership of heap allocated memory to stack varible
// so that when it is out of scope, it will be freed autonmatically
template<typename T>
class UniquePtr
{
public:
    UniquePtr()
        : m_ptr(nullptr)
    {
        
    }
    
    // This is actual a shadow copy 
    // but it is ok because the point here is let m_Ptr share the
    // reference of ptr so that we can free their shared memory
    UniquePtr(T* ptr)
        : m_ptr(ptr)
    {
        
    }
    
   // Move constructor 
    UniquePtr(T&& other)
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }
    
    ~UniquePtr()
    {
        delete m_ptr;
    }
    
    
    UniquePtr (const UniquePtr<T>& other) = delete;
    
    UniquePtr<T>& operator= (const UniquePtr<T>& other) = delete;
    
private:
    T* m_ptr;
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

int main()
{
    UniquePtr<AirCraft> uP = new AirCraft("Unique F-22 Tiger");
    // unique pointer是不能copy的 因为是unqiue ownership啊
    //UniquePtr<AirCraft> uPCopy = uP;
    return 0;
}