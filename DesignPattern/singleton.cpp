class Singleton{
private:
    Singleton();
    Singleton(const Singleton& other);
public:
    static Singleton* getInstance();
    // 这里也很巧妙static memeber function can access static member!!!!
    static Singleton* m_instance;
};
// 这里很关键 initialize stastic parameters, compile errors occur without them
Singleton* Singleton::m_instance=nullptr;

//线程非安全版本
Singleton* Singleton::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new Singleton();
    }
    return m_instance;
}

//线程安全版本，但锁的代价过高
Singleton* Singleton::getInstance() {
    Lock lock;
    if (m_instance == nullptr) {
        m_instance = new Singleton();
    }
    return m_instance;
}

//双检查锁，但由于内存读写reorder不安全
Singleton* Singleton::getInstance() {
    
    if(m_instance==nullptr){
        Lock lock;
        if (m_instance == nullptr) {
            m_instance = new Singleton();
        }
    }
    return m_instance;
}

//C++ 11版本之后的跨平台实现 (volatile)
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton* Singleton::getInstance() {
    Singleton* tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);//获取内存fence
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton;
            std::atomic_thread_fence(std::memory_order_release);//释放内存fence
            m_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}

//https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Singleton
class StringSingleton
 {
 public:
     // Some accessor functions for the class, itself
     std::string GetString() const 
     {return mString;}
     void SetString(const std::string &newStr)
     {mString = newStr;}
 
     // The magic function, which allows access to the class from anywhere
     // To get the value of the instance of the class, call:
     //     StringSingleton::Instance().GetString();
     static StringSingleton &Instance()
     {
         // This line only runs once, thus creating the only instance in existence
         // 这个是static后就不会被 free了 life time
         // static 对于global 这种本来已经是全局life time的改变的就是scope 
         // --- static function, every file include have is own copy and must have its own implementation
         // static inline
         /*
         inline does not mean you can use the function “inline” (it is normal to use functions inside other functions; you do not need  inline for that); it encourages the compiler to 
         build the function into the code where it is used (generally with the goal of improving execution speed).
         static inline is usually used with small functions that are better done in the calling routine than by using a call mechanism, s
         imply because they are so short and fast that actually doing them is better than calling a separate copy
          */
         // 头文件中的 static 函数会在每个文件中生成一份代码，这造成代码冗余倒不是最大的问题，最大的问题是可能带来库文件与工程文件同一函数的代码的不一致性
         // 对于 local 改变的是scope 
         static std::auto_ptr<StringSingleton> instance( new StringSingleton );
         // dereferencing the variable here, saves the caller from having to use 
         // the arrow operator, and removes temptation to try and delete the 
         // returned instance.

         // 这个需要deference的原因是因为smart pointer就是当做pointer使用 然后你要return its object
         return *instance; // always returns the same instance
     }
 
 private: 
     // We need to make some given functions private to finish the definition of the singleton
     StringSingleton(){} // default constructor available only to members or friends of this class
 
     // Note that the next two functions are not given bodies, thus any attempt 
     // to call them implicitly will return as compiler errors. This prevents 
     // accidental copying of the only instance of the class.
     StringSingleton(const StringSingleton &old); // disallow copy constructor
     const StringSingleton &operator=(const StringSingleton &old); //disallow assignment operator
 
     // Note that although this should be allowed, 
     // some compilers may not implement private destructors
     // This prevents others from deleting our one single instance, which was otherwise created on the heap
     ~StringSingleton(){} 
 private: // private data for an instance of this class
     std::string mString;
 };
