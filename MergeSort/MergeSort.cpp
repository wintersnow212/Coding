#include <iostream>
#include <vector>
using namespace std;


// 最大的劣势 extra space o(n) aux array来腾挪, 同时额外空间也需要时间
// 以mid为中界的两边合并 这个是整个merge sort的关键！！！！
void merge(vector<int>& nums, vector<int>& aux, int start, int mid, int end)
{
    for (int i = start; i <= end; ++i)
    {
        aux[i] = nums[i];
    }

    int left = start;
    int right = mid + 1;
    for (int i = start; i <= end; ++i)
    {
        if (left > mid)
        {
            nums[i] = aux[right++];
        }
        else if (right > end)
        {
            nums[i] = aux[left++];
        }
        else if (aux[left] < aux[right])
        {
            nums[i] = aux[left++];
        }
        else
        {
            nums[i] = aux[right++];
        }
        
    }

}

// 先局部有序再整体有序
void mergeSort(vector<int>& nums, vector<int>& aux, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int mid = (start + (end - start)/2);
    // 这里是和quick sort的一个关键不同 这里是mid和mid+1
    // 而quick sort是 partIdx - 1 和 partIdx + 1
    mergeSort(nums, aux, start, mid);
    mergeSort(nums, aux, mid + 1, end);
    merge(nums, aux, start, mid, end);
}

int main()
{
    vector<int> test = {12,3,4,7,12,8,56,32,23,6,8,99,12,34,23,45,68};
    vector<int> aux(test.size());
    mergeSort(test, aux, 0, test.size() - 1);
    // for (auto t : test)
    // {
    //     cout << t << endl;
    // }
    return 0;
}
