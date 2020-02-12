https://zhuanlan.zhihu.com/p/64719710
https://github.com/double-free/tiny-projects/blob/master/memory-pool/MemoryPool.h
https://www.zhihu.com/question/25527491
https://zhuanlan.zhihu.com/p/51855842

efficient c++ programing techniques book


http://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/
http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
https://github.com/mtrebi/memory-allocators

memory pool 是allocate 一大块memory 减少 malloc的次数 但是缺点是就是固定长度了 
不像malloc是dynmaic size 的所以他有什么first fit best fit stragety 

感觉memory pool 就是像stack一般的linked list就可以 因为他不需要是sorted linked list 所以有free block就insert到头
malloc比较复杂 他要在中间insert 所以用doubly linked list 比较好

union的用法比较疑惑 https://thinkingeek.com/2017/11/19/simple-memory-pool/ 
但是感觉其实就是为了return 自身地址

linked list node structure其实不需要data 只需要node* next;
其实node本身就是chunk的首地址 加上next的辅助把间断的chunk都连起来了！！！
