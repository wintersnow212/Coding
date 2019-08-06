#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>
using namespace std;

template <typename T1, typename T2>
class pair_hash 
{   
public:
    //std::size_t operator () (const std::pair<T1,T2> &p) const 
    std::size_t operator () (const pair<T1, T2> &p) const 
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};


template <typename Key>
struct myHash
{
    std::size_t operator()(const Key& k) const
    {
      using std::hash;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      // return ((hash<string>()(k.first)
      //          ^ (hash<string>()(k.second) << 1)) >> 1)
      //          ^ (hash<int>()(k.third) << 1);
        
        return hash<Key>()(k);
    }
};
// template <typename T1>
// class pair_hash2 
// {   
// public:
//     //std::size_t operator () (const std::pair<T1,T2> &p) const 
//     std::size_t operator () (const T1 &p) const 
//     {
//         auto h1 = std::hash<T1>{}(p.first);

//         // Mainly for demonstration purposes, i.e. works but is overly simple
//         // In the real world, use sth. like boost.hash_combine
//         return h1;  
//     }
// };


template<typename T>using Node = pair<T, T>;
template <typename T>
class LRUCache {
public:
    LRUCache(int capacity) {
        m_cap = capacity;
    }
    
    T get(T key) {
        if (map.find(key) == map.end())
        {
            return -1;
        }
        else
        {
            T ret = map[key]->second;
            m_list.splice(m_list.begin(), m_list, map[key]);
            return ret;
        }
    }
    
    void put(T key, T value) {
          // 关键错误1 在put的时候也要判断 如果已经有的话不用push
        if (map.find(key) != map.end())
        {
            map[key]->second = value;
            m_list.splice(m_list.begin(), m_list, map[key]);
        }
        else
        {
            if (m_list.size() == m_cap)
            {
                // 关键错误3 这里是关键错误！！！ 应该用back()而不是end()
                T delKey = m_list.back().first;
                map.erase(delKey);
                m_list.pop_back();
            }
            
            m_list.push_front({key, value});
            map[key] = m_list.begin();
        } 
    }
private:
    int m_cap;
    // list<pair<T, T>> m_list;
    // // 关键是typename
    // /* You need to write typename before std::list<...>::iterator, because iterator 
    //    is a nested type and you're writing a template.
    // */
    // //unordered_map<T, typename list<pair<T, T>>::iterator, myHash<T>> map;
    // unordered_map<T, typename list<pair<T, T>>::iterator> map;
    
    //  优化写法
    //typedef list<Node<T>> ListType;
    typedef list<pair<T, T>> ListType;
    typedef typename ListType::iterator IterType;
    ListType m_list;
    unordered_map<T, IterType> map;
};


// To execute C++, please define "int main()"
int main() {
  
    unordered_map<pair<string, string>, int, pair_hash<string, string>>
        distMap = 
    //std::unordered_map<pair,int,pair_hash> unordered_map =
        {
     {{"C++", "C++11"}, 2011},
     {{"C++", "C++14"}, 2014},
     {{"C++", "C++17"}, 2017},
     {{"Java", "Java 7"}, 2011},
     {{"Java", "Java 8"}, 2014},
     {{"Java", "Java 9"}, 2017}};

    for (auto const &entry: distMap)
    {
         auto key_pair = entry.first;
         std::cout << "{" << key_pair.first << "," << 
             key_pair.second << "}, " << entry.second << '\n';
    }

    LRUCache<int>* cache = new LRUCache<int> (2);
    cache->put(1, 1);
    cache->put(2, 2);
    cout << cache->get(1) << " ";       // returns 1
    cache->put(3, 3);    // evicts key 2
    cout << cache->get(2) << " ";       // returns -1 (not found)
    cache->put(4, 4);    // evicts key 1
    cout << cache->get(1) << " ";       // returns -1 (not found)
    cout << cache->get(3) << " ";       // returns 3
    cout << cache->get(4) << " ";      // returns 4
    return 0;
}
