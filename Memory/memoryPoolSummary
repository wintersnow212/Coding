https://zhuanlan.zhihu.com/p/64719710
https://github.com/double-free/tiny-projects/blob/master/memory-pool/MemoryPool.h
https://www.zhihu.com/question/25527491
https://zhuanlan.zhihu.com/p/51855842

efficient c++ programing techniques book


http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
https://github.com/mtrebi/memory-allocators


memory pool 是也就是buddy allocation:
allocate 一大块memory 减少 malloc的次数 但是缺点是就是固定长度了 
不像malloc是dynmaic size 的所以他有什么first fit best fit stragety 
buddy system is internal fragmentation as larger block of memory is acquired then required. 
For example if a 36 kb request is made then it can only be satisfied by 64 kb segment and reamining memory is wasted.
感觉memory pool 就是像stack一般的linked list就可以 因为他不需要是sorted linked list 所以有free block就insert到头
malloc比较复杂 他要在中间insert 所以用doubly linked list 比较好

union的用法比较疑惑 https://thinkingeek.com/2017/11/19/simple-memory-pool/ 
但是感觉其实就是为了return 自身地址

linked list node structure其实不需要data 只需要node* next;
其实node本身就是chunk的首地址 加上next的辅助把间断的chunk都连起来了！！！

Slab allocation:

A second strategy for allocating kernel memory is known as slab allocation. It eliminates fragmentation caused by allocations and deallocations. 
This method is used to retain allocated memory that contains a data object of a certain type for reuse upon subsequent allocations of objects of the same type. 
In slab allocation memory chunks suitable to fit data objects of certain type or size are preallocated. Cache does not free the space immediately after use although it keeps track of data which are required frequently so that whenever request is made the data will reach very fast. 
Two terms required are:

Slab – A slab is made up of one or more physically contiguous pages. The slab is the actual container of data associated with objects of the specific kind of the containing cache.
Cache – Cache represents a small amount of very fast memory. A cache consists of one or more slabs. There is a single cache for each unique kernel data structure.
12

Example –

A separate cache for a data structure representing processes descriptors
Separate cache for file objects
Separate cache for semaphores etc.
Each cache is populated with objects that are instantiations of the kernel data structure the cache represents. For example the cache representing semaphores stores instances of semaphore objects, the cache representing process descriptors stores instances of process descriptor objects.

Implementation –
The slab allocation algorithm uses caches to store kernel objects. When a cache is created a number of objects which are initially marked as free are allocated to the cache. The number of objects in the cache depends on size of the associated slab.
Example – A 12 kb slab (made up of three contiguous 4 kb pages) could store six 2 kb objects. Initially all objects in the cache are marked as free. When a new object for a kernel data structure is needed, the allocator can assign any free object from the cache to satisfy the request. The object assigned from the cache is marked as used.

In linux, a slab may in one of three possible states:

Full – All objects in the slab are marked as used
Empty – All objects in the slab are marked as free
Partial – The slab consists of both
The slab allocator first attempts to satisfy the request with a free object in a partial slab. If none exists, a free object is assigned from an empty slab. If no empty slabs are available, a new slab is allocated from contiguous physical pages and assigned to a cache.

Benefits of slab allocator –

No memory is wasted due to fragmentation because each unique kernel data structure has an associated cache.
Memory request can be satisfied quickly.
The slab allocating scheme is particularly effective for managing when objects are frequently allocated or deallocated. The act of allocating and releasing memory can be a time consuming process. However, objects are created in advance and thus can be quickly allocated from the cache. When the kernel has finished with an object and releases it, it is marked as free and return to its cache, thus making it immediately available for subsequent request from the kernel.

