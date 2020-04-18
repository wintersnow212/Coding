// 这样不一定对！！！！！ 感觉还可以？？？ 用shared_mutex 
// 也就是read write lock
#include <iostream>
#include <vector>
#include <list>
#include <atomic>
#include <utility>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <unordered_map>
using namespace std;
class LRUCache {
public:
    LRUCache(int capacity) {
        cap = capacity;
    }
    
    int get(int key) {
        if (keyToIt.find(key) == keyToIt.end())
            return -1;
        
        unique_lock<shared_mutex> lock(*keyToIt[key]->sharedMu);
        int ret = keyToIt[key]->val;
        
        m_list.splice(m_list.begin(), m_list, keyToIt[key]);
        
        return ret;
    }
    
    void put(int key, int value) 
    {
        if (keyToIt.find(key) != keyToIt.end())
        {
            unique_lock<shared_mutex> lock(*keyToIt[key]->sharedMu);
            keyToIt[key]->val = value;
            m_list.splice(m_list.begin(), m_list, keyToIt[key]);
        }
        else
        {
            if (m_list.size() == cap)
            {
                unique_lock<shared_mutex> lock(*keyToIt[key]->sharedMu);
                int delKey = m_list.back().key;
                keyToIt.erase(delKey);
                m_list.pop_back();
            }
            
            m_list.push_front({key, value});
            keyToIt[key] = m_list.begin();
            
        }
    }
private:
    // Key to value
    //typedef pair<int, int> Node;
    struct Node
    {
        Node(int k = 0, int v = 0)
        {
            key = k;
            val = v;
            sharedMu = make_unique<shared_mutex>();
        }
        int key;
        int val;
        // 如果要储存的话这里不用储存mutex object 我们可以存储pointer to object
        unique_ptr<shared_mutex> sharedMu;
    };
    list<Node> m_list;
    unordered_map<int, list<Node>::iterator> keyToIt;
    int cap;
};


int main()
{
    return 0;
}
