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
    
    ~UniquePtr()
    {
        delete m_ptr;
    }
    
    
    UniquePtr (const UniquePtr<T>& other) = delete;
    
    UniquePtr<T>& operator= (const UniquePtr<T>& other) = delete;
    
private:
    T* m_ptr;
};
