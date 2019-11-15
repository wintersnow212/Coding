#include <iostream>
#include <utility>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// 其实template 就是申明的时候前面加一个template<typename T> 就像template class 和template function一样
// 使用的时候正常的type后面再加一个<T> ---- 比如 Node<T> 因为这个就是它的完整type了！！！
template<typename T>using Node = pair<T, T>;

template<typename T>
struct pair_hash
{
    // Const member functuion why it must be const????
    std::size_t operator()(const Node<T>& key) const
    {
        return (hash<T>()(key.first) ^ hash<T>()(key.second));
    }
};

template<typename T, typename V>
class SPMatrix
{
public:
    SPMatrix(T r, T c)
        : m_row(r)
        , m_col(c)
    {
        
    }
    
    ~SPMatrix()
    {
        
    }
    
    void Set(T r, T c, V val)
    {
        if (r < 0 || r >= m_row || c < 0 || c >= m_col)
        {
            throw "invalid input";
        }
        
        Node<T> n = {r, c};
        
        if (map.find(n) != map.end())
        {
            if (val == 0)
            {
                map.erase(n);
            }
            else
            {
                map[n] = val;
            }
           
        }
        else
        {
            if (val != 0)
            {    
                map.insert({{r, c}, val});
            }
        }
    
    }
    
    
    V Get(T r, T c) const
    {
        if (r < 0 || r >= m_row || c < 0 || c >= m_col)
        {
            throw "invalid input";
        }
        
        if (map.find({r, c}) == map.end())
        {
            return 0;
        }
        
        return map.find({r, c})->second;
    }
    
    // Copy constructor
    SPMatrix (const SPMatrix& other)
    {
        m_row = other.m_row;
        m_col = other.m_col;
        map   = other.map;
    }
    
    SPMatrix& operator= (const SPMatrix& other)
    {
        m_row = other.m_row;
        m_col = other.m_col;
        map   = other.map;
        
        return *this;
    }

    SPMatrix Add(const SPMatrix& other) const
    {
        
//         SPMatrix ret(m_row, m_col);
        
//         // best method is to use copy constructor
//         unordered_set<Node, pair_hash> ownSet;
//         for (auto m : other.map)
//         {
//             if (this->map.find(m.first) != this->map.end())
//             {
//                 int r = m.first.first;
//                 int c = m.first.second;
//                 int val = this->Get(r, c) + 
//                           other.Get(r, c);
                
//                 ret.Set(r, c, val);
                
//                 other.map.erase({r, c});
                
//             }
//         }
        
        // Best method is to use copy constructor
        SPMatrix ret(other);
        
        for (auto m : this->map)
        {
            T r = m.first.first;
            T c = m.first.second;
            V val = this->Get(r, c) + 
                      ret.Get(r, c);
            ret.Set(r, c, val);
        }
        
        return ret;
    }
    
    SPMatrix Multiply (const SPMatrix& other) const
    {
        SPMatrix ret(m_row, other.m_col);
        
        // 其实思想很简单就是 
        // 1. A的row B的col 要相同
        // 2. 最后的结果就是A的row 和B的col组成
        for (const auto& m1 : map)
        {
            T r1 = m1.first.first;
            T c1 = m1.first.second;
            for (const auto& m2 : other.map)
            {
                T r2 = m2.first.first;
                T c2 = m2.first.second;
                if (c1 == r2)
                {
                    V val = this->Get(r1, c1) *
                            other.Get(r2, c2);
                    
                    V sum = ret.Get(r1, c2) + val;
                    ret.Set(r1, c2, sum);
                }
            }
        }
        
        return ret;
    }
    
    SPMatrix operator+ (const SPMatrix& other) const
    {
        if (other.m_row != m_row || other.m_col != m_col)
        {
            throw "invalid dimension";
        }
        
        return this->Add(other);
    }
    
    SPMatrix operator* (const SPMatrix& other) const
    {
        if (m_row != other.m_col)
        {
            throw "invalid dimension";
        }
        
        return Multiply(other);
    }
private:
    T m_row;
    T m_col;
    // 这个写在里面比较方便 对比定义在外面的
    using innerNode = pair<T, T>;
    //unordered_map<Node<T>, V, pair_hash<T>> map;
    
    // 进一步优化简洁代码
    typedef unordered_map<innerNode, V, pair_hash<T>> mapType;
    mapType map;
    //unordered_map<innerNode, V, pair_hash<T>> map;
};


// To execute C++, please define "int main()"
int main() {
    SPMatrix<int, float> sp(2, 2);
    sp.Set(0, 0, 1.5);
    sp.Set(0, 0, 0.5);
    sp.Set(1, 1, 2.5);
    
    SPMatrix<int, float> sp2(2, 2);
    sp2.Set(0, 0, 1.3);
    sp2.Set(1, 1, 2.3);
    sp2.Set(0, 1, 2.3);
    
    SPMatrix<int, float> ret = sp + sp2;
    
    cout << ret.Get(0, 0) << endl;
    cout << ret.Get(1, 1) << endl;
    cout << ret.Get(0, 1) << endl;
    
    
    SPMatrix<int, int> spM(2, 2);
    spM.Set(0, 0, 2);
    spM.Set(0, 1, 1);
    spM.Set(1, 1, 3);
    
    SPMatrix<int, int> spM2(2, 2);
    spM2.Set(0, 0, 1);
    spM2.Set(0, 1, 3);
    spM2.Set(1, 1, 2);

    
    SPMatrix<int, int> retM = spM * spM2;
    
    cout << retM.Get(0, 0) << endl;
    cout << retM.Get(0, 1) << endl;
    cout << retM.Get(1, 1) << endl;
    
    return 0;
}
/* 
Your previous Python 2 content is preserved below:

# This is a sandbox to experiment with CoderPad's execution capabilities.
# It's a temporary, throw-away session only visible to you.

def say_hello():
    print 'Hello, World'

for i in xrange(5):
    say_hello()

 */



#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <queue>
using namespace std;
