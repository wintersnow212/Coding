#include <stdio.h>
#include <stdint.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
ntohl() //Network to Host byte order (Long)
htonl() //Host to Network byte order (Long)

ntohs() //Network to Host byte order (Short)
htons() //Host to Network byte order (Short)
*/

// bitfield flags;

union Test
{
    struct 
    {
        uint32_t f1 : 1;  // 这里的bitfiled size不能少！！！！！！
        uint32_t f2 : 1;
        uint32_t f3 : 1;
        uint32_t f4 : 1;
        uint32_t f5 : 1;
        uint32_t f6 : 1;
        uint32_t reserved : 26;
    }flags;
    
    uint32_t u32All;
};

void swapByteOrder(unsigned short& us)
{
    us = (us >> 8) |
         (us << 8);
}

void swapByteOrder(unsigned int& val)
{
    // ui = (ui >> 24)               |
    //      ((ui << 8) & 0x00FF0000) |
    //      ((ui >> 8) & 0x0000FF00) |
    //      (ui << 24);
    val =  ((val & 0x000000ff) << 24 | (val & 0x0000ff00) << 8 |
           (val & 0x00ff0000) >> 8   | (val & 0xff000000) >> 24);
}

void swapByteOrder(unsigned long long& ull)
{
    ull = (ull >> 56) |
          ((ull<<40) & 0x00FF000000000000) |
          ((ull<<24) & 0x0000FF0000000000) |
          ((ull<<8) & 0x000000FF00000000) |
          ((ull>>8) & 0x00000000FF000000) |
          ((ull>>24) & 0x0000000000FF0000) |
          ((ull>>40) & 0x000000000000FF00) |
          (ull << 56);
}

template<typename T>
T swap_endian(T val)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
    
    union 
    {
        char data[sizeof(T)];
        T val;
    }src, dst;
    
    src.val = val;
    int len = sizeof(T);
    for (int i = 0; i < len; ++i)
    {
        dst.data[len - 1 - i] = src.data[i];
    }
    
    return dst.val;
}

// To execute C++, please define "int main()"
int main() {
    unsigned int val = 0x12345678;
    printf("Reverse byte order: %2x\n\n", swap_endian<int>(val));
    
    swapByteOrder(val);
    printf("Reverse byte order: %2x\n\n", val);
    
    int arr[8] = {};
    printf("Size of array: %d\n", sizeof(arr));
    return 0;
}

