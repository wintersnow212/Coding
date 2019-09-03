00001 /*
00002  * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
00003  *      The President and Fellows of Harvard College.
00004  *
00005  * Redistribution and use in source and binary forms, with or without
00006  * modification, are permitted provided that the following conditions
00007  * are met:
00008  * 1. Redistributions of source code must retain the above copyright
00009  *    notice, this list of conditions and the following disclaimer.
00010  * 2. Redistributions in binary form must reproduce the above copyright
00011  *    notice, this list of conditions and the following disclaimer in the
00012  *    documentation and/or other materials provided with the distribution.
00013  * 3. Neither the name of the University nor the names of its contributors
00014  *    may be used to endorse or promote products derived from this software
00015  *    without specific prior written permission.
00016  *
00017  * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
00018  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
00019  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
00020  * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
00021  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
00022  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
00023  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
00024  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
00025  * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
00026  * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
00027  * SUCH DAMAGE.
00028  */
00029 
00030 /*
00031  * This file is shared between libc and the kernel, so don't put anything
00032  * in here that won't work in both contexts.
00033  */
00034 
00035 #ifdef _KERNEL
00036 #include <types.h>
00037 #include <lib.h>
00038 #else
00039 #include <stdint.h>
00040 #include <string.h>
00041 #endif
00042 
00043 /*
00044  * C standard function - copy a block of memory, handling overlapping
00045  * regions correctly.
00046  */
00047 
00048 void *
00049 memmove(void *dst, const void *src, size_t len)
00050 {
00051         size_t i;
00052 
00053         /*
00054          * If the buffers don't overlap, it doesn't matter what direction
00055          * we copy in. If they do, it does, so just assume they always do.
00056          * We don't concern ourselves with the possibility that the region
00057          * to copy might roll over across the top of memory, because it's
00058          * not going to happen.
00059          *
00060          * If the destination is above the source, we have to copy
00061          * back to front to avoid overwriting the data we want to
00062          * copy.
00063          *
00064          *      dest:       dddddddd
00065          *      src:    ssssssss   ^
00066          *              |   ^  |___|
00067          *              |___|
00068          *
00069          * If the destination is below the source, we have to copy
00070          * front to back.
00071          *
00072          *      dest:   dddddddd
00073          *      src:    ^   ssssssss
00074          *              |___|  ^   |
00075          *                     |___|
00076          */
00077 
00078         if ((uintptr_t)dst < (uintptr_t)src) {
00079                 /*
00080                  * As author/maintainer of libc, take advantage of the
00081                  * fact that we know memcpy copies forwards.
00082                  */
00083                 return memcpy(dst, src, len);
00084         }
00085 
00086         /*
00087          * Copy by words in the common case. Look in memcpy.c for more
00088          * information.
00089          */
00090 
00091         if ((uintptr_t)dst % sizeof(long) == 0 &&
00092             (uintptr_t)src % sizeof(long) == 0 &&
00093             len % sizeof(long) == 0) {
00094 
00095                 long *d = dst;
00096                 const long *s = src;
00097 
00098                 /*
00099                  * The reason we copy index i-1 and test i>0 is that
00100                  * i is unsigned -- so testing i>=0 doesn't work.
00101                  */
00102 
00103                 for (i=len/sizeof(long); i>0; i--) {
00104                         d[i-1] = s[i-1];
00105                 }
00106         }
00107         else {
00108                 char *d = dst;
00109                 const char *s = src;
00110 
00111                 for (i=len; i>0; i--) {
00112                         d[i-1] = s[i-1];
00113                 }
00114         }
00115 
00116         return dst;
00117 }
