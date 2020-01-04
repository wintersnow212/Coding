#include <iostream>
#include <vector>
#include <list>
#include <atomic>
#include <utility>
#include <shared_mutex>
#include <mutex>
#include <thread>
using namespace std;

template<int SIZE>
class MyHashMap {
public:
    /** Initialize your data structure here. */
    MyHashMap() 
    {
        mSize = SIZE;
        vec.resize(SIZE);
        // 这里的resize 只会初始化为nullptr吧
        //mutexs.resize(SIZE);
        // 如果必须要用push_back 或者 emplace back来初始化 此时用reserve更好！！！！
        mutexs.reserve(SIZE);
        for (int i = 0; i < SIZE; ++i)
        {
            mutexs.emplace_back(make_unique<shared_mutex>());
        }
    }
    
    MyHashMap(int size)
        : mSize(size)
    {
        vec.resize(size);
        //mutexs.resize(size);
        mutexs.reserve(SIZE);
        for (int i = 0; i < SIZE; ++i)
        {
            mutexs.emplace_back(make_unique<shared_mutex>());
        }
    }
    
    /** value will always be non-negative. */
    void insert(int key, int value) {
        int hashCode = key % mSize;
        if (mutexs[hashCode] == nullptr)
        {
            cout << "It is nullptr" << endl;
        }
        unique_lock<shared_mutex> wLock(*mutexs[hashCode]);
        //unique_lock<shared_mutex> wLock(mutexsArr[hashCode]);
        auto& list = buckets[hashCode];
        
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
        atomic<int> hashCode = key % mSize;
        if (mutexs[hashCode] == nullptr)
        {
            cout << "It is nullptr" << endl;
        }
        shared_lock<shared_mutex> rLock(*mutexs[hashCode]);
        //shared_lock<shared_mutex> rLock(mutexsArr[hashCode]);
        auto list = buckets[hashCode];
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
        unique_lock<shared_mutex> wLock(*mutexs[hashCode]);
        //unique_lock<shared_mutex> wLock(mutexsArr[hashCode]);
        auto& list = buckets[hashCode];
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
    // 这个vector 要求element是movable的所以我们不能直接存放mutex
    vector<unique_ptr<shared_mutex>> mutexs;
    
    
    // 这样设计的不好在于 我们不能rehashing了？？？？？
    list<pair<int, int>> buckets[SIZE];
    // 每个bucket都有一个read write lock
    shared_mutex mutexsArr[SIZE];
};

// To execute C++, please define "int main()"
int main() {
    vector<shared_mutex*> mutexs;
    mutexs.resize(10);
    MyHashMap<1000> myHashTable;
    vector<int> keys = {1, 2, 12, 3, 4};
    vector<int> vals = {100, 102, 112, 103, 104};
    vector<int> targets = {12, 2, 23, 3, 4};
    auto insert = [&] () {
                    for (int i = 0; i < keys.size(); ++i)
                    {
                        myHashTable.insert(keys[i], vals[i]);
                    }
                    
                  };
    
    auto find = [&targets, &myHashTable]()
                {
                    for (int k : targets)
                    {
                        if (myHashTable.find(k) == -1)
                        {
                            cout << "not found" << endl;
                        }
                        else
                        {
                            cout << "find " << k << " is  " << myHashTable.find(k) << endl;
                        }
                    } 
                };
    
    thread reader(find);
    thread writer(insert);
    thread reader2(find);
    writer.join();
    reader.join();
    reader2.join();
    // myHashTable.insert(1, 100);
    // myHashTable.insert(2, 102);
    // myHashTable.insert(12, 112);
    // myHashTable.insert(3, 103);
    // myHashTable.insert(4, 104);
    // cout << myHashTable.find(12) << endl;
    // myHashTable.erase(12);
    // cout << myHashTable.find(12) << endl;
    // cout << myHashTable.find(3) << endl;
    // myHashTable.insert(3, 123);
    // cout << myHashTable.find(3) << endl;
  
    return 0;
}
