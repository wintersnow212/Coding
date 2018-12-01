
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

int main()
{
    bool mulArray[3][4];
    // 注意1 2D array的declare
    bool** mulTest;
    mulTest = new bool* [3];
    for (int r = 0; r < 3; r++)
    {
        *mulTest =  new bool[4];

        // 注意2 2D array的init
        for (int c = 0; c < 4; c++)
            mulTest[r][c] = false;
    }

    // 注意3 这个multArray的memset是可以的 但是下面的mulTest的memset是不可以的
    // 因为static allocated保证memory是连续的而 分次dynamic allocated的不保证的
    memset(mulArray, 0, 3*4*sizeof(bool));
    memset(mulTest, 0, 3*4*sizeof(bool));

    // 注意4 一次性dynamic allocated的memory是保证连续的是可以memset
    int* test = new int [4];
    memset(test, 0, 4*sizeof(int));
    cout << test[3];

    // memset 只能init 0 而不能init成为其他值因为 他是以byte为单位init的
    return 0;
}
