#include <iostream>
#include <utility>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// 其实template 就是申明的时候用template<typename T>
// 使用的时候用type + <T> ---- 比如 Node<T>
template<typename T>using Node = pair<T, T>;

template<typename T>
struct pair_hash
{
    // Const member functuion why it must be const????
    // std::size_t operator()(const Node& key) const
    // {
    //     return (hash<int>()(key.first) ^ 
    //             hash<int>()(key.second));
    // }
    
    std::size_t operator()(const Node<T>& key) const
    {
        return (hash<T>()(key.first) ^ 
                hash<T>()(key.second));
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
    
    SPMatrix operator+ (const SPMatrix& other) const
    {
        if (other.m_row != m_row || other.m_col != m_col)
        {
            throw "invalid dimension";
        }
        
        return this->Add(other);
    }
private:
    T m_row;
    T m_col;
    unordered_map<Node<T>, V, pair_hash<T>> map;
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
    return 0;
}
