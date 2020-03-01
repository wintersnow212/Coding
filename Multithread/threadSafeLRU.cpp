// 这样不一定对！！！！！

class LRUCache {
public:
    LRUCache(int capacity) {
        cap = capacity;
    }
    
    int get(int key) {
        if (keyToIt.find(key) == keyToIt.end())
            return -1;
        
        unique_lock<mutex> lock(*keyToIt[key]->mu);
        int ret = keyToIt[key]->val;
        
        m_list.splice(m_list.begin(), m_list, keyToIt[key]);
        
        return ret;
    }
    
    void put(int key, int value) 
    {
        if (keyToIt.find(key) != keyToIt.end())
        {
            unique_lock<mutex> lock(*keyToIt[key]->mu);
            keyToIt[key]->val = value;
            m_list.splice(m_list.begin(), m_list, keyToIt[key]);
        }
        else
        {
            if (m_list.size() == cap)
            {
                unique_lock<mutex> lock(*m_list.back().mu);
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
            mu = make_unique<mutex>();
        }
        int key;
        int val;
        unique_ptr<mutex> mu;
    };
    list<Node> m_list;
    unordered_map<int, list<Node>::iterator> keyToIt;
    int cap;
};
