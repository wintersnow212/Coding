#include <iostream>
#include <vector>
using namespace std;

class ZigzagIterator {
public:
    // 关键错误！！！！！！！ 这里一定要是pass by reference
    // 不要你存的就是temporary object!!!!!!
    ZigzagIterator(vector<vector<int>>& v)
        : cnt(-1)
        , vSize(v.size())
    {
        for (auto it = v.begin(); it != v.end(); ++it)
        {
            vItVec.push_back(it->begin());
            vItEndVec.push_back(it->end());
        }
            
        // for (int i = 0; i < v.size(); ++i)
        // {
        //     vItVec.push_back((v.begin() + i)->begin());
        //     vItEndVec.push_back((v.begin() + i)->end());
        // }
    }

    int next() 
    {
        //cout << *vItVec[cnt] << endl;
        int ret  = *vItVec[cnt];
        vItVec[cnt]++;
        return ret;
    }

    bool hasNext() {
        cnt = (cnt + 1) % vSize;
        int start = cnt;
        while (vItVec[cnt] == vItEndVec[cnt])
        {
            cnt = (cnt + 1) % vSize;
            if (cnt == start)
            {
                return false;
            }
        }
        
        return true;
    }
private:
    vector<vector<int>::iterator> vItVec;
    vector<vector<int>::iterator> vItEndVec;
    int cnt;
    int vSize;
};
// To execute C++, please define "int main()"
int main() {
    
    vector<int> v1 = {1, 2, 3, 4, 5, 6};
    vector<int> v2 = {7, 8, 9, 10};
    vector<int> v3 = {11,12,13};
    vector<vector<int>> v = {v1, v2, v3}; 
    
    ZigzagIterator ZI (v);
    
    while (ZI.hasNext() == true)
    {
        cout << ZI.next() << " ";
    }
    
    cout << endl;
    
    
    return 0;

}
