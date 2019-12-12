#include <iostream>
#include <vector>
#include <exception>
#include <unordered_set>
using namespace std;

class RandomGen
{
public:
    RandomGen(int minVal, int maxVal)
        : m_min(minVal)
        , m_max(maxVal)
    {
        initVec(m_min, m_max);
        srand(time(nullptr));
    }
    
    
    void updateRange(int minVal, int maxVal)
    {
        m_min = minVal;
        m_max = maxVal;
        for (int i = m_min; i <= m_max; ++i)
        {
            if (used.find(i) == used.end())
            {
                m_vec.push_back(i);
            }
        }
    }
    
    void reset()
    {
        initVec(m_min, m_max);
        used.clear();
    }
    
    int getRandandom()
    {
        if (m_vec.size() == 0)
        {
            //return -1;
            throw "access out of bound";
            //reset();
        }
        
        int rIdx = rand() % m_vec.size();
        
        int ret = m_vec[rIdx];
        swap(m_vec[rIdx], m_vec.back());
        
        m_vec.pop_back();
        used.insert(ret);
        return ret;
    }
private:
    void initVec(int minVal, int maxVal)
    {
        for (int i = minVal; i <= maxVal; ++i)
        {
            m_vec.push_back(i);
        }
    }    
    int m_min;
    int m_max;
    vector<int> m_vec;
    unordered_set<int> used;
};


// To execute C++, please define "int main()"
int main() {
    int minVal = 3;
    int maxVal = 5;
    RandomGen rd(minVal, maxVal);
    
    for (int i = minVal; i <= maxVal; ++i)
    {
        try
        {
            cout << rd.getRandandom() << endl;
        } 
        catch (const char* msg)
        {
            cout << msg << endl;    
        }
    }
    minVal = 1;
    maxVal = 8;
    rd.updateRange(minVal, maxVal);
    
    for (int i = minVal; i <= maxVal; ++i)
    {
        try
        {
            cout << rd.getRandandom() << endl;
        } 
        catch (const char* msg)
        {
            cout << msg << endl;    
        }
    }
    return 0;
}
