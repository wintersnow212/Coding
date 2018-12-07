#include <iostream>
#include <vector>
using namespace std;
typedef struct sumIndexPair{
    int prefixsum;
    int index;
    
    sumIndexPair() {
        prefixsum = 0;
        index = 0;
    }
    sumIndexPair(int s, int i) {
        prefixsum = s;
        index = i;
    }
    
    // sort with custom comparator 不是说在这customized 而是单独create一个customized的comparator
    // compile错误error: 'bool sumIndexPair::operator<(sumIndexPair, sumIndexPair)' must take exactly one argument
    // 原因 As a member operator overload it should only take one argument, the other being this.
    // bool operator < (struct sumIndexPair a, struct sumIndexPair b)
    //     return (a.prefixsum < b.prefixsum);
}siPair;

struct myComparator
{
    bool operator() (siPair* a, siPair* b)
    {
        return (a->prefixsum < b->prefixsum);
    }
};


int main()
{
    cout << "Hello World" << endl; 
    vector<int> inputArr = {-3, 1, 1, -3, 5};
    vector<siPair*> pairs(len+1);
    // 大错特错 要for loop 每个iteration new allocate
    //vector<siPair*> pairs(len+1, new pair(0, 0));
    //vector<siPair> pairs(len+1,  new siPair(0, 0));
    int sum = 0;
    int len = inputArr.size();
    pairs[0] = new siPair(0, 0);
    // 这里是 <= !!!!
    for (int i = 1; i <= len; ++i) {
        sum += nums[i-1];
        pairs[i] = new siPair (sum, i);
    }

    sort(pairs.begin(), pairs.end(), myComparator());
   return 0;
}