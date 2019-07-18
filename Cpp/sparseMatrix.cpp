// C++ program for Sparse Matrix Representation 
// using Array 
#include<stdio.h> 
#include<iostream> 
using namespace std;
struct Trituple
{        //自定义数据结构：矩阵元素的行，列，值；
    int row;
    int col;
    int value;
    
    Trituple& operator=(Trituple& x)
    {
        //赋值运算符重载
        row=x.row;
        col=x.col;
        value=x.value;
        return *this;
    }
};

class spMatrix
{
public:
    spMatrix(int noZeroSize)
        : pos(new Trituple[noZeroSize])
    {
        
    }
    
    ~spMatrix ()
    {
        delete pos;
    }
private:
    Trituple* pos;
};

/*
稀疏矩阵类：

     该类型的矩阵中0元素占多半，我们平常储存一般矩阵一般用二维数组，但是该矩阵的0元素既占用存储空间，而且在运算中会花费大量时间来进行0元素的无效运算。

因此我们利用三元数组（注意不是三维数组）存储非零元素的坐标和值
*/
int main() 
{ 
    // Assume 4x5 sparse matrix 
    int sparseMatrix[4][5] = 
    { 
        {0 , 0 , 3 , 0 , 4 }, 
        {0 , 0 , 5 , 7 , 0 }, 
        {0 , 0 , 0 , 0 , 0 }, 
        {0 , 2 , 6 , 0 , 0 } 
    }; 
  
    int size = 0; 
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 5; j++) 
            if (sparseMatrix[i][j] != 0) 
                size++; 
  
    /* number of columns in compactMatrix (size) must be 
       equal to number of non - zero elements in 
       sparseMatrix 
       Row: Index of row, where non-zero element is located
       Column: Index of column, where non-zero element is located
       Value: Value of the non zero element located at index – (row,column)
  */
    cout << size << endl;
    int compactMatrix[3][size]; 
  
    // Making of new matrix 
    int k = 0; 
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 5; j++) 
            if (sparseMatrix[i][j] != 0) 
            { 
                compactMatrix[0][k] = i; 
                compactMatrix[1][k] = j; 
                compactMatrix[2][k] = sparseMatrix[i][j]; 
                k++; 
            } 
  
    for (int i=0; i < 3; i++) 
    { 
        for (int j= 0; j < size; j++) 
            printf("%d ", compactMatrix[i][j]); 
  
        printf("\n"); 
    } 
    return 0; 
} 