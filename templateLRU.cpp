#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>
using namespace std;

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
    /*
    在模板中，如何声明嵌套从属类型(即模板嵌套类型)？
    根据C++的规则，编译器先假设C::iterator不是一个类型。然而iter的声明只有在C::iterator是一个类型时才合理。因此需要我们自己告诉编译器。
    那么，就需要再C::iterator之前加上typename，告诉编译器C::iterator是一个类型。
    template<typename C>
    void doSomething(const C& container)
    {
        if(container.size() > 0)
            typename C::iterator iter(container.begin());
    }
    
    */
    typedef typename ListType::iterator IterType;
    ListType m_list;
    unordered_map<T, IterType> map;
};


// To execute C++, please define "int main()"
int main() 
{
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
