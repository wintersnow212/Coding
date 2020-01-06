#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include <climits>
#include <set>
using namespace std;


struct OrderCombine
{
    int price;
    int time;
    int id;
    bool operator < (const OrderCombine& other) const
    {
        if (price < other.price)
        {
            return true;
        }
        
        return time < other.time;
    }
    
};

struct OrderType
{
    OrderType()
    {
        
    }
    
    OrderType(int p, int t, int i)
        : price(p)
        , time(t)
        , id(i)
    {
            
    }
    int price;
    int time;
    int id;
};
int main()
{
    /* limit order book
        加入order的时候price 小的先 price一样的话 时间小的先
        然后cancel order 是按照id
    */
    // first is price 
    // key is price and set is based on time
    // set<OrderCombine> orderMap;
    // unordered_map<int, set<OrderCombine>::iterator> itMap2;
    
    auto Comp = [](const OrderType& a, const OrderType& b)
                {    
                    if (a.price == b.price)
                    {
                        return a.time < b.time;
                    }

                    return a.price < b.price;
                };

    
    set<OrderType, decltype(Comp)> orderSet(Comp);
    unordered_map<int, set<OrderType, decltype(Comp)>::iterator> itMap;
    auto it = orderSet.insert(OrderType(1, 1, 2));
    itMap[2] = it.first;
    it = orderSet.insert(OrderType(1, 2, 5));
    itMap[5] = it.first;
    it = orderSet.insert(OrderType(2, 1, 4));
    itMap[4] = it.first;
    for (auto it = orderSet.begin(); it != orderSet.end(); ++it)
    {
        cout << it->id << endl;
    }
    return 0;
}
