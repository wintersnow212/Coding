#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// Implement a space efficient integer vector
// Common Case:
// 1. There are 3 or fewer values
// 2. All values are between 1 and 100
// Implementation must always be correct
// Only care about space efficiency in the common case
// Only implements push_back and operator[]
// You may use std::vector

// 1, 2 (Common)
// 1, 1000 (Not Common - Breaks 2)
// 1, 2, 3, 4 (Not Common - Breaks 1)
// 1, 2, 3, 1000 (Not Common - Breaks 1 and 2)

class myVector
{
public:
      myVector()
          :valIdx(0)
          ,innerIdx(0)
          ,outerIdx(0)
          ,smallValue(0)
      {
        
      }

      void push_back(const int& val)
      {
          if (val >= 1 && val <= 100)
          {
             if (innerIdx == 0)
             {
                int num = 0;
                num |= (val + '0') << (8 * innerIdx);
                vec.push_back(num);
                outerIdx = vec.size() - 1;
             }
             else
             {
               vec[outerIdx] |= (val + '0') << (8 * innerIdx);
             }
             idxMap[valIdx] = {outerIdx, innerIdx};
             innerIdx = (innerIdx + 1) % 4;
          }
          else
          {
            vec.push_back(val);
            idxMap[valIdx] = {vec.size() - 1, -1};
          }
          
          valIdx++;
      }
      
      int& operator[](int outsideIdx)
      {
          int outer = idxMap[outsideIdx].first;
          int inner = idxMap[outsideIdx].second;
          if (inner == -1)
          {
              return vec[outer];
          }
          else
          {
              int mask = 0xff << (8 * inner);
              char val = (vec[outer] & mask) >> (8 * inner);
              smallValue = val - '0';
              // create 一个member vaiable 就可以return by reference
              return smallValue;
          }
      }
    
      int size()
      {
          return valIdx;
      }
private:
    vector<int> vec;
    // outer and inner pair
    unordered_map<int, pair<int, int>> idxMap;
    int valIdx;
    int innerIdx;
    int outerIdx;
    int smallValue;
};


int main() {
    //cout<<"Hello";
    myVector mVec;
    mVec.push_back(1);
    mVec.push_back(64);
    mVec.push_back(1000);
    mVec.push_back(36);
     mVec.push_back(235);
    mVec.push_back(23);
    mVec.push_back(10);
    for (int i = 0; i < mVec.size(); ++i)
    {
        cout << mVec[i] << endl;
    }
    
    return 0;
}
