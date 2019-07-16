#include <iostream>
#include <utility>
#include <unordered_map>
#include <list>
using namespace std;
// key to val pair
using Node = pair<int, int>;

class LRUCache
{
public:
    LRUCache (int capacity)
    {
        m_cap = capacity;
    }
    
    int get(int key)
    {
        if (m_map.find(key) != m_map.end())
        {
            int val = m_map[key]->second;
            m_list.splice(m_list.begin(), m_list, m_map[key]);
            
            return val;
        }
        else
        {
            return -1;
            //throw "exception";
        }
    }
    
    
    void put(int key, int val)
    {
        if (m_map.find(key) != m_map.end())
        {
            m_map[key]->second = val;
            m_list.splice(m_list.begin(), m_list, m_map[key]);
        }
        else
        {
            if (m_list.size() == m_cap)
            {
                // pop
                int deleteKey = m_list.back().first;
                m_list.pop_back();
                m_map.erase(deleteKey);
            }
            
            m_list.push_front({key, val});
            m_map[key] = m_list.begin();
        }
    }
    
private:
    int m_cap;
    // key to list iterator map
    unordered_map<int, list<Node>::iterator> m_map;
    // key to val pair
    list<Node> m_list;
    
};

class LFUCache 
{
public:
    LFUCache(int capacity) 
    {
        m_cap = capacity;
        m_minFreq = 0;
    }
    
    int get(int key)
    {
        if (keyToFreqAndVal.find(key) == keyToFreqAndVal.end())
        {
            return -1;
        }
        else
        {
            updateKey(key);
            
            return keyToFreqAndVal[key].second;
        }
    }
    
    void put(int key, int value) 
    {
        // 关键错误2 
        /*
        ["LFUCache","put", "get"]
        [[0],[0,0],[0]]
        */
        if (m_cap <= 0){
            return;
        }
        if (keyToFreqAndVal.find(key) != keyToFreqAndVal.end())
        {
            keyToFreqAndVal[key].second = value;
            updateKey(key);
        }
        else
        {
            if (m_cap == keyToListIt.size())
            {
                // 这里是关键啊 我们要delete element from freqList ！！！
                // 或者直接在freqList[m_minFreq] 上面操作！！！
                list<int>& minFreqList = freqList[m_minFreq];
                int deleteKey = minFreqList.back();
                minFreqList.pop_back();
                
                keyToFreqAndVal.erase(deleteKey);
                keyToListIt.erase(deleteKey);
            }
            
            keyToFreqAndVal[key] = {1, value};
            
            freqList[1].push_front(key);
            keyToListIt[key] = freqList[1].begin();
            m_minFreq = 1;
        }
    }
private:
    
    void updateKey(int key)
    {
        int oldFreq = keyToFreqAndVal[key].first;
        
        // freqList.erase(oldFreq);
        // 错误 这里只应该erase freqList里面的一个元素
        // 要想erase 我们就是要iterator 所以再需要一个map
        // 记录下key to iterator
        freqList[oldFreq].erase(keyToListIt[key]);
        
        // update keyToValPair
        keyToFreqAndVal[key].first++;
        
         // Push the key to new freq list
        int newFreq = keyToFreqAndVal[key].first;
        freqList[newFreq].push_front(key);
        
        // update the keyToListIt map
        keyToListIt[key] = freqList[newFreq].begin();
        
        if (freqList[m_minFreq].empty() == true)
        {
            m_minFreq++;
        }
    }
    
    int m_cap;
    int m_minFreq;
    // 1. key 2. pair： first freq second value
    // 这个用来找值
    unordered_map<int, pair<int, int>> keyToFreqAndVal;
    
    
    // list of the element that has same freq
    // 为了LRU
    // first is frequency, second is 这里list里面放的是key
    unordered_map<int, list<int>> freqList;
    
    // key to list iterator
    // 这个用来delete 如果没有cap满 delete这一步的话都不用list了！！！
    // 一个key 应该只有一个list iterator相对应 但是一个freq可能有多个key/iter
    // 因为其实是以key来区分元素的而不是freq，多个元素可以有相同的freq
    unordered_map<int, list<int>::iterator> keyToListIt;
};

// To execute C++, please define "int main()"
int main() 
{
    LRUCache* cache = new LRUCache(2);
    
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
