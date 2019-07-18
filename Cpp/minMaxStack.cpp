#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
using namespace std;

class MaxStack {
public:
    /** initialize your data structure here. */
    MaxStack() {
        
    }
    
    void push(int x) {
        m_list.push_back(x);
        // 1. 错误 因为iterator of map is bidirectional iterator
        //m_map[x].push_back(l.end() - 1);
        
        // 2. 错误 其实仔细思考rbegin() 是另外一种叫做reverse的iterator
        //    不简单
        //m_map[x].push_back(m_list.rbegin());
        //m_map[x].push_back(--m_list.end());
        m_map[x].push_back(prev(m_list.end()));
    }
    
    int pop() {
        int ret = m_list.back();
        m_list.pop_back();
        
        m_map[ret].pop_back();
        if (m_map[ret].size() == 0)
        {
            m_map.erase(ret);
        }
        return ret;
    }
    
    int top() {
        return m_list.back();
    }
    
    int peekMax() {
        
        // 大SB 啊 associated containter 不能back()!!! It need iterator
        //  Sequence containers 可以！！！
        // auto it = m_map.back()
        // auto it = prev(m_map.end());
        // return it->first;
        
        // 或者 rbegin()
        return m_map.rbegin()->first;
    }
    
    int popMax() {
        //int ret = m_map.back().first;
        int ret = m_map.rbegin()->first;
        //int ret = prev(m_map.end())->first;
        
        m_list.erase(prev(m_map.end())->second.back());
        m_map[ret].pop_back();
        
        if (m_map[ret].size() == 0)
        {
            m_map.erase(ret);
        }
        return ret;
    }
private:
    list<int> m_list;
    //map<int, list<int>::iterator> m_map;
    map<int, vector<list<int>::iterator>> m_map;
};

// To execute C++, please define "int main()"
int main() {

    MaxStack stack;
    stack.push(5); 
    stack.push(1);
    stack.push(5);
    stack.top();
    stack.popMax();
    stack.top();
    stack.peekMax();
    stack.pop();
    stack.top(); 
    return 0;
}
