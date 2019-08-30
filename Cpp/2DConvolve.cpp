// Given a 2D array of N by N input pixels, convolve those pixels with a
// M by M kernel to produce an N by N output pixel array. For example if
// N=10 and M=5, each output pixel is the the weighted sum of 25 input
// pixels centered around the output pixels, like this:
//
// Input pixels (10x10)     Output pixels (10x10)    Kernel (5x5)
// . . . . . . . . . .      . . . . . . . . . .      w w w w w
// . . . . . . . . . .      . . . . . . . . . .      w w w w w
// . i i i i i . . . .      . . . . . . . . . .      w w W w w
// . i i i i i . . . .      . . . . . . . . . .      w w w w w
// . i i I i i . . . .      . . . O . . . . . .      w w w w w
// . i i i i i . . . .      . . . . . . . . . .
// . i i i i i . . . .      . . . . . . . . . .
// . . . . . . . . . .      . . . . . . . . . .
// . . . . . . . . . .      . . . . . . . . . .
// . . . . . . . . . .      . . . . . . . . . .
//   
// Here the 5x5 input pixels (i) around the output pixel (O) are multiplied
// by the kernel values (w) and those 25 values are summed together to produce
// the output value (O). This is done for every output pixel, 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Convolve(int N, int M, double *input, double *output, double *kernel) {
//5 * 5
  // output[i][j] = input[i-2][j-2] * kernel[0][0] +
  //                input[i-1][j-2] * kernel[1][0] +
  //                .... etc for 25 multiply adds in total ...
    // https://blog.csdn.net/gotowu/article/details/48806397
    int kfR = 0;
    int kfC = 0;
    int rr  = 0;
    int cc  = 0;
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            for (int kR = 0; kR < M; ++kR)
            {
                // 也就是旋转180度
                // （1）将h先上下翻转
                kfR = M - 1 - kR;
                
                for (int kC = 0; kC < M; ++kC)
                {
                    // (2) 再左右翻转
                    kfC = M - 1 - kC;
                    
                    // 中心重合http://www.songho.ca/dsp/convolution/convolution2d_example.html
                    // If the kernel is centered (aligned) exactly at the sample that 
                    // we are interested in, multiply the kernel data by the overlapped input data.
                    // 所以应该是 kfR - m/2 而不是 m/2 - kfR
                    rr = r + (kfR - (M/2));
                    cc = c + (kfC - (M/2));
            
                    if (rr < 0 || rr >= N || cc < 0 || cc >= N)
                    {
                        continue;
                    }
                    // r*N + c
                    // kfR * M + kfC
                    //output[r][c] += (input[rr][cc] + kernel[kfR][kfC]);
                    output[r * N + c] +=
                        (input[rr * N +cc] * kernel[kfR * M + kfC]);
                }
            }
                
        }
    }
}

//--------------------------------------------------------------------------
// Test the Convolve() function.

#include <string.h>

#define CHECK_EQ(a,b) if ((a) != (b)) { \
  printf("Check failed at line %d, %s != %s\n", __LINE__, #a, #b); \
  return 0; \
}

int main() {
  double input[10*10] = {
     3,     9,     4,     1,     5,     9,     7,     6,     6,     5,
     5,     9,     2,     9,     3,     7,     6,     9,     3,     1,
     7,     4,     0,     5,     4,     5,     4,     4,     1,     9,
     4,     2,     8,     6,     1,     4,     3,     0,     0,     2,
     4,     7,     1,     0,     2,     2,     8,     8,     4,     9,
     1,     7,     1,     8,     0,     7,     3,     6,     1,     2,
     0,     7,     6,     7,     9,     2,     8,     3,     7,     3,
     2,     7,     8,     1,     6,     0,     7,     9,     3,     0,
     3,     1,     5,     5,     9,     7,     8,     2,     8,     6,
     6,     6,     7,     3,     1,     6,     5,     6,     7,     1};
  double kernel[5*5] = {
     0,     6,     8,     2,     9,
     7,     0,     3,     2,     0,
     3,     9,     6,     4,     5,
     6,     8,     5,     2,     0,
     3,     7,     5,     8,     8};
  double expected_output[10*10] = {
   184,   300,   394,   364,   362,   417,   421,   459,   373,   249,
   246,   413,   400,   411,   421,   377,   365,   372,   302,   204,
   277,   379,   463,   470,   475,   605,   544,   607,   431,   324,
   280,   492,   487,   520,   442,   461,   444,   441,   368,   253,
   260,   407,   515,   451,   514,   541,   463,   609,   410,   325,
   317,   396,   483,   573,   472,   487,   477,   403,   379,   275,
   217,   379,   529,   578,   597,   621,   574,   715,   486,   404,
   276,   457,   437,   571,   530,   604,   619,   531,   467,   284,
   177,   338,   463,   450,   529,   380,   503,   433,   367,   340,
   208,   239,   339,   241,   296,   307,   338,   323,   285,   137};

  double output[10*10];
  memset(output,0,sizeof(output));
  Convolve(10, 5, input, output, kernel);
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      printf("%6.0f", output[y*10 + x]);
    }
    printf("\n");
  }
  for (int i = 0; i < 10*10; i++) {
    CHECK_EQ(output[i], expected_output[i]);
  }

  printf("Success!\n");
  return 0;
}
