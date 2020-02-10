#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

//简单解释一下为什么malloc/free是效率杀手。以简化的tcmalloc为例，一次malloc至少需要访问一次thread cache（无锁），
//然后有可能访问central cache（有锁）。
//增加的指令、跳转和cache miss已经不少，然而，central cache的锁可能造成的contention根本就是几个数量级的慢（ns vs. ms）。
//更要命的是，生产系统里，小的string特别多，大大增加了central cache的压力和contention的几率，影响所有线程的效率（都是us甚至ms级别的）。
//小测试无法捕捉到这种问题
/*
Developers noticed that and now effectively all std::string implementations implement the so called “small string optimization”. 
Each string object contains a small inline buffer that will hold the data, if it fits. It’ll allocate only if the data can’t fit that buffer.
As most strings in practice are small – they’ll save an allocation and improve cache locality very often.
*/

// https://github.com/facebook/folly/blob/master/folly/docs/FBVector.md
// Facebook small vector optimization !!!!!!!!

// Implement a space efficient integer vector
// Common Case:
// 这个题目的意思其实是没有大数和小数的交叉前8个必须都是小数 如果是大数的话 
// 那就不用optimization
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
template<int N>
class myVector
{
public:
      myVector()
          :valIdx(0)
          ,innerIdx(0)
          ,smallValue(0)
      {
            
      }

      void push_back(const int& val)
      {
          if (val >= 1 && val <= 100 && vec.size() == 0)
          {
              smallBuffer[innerIdx++] = val;
          }
          else
          {
              vec.push_back(val);
          }
          
          valIdx++;
      }
      
      int& operator[](int idx)
      {
          if (idx < innerIdx)
          {
              smallValue = smallBuffer[idx];
              return smallValue;
          }
          else
          {
              return vec[idx - innerIdx];
          }
      }
    
      int size()
      {
          return valIdx;
      }
private:
    char smallBuffer[sizeof(int) * N];
    vector<int> vec;
    int valIdx;
    int innerIdx;
    int smallValue;
};


int main() {
    //cout<<"Hello";
    myVector<2> mVec;
    mVec.push_back(1);
    mVec.push_back(64);
    mVec.push_back(36);
    mVec.push_back(64);
    mVec.push_back(36);
    mVec.push_back(64);
    mVec.push_back(1000);
    mVec.push_back(64);
    mVec.push_back(36);
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
