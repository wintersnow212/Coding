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
    return 0;
}
