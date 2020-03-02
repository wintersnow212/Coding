#include <iostream>
#include <cassert>
using namespace std;

/************************************************************
这里的memory pool 其实是fixed size buddy allocator 
优点是消除external memory fragmentation(enough memory but not continuous
缺点是internal memory fragmentation ----- 每个chunk可能有memory waste
这里的区别是
1. input 不同 这里我们的chunkSize和chunksPerBlock 都是动态的
   而且是通过constructor传给我们. 
2. 我们可以再constructor里面 allocate block 也可以在allocate的时候laze allocation
   
我们在Init pool 的时候就已经allocate了一块
Reference: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
这个memory pool 是固定chunk size，但是静态设置chunk number
所以你每次allocate拿到的就是固定size的chunk 在这个chunk内memory是连续的
一个allocate (chunk number * chunk size) 大小的block
最开始这么大的block是连续的 但是经过 deallocate后 只有每个chunk内是连续的
利用free list把不连续的各个chunk连接起来
************************************************************/
// 有点像linked list 的node 但是这里的作用只是连接 所以不需要data
struct Chunk
{
    Chunk* next;  
};

// It is used when we need to fast-allocate a lot of objects with 
// fixed size !!!
class PoolAllocator
{
public:
    PoolAllocator(int chkSize = 0, int chkPerBlock = 0)
        : chunkSize(chkSize)
        , chunksPerBlock(chkPerBlock)
        , pFreeList(nullptr)
    {
        // Init allocate a big block
        pFreeList = allocateBlock();
    }

    // 这里是不需要size argument的 因为我们是fixed size memory pool
    void* allocate()
    {
        if (pFreeList == nullptr)
        {
            pFreeList = allocateBlock();
        }
        
        void* pRet = pFreeList;
        pFreeList  = pFreeList->next;
        
        return pRet;
    }
    
    // 但是dealocate 其实是需要 pointer argument 这样我们才知道应该free哪个
    void deallocate(void* pChunk)
    {
        reinterpret_cast<Chunk*>(pChunk)->next = pFreeList;
        pFreeList = reinterpret_cast<Chunk*>(pChunk);
    }
    
private:
    Chunk* allocateBlock()
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
    
    int chunkSize;
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
        assert(size == sizeof(Object));
        return allocator.allocate( );
    }
    
    static void operator delete(void* ptr) 
    {
      return allocator.deallocate(ptr);
        //return allocator.deallocate(ptr, size);
    }
};
 
// Instantiate our allocator, using 8 chunks per block:
PoolAllocator Object::allocator{sizeof(Object), 8};

 
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
