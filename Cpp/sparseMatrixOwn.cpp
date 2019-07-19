#include <iostream>
#include <utility>
#include <unordered_map>
#include <unordered_set>
using namespace std;

using Node = pair<int, int>;


// struct hash<Key>
//   {
//     std::size_t operator()(const Key& k) const
//     {
//       using std::size_t;
//       using std::hash;
//       using std::string;

//       // Compute individual hash values for first,
//       // second and third and combine them using XOR
//       // and bit shifting:

//       return ((hash<string>()(k.first)
//                ^ (hash<string>()(k.second) << 1)) >> 1)
//                ^ (hash<int>()(k.third) << 1);
//     }
//   };

struct pair_hash
{
    // Const member functuion
    std::size_t operator()(const Node& key) const
    {
        return (hash<int>()(key.first) ^ 
                hash<int>()(key.second));
    }
};

class SPMatrix
{
public:
    SPMatrix(int r, int c)
        : m_row(r)
        , m_col(c)
    {
        
    }
    
    ~SPMatrix()
    {
        
    }
    
    void Set(int r, int c, int val)
    {
        if (r < 0 || r >= m_row || c < 0 || c >= m_col)
        {
            throw "invalid input";
        }
        
        Node n = {r, c};
        
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
    
    
    int Get(int r, int c) const
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
            int r = m.first.first;
            int c = m.first.second;
            int val = this->Get(r, c) + 
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
    int m_row;
    int m_col;
    unordered_map<Node, int, pair_hash> map;
};


// To execute C++, please define "int main()"
int main() {
    SPMatrix sp(2, 2);
    sp.Set(0, 0, 1);
    sp.Set(0, 0, 0);
    sp.Set(1, 1, 2);
    
    SPMatrix sp2(2, 2);
    sp2.Set(0, 0, 1);
    sp2.Set(1, 1, 2);
    sp2.Set(0, 1, 2);
    
    SPMatrix ret = sp + sp2;
    
    cout << ret.Get(0, 0) << endl;
    cout << ret.Get(1, 1) << endl;
    cout << ret.Get(0, 1) << endl;
    return 0;
}
