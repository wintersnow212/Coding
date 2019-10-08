#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
using namespace std;

template<typename T>
struct pair_hash
{
    size_t operator()(const pair<T, T>& obj) const
    {
        std::hash<string> hasher;
        return hasher(obj.first) ^ hasher(obj.second);
    }
};

unordered_map<string, pair<string, int>> m1;
unordered_map<pair<string, string>, pair<int, int>, pair_hash<string>> m2;

void test(string cardId, string stId, int time)
{
    if (m1.find(cardId) == m1.end())
    {
        m1[cardId] = {stId, time};
    }
    else
    {
        string stId1 = m1[cardId].first;
        m2[{stId1, stId}] = {m2[{stId1, stId}].first + time - m1[cardId].second, m2[{stId1, stId}].second + 1};
        m1.erase(cardId);
    }
    
}
// To execute C++, please define "int main()"
int main() {
    string stId   = "abc";
    string stId2  = "efg";
    string stId3   = "ab";
    string stId4  = "ef";
    string cardId = "Tianyu";
    string cardId2 = "Yingzi";
    pair<int, int> p = {1, 2};
    
    test(cardId, stId, 2);
    test(cardId, stId2, 5);
    
    test(cardId, stId, 2);
    test(cardId, stId2, 6);
    
    test(cardId2, stId3, 2);
    test(cardId2, stId4, 5);
    
    test(cardId2, stId3, 4);
    test(cardId2, stId4, 6);
    
    
    for (auto m : m2)
    {
        double avg = double(m.second.first) / m.second.second;
        cout << m.first.first + "-" + m.first.second << " " << avg << endl;
    }
    
    auto h1 = std::hash<int>()(p.first);
    auto h2 = std::hash<int>()(p.second);
    int h = h1 ^ h2;
    //cout << h << endl;
    return 0;
}
