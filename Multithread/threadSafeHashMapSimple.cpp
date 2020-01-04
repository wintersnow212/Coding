#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <shared_mutex>
#include <mutex>
using namespace std;

template<int SIZE>
class MyHashMap {
public:
    /** Initialize your data structure here. */
    MyHashMap() 
    {
        mSize = SIZE;
        vec.resize(SIZE);
        mutexs.resize(SIZE);
    }
    
    MyHashMap(int size)
        : mSize(size)
    {
        vec.resize(size);
        mutexs.resize(size);
    }
    
    /** value will always be non-negative. */
    void insert(int key, int value) {
        int hashCode = key % mSize;
        //unique_lock<shared_mutex> wLock(*mutexs[hashCode]);
        unique_lock<shared_mutex> wLock(mutexsArr[hashCode]);
        auto& list = vec[hashCode];
        
        for (auto& l : list)
        {
            if (l.first == key)
            {
                l.second = value;
                return;
            }
        }
        
        list.push_back({key, value});
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int find(int key) 
    {
        int hashCode = key % mSize;
        //shared_lock<shared_mutex> rLock(*mutexs[hashCode]);
        shared_lock<shared_mutex> rLock(mutexsArr[hashCode]);
        auto list = vec[hashCode];
        for (auto& l : list)
        {
            if (l.first == key)
                return l.second;
        }
        
        return -1;
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void erase(int key) {
        int hashCode = key % mSize;
        auto& list = vec[hashCode];
        
        // 根本就不用这么复杂的 连function arg type pair<int, int>都不要！！！
        // remove_if(bucket[hashIdx].begin(), bucket[hashIdx].end(), [key](pair<int, int> node){return node.first == key;});
        //list.remove_if([key](pair<int, int> l) { return l.first == key; });
        list.remove_if([key](auto l) { return l.first == key; });
    }
private:
    int mSize;
    // 注意这里是key 和 value的 pair !!!!
    vector<list<pair<int, int>>> vec;
    //vector<shared_mutex> mutexs;
    vector<unique_ptr<shared_mutex>> mutexs;
    shared_mutex mutexsArr[SIZE];
    shared_mutex mu;
};

// To execute C++, please define "int main()"
int main() {
  
    MyHashMap<1000> myHashTable;
    
    myHashTable.insert(1, 100);
    myHashTable.insert(2, 102);
    myHashTable.insert(12, 112);
    myHashTable.insert(3, 103);
    myHashTable.insert(4, 104);
    
    cout << myHashTable.find(12) << endl;
    myHashTable.erase(12);
    cout << myHashTable.find(12) << endl;
    cout << myHashTable.find(3) << endl;
    myHashTable.insert(3, 123);
    cout << myHashTable.find(3) << endl;
    
    return 0;
}
