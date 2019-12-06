#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

class SparseVec
{
public:
    SparseVec()
    {
        
    }
    
    void setValue(int idx, int val)
    {
        vecMap[idx] = val;
    }
    
    int getElement(int idx)
    {
        if (vecMap.find(idx) == vecMap.end())
        {
            return -1;
        }
        
        return vecMap[idx];
    }
    
    // Dot product最后输出的是single number
    int dotProduct(const SparseVec& other)
    {
        int ret = 0;
        for (auto v : other.vecMap)
        {   
            if (this->vecMap.find(v.first) != this->vecMap.end())
            {
                ret += vecMap[v.first] * v.second;
            }
        }
        
        return ret;
    }
    
private:
    // index to value
    unordered_map<int, int> vecMap;
};


int main() {

  // Dot product of two sparse vectors stored in hash tables
    unordered_map<int, double> asht({{0, 1.}, {7, 2.}, {8, 3.}});
    unordered_map<int, double> bsht({{7, 4.}, {8, 9.}});

    SparseVec sp1;
    for (auto a : asht)
    {
        sp1.setValue(a.first, a.second);
    }
    
    cout << sp1.getElement(4) << " " << sp1.getElement(7) << endl;
    SparseVec sp2;
    for (auto b : bsht)
    {
        sp2.setValue(b.first, b.second);
    }
    
    cout << sp1.dotProduct(sp2) << endl;
    
    return 0;
}
