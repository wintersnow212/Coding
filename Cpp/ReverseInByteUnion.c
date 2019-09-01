
#include <stdio.h>
#include <stdint.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef union
{
    uint32_t dword;
    struct
    {
        uint8_t lo_byte_short0;
        uint8_t hi_byte_short0;
        uint8_t lo_byte_short1;
        uint8_t hi_byte_short1;
    } bytes;
} Word;

Word Reverse(Word w)
{
    Word result;
    result.bytes.lo_byte_short0 = w.bytes.hi_byte_short1;
    result.bytes.hi_byte_short0 = w.bytes.lo_byte_short1;
    result.bytes.lo_byte_short1 = w.bytes.hi_byte_short0;
    result.bytes.hi_byte_short1 = w.bytes.lo_byte_short0;
    return result;
}

int main()
{
    Word a;
    a.dword = 0x12345678;
    printf("Reverse byte order: %2x\n\n", Reverse(a).dword);
    return 0;
}
