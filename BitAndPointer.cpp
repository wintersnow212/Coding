#include <iostream>
//master branch change
using namespace std;

int main()
{
    int num = 5682;
    char* pOut = static_cast<char*>(malloc(sizeof(char)*8));
    char* lookUp = "0123456789abcdef";
    
    
    /* i can not be larger than 7, it will result in accessing out of bound of allocated
       memory.
       This is undefined behavor https://en.wikipedia.org/wiki/Undefined_behavior
       http://www.yolinux.com/TUTORIALS/C++MemoryCorruptionAndMemoryLeaks.html
       
       Basically, the memory allocator allocates pages of memory at once for use
       by programs, and it gives you a pointer within them (making sure the following space
       is free for use). Since these pages are usually bigger than 8KiB, you have no issue
       in your mini-program. But if a larger program is allocating larger amounts of memory
       and writing further and further past the end of your allocated space, then you'll end
       up attempting to write into unallocated memory (or memory used by another program!),
       thus corrupting memory.
*/
    for (int i = 7; i >= 0; i--)
    {
        cout << (num&0xf)<< endl;
        *(pOut + i) = lookUp[num&0xf];
        num >>= 4;
    }

    printf ("Output is 0x%s\n", pOut);
    return 0;
}
