#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iterator>
using namespace std;


int CUTOFF = 7;

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/********************************************************************************************************************************************************
 *    BubbleSort:
 *    --- last i elements are already sorted/in place
 *        从前向后
 *    InsertionSort
 *    ---- 向前sort
 *
 ********************************************************************************************************************************************************/

void BubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void InsertionSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j > 0; j--)
        {
            if (arr[j] < arr[j - 1])
                swap(&arr[j], &arr[j - 1]);
        }
    }
}

void SelectionSort(int arr[], int n)
{
    
    for (int i = 0; i < n; i++)
    {
        int min_index = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[min_index] > arr[j])
                min_index = j;
        }
        
        swap(&arr[i], &arr[min_index]);
    }
    
}

/******************************************************************************************************************************************
 *    MergeSortTopDown：
 *    先局部有序再整体有序。Merge function 是使其有序的关键！!
 *    Basic plan for
 *       . Divide array into two halves. 先分半！！！！
 *        . Recursively sort each half.
 *        . Merge two halves.
 *    缺点 一定需要额外空间 arrAux 辅助数组 时间复杂度 是nlg(n)
 ******************************************************************************************************************************************/
// 这里特别注意Merge function的函数三个parameter: start mid end
void Merge(int arr[], int arrAux[], int start, int mid, int end)
{
    int left = start;
    int right = mid + 1;
    int index = start;

    while (left <= mid && right <= end)
    {
        if (arr[left] < arr[right])
        {
            arrAux[index++] = arr[left++];
        }
        else
        {
            arrAux[index++] = arr[right++];
        }
    }

    while (left <= mid)
    {
        arrAux[index++] = arr[left++];
    }

    while (right <= end)
    {
        arrAux[index++] = arr[right++];
    }

    // 这里是关键！！！！ i是从start开始的！！！
    for (int i = start; i <= end; ++i)
    {
        arr[i] = arrAux[i];
    }
}

void MergeSortTopDown(int arr[], int arrAux[], int start, int end)
{
    // Optimization 1 ：Use inserion sort for small subarrays
    if (end < start + CUTOFF - 1)
    {
        InsertionSort(arr, end - 1);
    }
    // @Note 特别注意 这里是关键！！！！！！！ 是>= 而不是>
    // > 会出现死循环 因为 start 和 mid 相同
    if (start >= end)
        return;
    
    int mid = start + (end - start)/2;
    MergeSortTopDown(arr, arrAux, start, mid);
    MergeSortTopDown(arr, arrAux, mid + 1, end);
    
    // Optimization 2 : Stop if already sorted --- biggest item in first half <= smallest item in second half
    if (arr[mid] <= arr[mid+1])
        return;
    Merge(arr, arrAux, start, mid, end);
    
    // Optimization 3 ：Recursion argument switch 没懂。。。
}

/****************************************************************************************************************************************************
 // !!!! 这里的是Top-Down solution 使用了recurison !!!
 // 研究bottom-up Merge Sort solution
 https://leetcode.com/problems/sort-list/discuss/46712/Bottom-to-up(not-recurring)-with-o(1)-space-complextity-and-o(nlgn)-time-complextity
 *    MergeSortBottomUp：
 *    Basic plan for
 *       . Pass through array, merging subarrays of size 1
 *        . Repeat for subarrays of size 2, 4, 8, 16
 *    优点不需要recursion了
 *    缺点 Use extra sapce proportional to size of the array
 *    How many passes (over the input array) does bottom-up mergesort make
 *    in the worst case?
 ***************************************************************************************************************************************************/
void MergeSortBottomUp(int arr[], int arrAux[], int size)
{
    // 关键 Step的变化是 1， 2， 4.....乘以2的变化
    for (int step = 1; step < size; step = step*2)
    {
        for (int start = 0; start < size - step; start += step*2)
        {
            // 这里的start; mid; end 的index非常巧妙！！！！ mid = start + step - 1; end = start + 2*step - 1;
            int end = start + 2*step -1;
            // 同时特别注意 end的值是比较后的
            // mid 是 start index move steps 后
            Merge(arr, arrAux, start, start + step - 1, (end > size - 1) ? size - 1 : end);
        }
    }
}
// 可以看到的MergeSort和QuickSort的重大区别
// mergeSort 是先局部再整体 体现在先分别递归sort左右 最后在Merge
// quickSort 是先整体在局部 体现在先partition 最后在分别递归QuickSort左右
// 同时感觉两者的共同点就是 都是 Merge函数和partition函数是关键点使其变得有序
// Two Divide & Conquer sorts
// Merge sort
//   Divide is trivial
//   Merge (i.e, conquer) does all the work
// Quick sort
//   Partition (i.e, Divide) does all the work
//   Merge (i.e, conquer) is trivial

/********************************************************************************************************************************************************
 * QuickSort
 Basic plan.
 ・Shuffle the array. ・Partition so that, for some j
 – entry a[j] is in place
 – no larger entry to the left of j
 – no smaller entry to the right of j
 ・Sort each piece recursively.
 ********************************************************************************************************************************************************/
// 关键 1 返回值是partition后的index
int Partition(int arr[], int start, int end)
{
    // Pivot 相当于always 从start开始
    int pivot = arr[start];
    int i = start + 1;
    int j = end;
    while (true)
    {
        // 1. 从左向右扫描， 比pivot小则保持自增++ 否则跳出
        while (arr[i] <= pivot)
        {
            // 特殊条件当i已经是end了
            if (i == end)
                break;
            i++;
        }

        // 2. 从右向左扫描， 比pivot大的自保持自减-- 否则跳出
        while (arr[j] >= pivot)
        {
            if (j == start)
                break;
            j--;
        }

        // 3. 特别关键！！！ 当i和j交错的时候 退出整个while(true)循环 这是唯一退出的条件！!
        // 这个其实反映了算法的核心思想：一直比较直到整体有序。 i.e. 所有比pivot小的都在左边； 所有比pivot大的都在右边
        // 同时这个要在下面的swap之前出现
        if ( i >= j)
            break;

        //4. 交换 i,j 因为此时 i一定是比基准大而j一定比基准小
        swap(&arr[i], &arr[j]);
    }

    //5. 这个交换不能少！！同时小心是 a) 是用j的因为此时j <= i 已经进入小的区域
    //   b) 不能用Swap(ArrayInput[j], pivot);因为要交换实际数组的值
    swap(&arr[start], &arr[j]);

    //6. 特别注意是return j的值因为此时j已经进入了小的区域
    return j;
}

/****************************************************************************************************************************************************
 * 这个递归比较清晰
 * 
 * **************************************************************************************************************************************************/
int Partition2(int arr[], int start, int end)
{
    int pivot = arr[start];
    int left = start + 1;
    int right = end;
    while (left <= right) {
        if (arr[left] > pivot && arr[right] < pivot)
        {
            swap(arr[left], arr[right]);
            left ++;
            right --;
        }
        if (arr[left] <= pivot) {
            left++;
        }
        if (arr[right] >= pivot) {
            right--;
        }
    }
    
    swap(arr[start], arr[right]);
    return right;
}


void QuickSort2(int arr[], int start, int end)
{
    // 这里等于的情况也要return
    if (start >= end)
        return;
    // 先整体有序再局部有序; 也就是先小的都在左半边大的都在右半边
    int index = Partition2(arr, start, end);
    QuickSort2(arr, start, index);
    QuickSort2(arr, index + 1, end);
}

void QuickSort(int arr[], int start, int end)
{
    // 这里等于的情况也要return
    if (start >= end)
        return;

    // 先整体有序再局部有序; 也就是先小的都在左半边大的都在右半边
    //int index = Partition(arr, start, end);
    int left = start;
    int right = end;

    // 选择基准数很重要， 决定quick sort的时间复杂度 最差O(n^2) 选的好的话是O(nlogn) 平均是O(nlogn)
    // 1. 随机选择
    // 2. 就/2 但是特别注意不能选start或者end： 选start 如果本来已经是排好序的
    int pivot = (arr[start] + arr[end]) / 2;
    while (left <= right)
    {
        while ((left <= right) && (arr[left] < pivot))
        {
            left++;
        }

        // 特别重要 在！！！！ 这里没有等于号
        while ((left <= right) && (arr[right] > pivot))
        {
            right--;
        }
        // 特别重要 ！！！ 等于pivot的情况为了balance 直接交换
        /* Quick Sort 要处理这种情况：[1,1,1,1,1,1...] 如果严格的把1非左即右的划分的话，要么全去左边，要么全去右边，会造成划分的不平衡。
          所以 Quick Sort 在这个上的处理是，让 1 可以比较均匀的分到两边去。所以采用了 > k 和 < k。
          而 partition array 以及其他的一些大部分的 partition 的问题，其目的就是要非左即右的划分成两个部分，所以可以直接 >=k 和 < k
        */
        if (left <= right)
        {
            int temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;

            left ++;
            right--;
        }
    }
    QuickSort(arr, start, right);
    QuickSort(arr, left, end);
}

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    //printf("n");
}

int main(int argc, char* argv[])
{
    int arr[] = { 64, 34, 25, 12, 22, 11, 90, 2, 23, 39, 89, 88, 123, 10};
    
    // @note sizeof(arry) returns array size in bytes, sizeof(arr) / sizeof(arr[0]) returns array length.
    int n = sizeof(arr) / sizeof(arr[0]);
    
    for(int i = 0; i < argc; ++i)
    {
        if (!strcmp("merge", argv[i]))
        {
            int* arrAux = static_cast<int*>(malloc(n*sizeof(int)));
            MergeSortTopDown(arr, arrAux, 0, n - 1);
        }
        else if (!strcmp("insertion", argv[i]))
        {
            InsertionSort(arr, n);
        }
        else if (!strcmp("quick", argv[i]))
        {
            QuickSort2(arr, 0, n-1);
        }
        else
        {
            int* arrAux = static_cast<int*>(malloc(n*sizeof(int)));
            MergeSortTopDown(arr, arrAux, 0, n - 1);
            //QuickSort2(arr, 0, n-1);
        }
    }
    
    //     int* arrAux = static_cast<int*>(malloc(n*sizeof(int)));
    //     MergeSort(arr, arrAux, 0, n - 1);
    printf("Sorted array: \n");
    printArray(arr, n);
    
    string str = "Hello World";
    size_t start = 0;
    size_t pos = 0;
    //while(start < str.size())
    while(str[start] != '\0')
    {
        pos = str.find(" ", start);
        if (pos != string::npos)
        {
            cout << str.substr(start, pos - start) << endl;
            start = pos + 1;
        }
        else
        {
            cout << str.substr(start, str.size() - 2) << endl;
            cout << str.substr(str.size() - 1, str.size() - 2) << endl;
            break;
        }
    }
    
    return 0;
}
