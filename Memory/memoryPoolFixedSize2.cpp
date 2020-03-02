#include <iostream>
 
using namespace std;

/************************************************************
Reference: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
这个memory pool 是固定chunk size，但是静态设置chunk number
所以你每次allocate拿到的就是固定size的chunk 在这个chunk内memory是连续的
一个allocate (chunk number * chunk size) 大小的block
最开始这么大的block是连续的 但是经过 deallocate后 只有每个chunk内是连续的
利用free list把不连续的各个chunk连接起来
************************************************************/
/* header
union Chunk
{
    struct
    {
        union Chunk* next;
        // 这里不需要size 因为是fixed size的 
        // malloc这种variable size的就需要了
        int size;
    }tag;
    
    long align;
};
*/
struct Chunk
{
    Chunk* next;  
};

// It is used when we need to fast-allocate a lot of objects with 
// fixed size !!!
class PoolAllocator
{
public:
    PoolAllocator(int chkPerBlock = 0)
        : chunksPerBlock(chkPerBlock)
        , pFreeList(nullptr)
    {

    }
    // 所以这里的size其实是chunk Size 而其实fixed size
    void* allocate(int size)
    {
        if (pFreeList == nullptr)
        {
            pFreeList = allocateBlock(size);
        }
        
        void* pRet = pFreeList;
        pFreeList  = pFreeList->next;
        
        return pRet;
    }
    
    
    void deallocate(void* pChunk)
    {
        reinterpret_cast<Chunk*>(pChunk)->next = pFreeList;
        pFreeList = reinterpret_cast<Chunk*>(pChunk);
    }
    
private:
    Chunk* allocateBlock(int chunkSize)
    {
        cout << "\nAllocating block (" << chunksPerBlock << " chunks):\n\n";
        int totalSize = chunksPerBlock * chunkSize;
        // 所以这里就是char* 为单位的 所以可以看出这里就是1Byte
        
        Chunk* pBegin = static_cast<Chunk*>(malloc(totalSize));
        Chunk* pTrav = pBegin;
        for (int i = 0; i < chunksPerBlock - 1; ++i)
        {
            // 这里是关键要cast成char* 也就是byte的单位 然后进行移动
            pTrav->next = reinterpret_cast<Chunk*>((char*)pTrav + chunkSize);
            pTrav = pTrav->next;
        }
        pTrav->next = nullptr;
        return pBegin;
    }
    
    int chunksPerBlock;
    Chunk* pFreeList;
};


/**********************************************

Usage function !!!!

**********************************************/
struct Object 
{
  // Object data, 16 bytes:
 
  uint64_t data[2];
 
  // Declare out custom allocator for
  // the `Object` structure:
 
    static PoolAllocator allocator;
    // https://eli.thegreenplace.net/2011/02/17/the-many-faces-of-operator-new-in-c
    // 这个自动就知道size 是16 byte
    static void *operator new(size_t size) {
        cout << "WTF!!!!" << size << endl;
        return allocator.allocate(size);
    }
 
    static void operator delete(void* ptr, size_t size) 
    {
      return allocator.deallocate(ptr);
        //return allocator.deallocate(ptr, size);
    }
};
 
// Instantiate our allocator, using 8 chunks per block:
PoolAllocator Object::allocator{8};

 
int main(int argc, char const *argv[]) {
 
  // Allocate 10 pointers to our `Object` instances:
  constexpr int arraySize = 10;
 
  Object *objects[arraySize];
 
  // Two `uint64_t`, 16 bytes.
  cout << "size(Object) = " << sizeof(Object) << endl << endl;
 
  // Allocate 10 objects. This causes allocating two larger,
  // blocks since we store only 8 chunks per block:
 
  cout << "About to allocate " << arraySize << " objects" << endl;
 
  for (int i = 0; i < arraySize; ++i) {
    objects[i] = new Object();
    cout << "new [" << i << "] = " << objects[i] << endl;
  }
 
  cout << endl;
 
  // Deallocated all the objects:
 
  for (int i = arraySize - 1; i >= 0; --i) {
    cout << "delete [" << i << "] = " << objects[i] << endl;
    delete objects[i];
  }
 
  cout << endl;
 
  // New object reuses previous block:
 
  objects[0] = new Object();
  cout << "new [0] = " << objects[0] << endl << endl;
}
/* 
