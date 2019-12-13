#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& nums, int start, int end)
{
    /* 
         优化版本 
         rand pivot
         int idx = start + rand() % (end - start + 1) ;
       int pivot = nums[idx];
       // 巧妙的地方在于我们直接swap 就按照原来的方法 一样的模板做了
       // 因为我们想最后return的就是pivot所指向的位置 同时我们希望start在pivot的右边这样比较省心
       swap(nums[idx], nums[start]);
       int left = start + 1;
    */
    int pivot = nums[start];
    int left = start + 1;
    int right = end;
    while (left <= right) {
        while (left <= right && nums[left] < pivot)
        {
            left++;
        }

        while (right >= left && nums[right] > pivot)
        {
            right--;
        }

        if (left <= right)
        {
            swap(nums[left], nums[right]);
            left++;
            right--;
        }
    }
    
    swap(nums[start], nums[right]);
    return right;
}

void quickSort2(vector<int>& nums, int start, int end)
{
    // 关键1 这里等于的情况也要return!!!!!
    // 貌似取决于 你下面是idx - 1 and idx + 1---- 这样> 和 >= 都可以
    // 还是 idx 和idx + 1 只能>=
      // 其实原因很简单 因为我们不能死循环
      // 不过下面的merge sort却不是 貌似只能 idx和idx+1
      // 可以这样理解 quick sort和quick select都是partition 系列
    if (start >= end)
    {
        return;
    }
    // 先整体有序再局部有序; 也就是先小的都在左半边大的都在右半边
    int index = partition(nums, start, end);
    
    // 关键2 这里index的选择！！！
    //        /* 这个就会overflow!!!
    //        QuickSort2(arr, start, index - 1);
    //     QuickSort2(arr, index, end);
    //        */
    quickSort2(nums, start, index);
    quickSort2(nums, index + 1, end);
}


int main()
{
    vector<int> b = {1,4,5,9,32,4,123,43,5,234,2,7,8,3,8};
    int end = b.size() - 1;
    quickSort2(b, 0, end);
    for (int n : b)
    {
        cout << n << endl;
    }
}
