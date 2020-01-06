#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
#include <climits>
using namespace std;
/*
Ask
An order listed on the sell side of the order book

Bid
An order listed on the buy side of the order book 

Bid/Ask Depth
The Bid/Ask Depth represents the cumulative volume of buy and sell orders at a particular price. The bid depth at a given price is the cumulative volume of current buy orders on the book at that price or higher, while the ask depth at a given price is the cumulative volume of current sell orders on the book at that price or lower.

Bid/Ask Spread
The difference in price between the highest bid and the lowest ask on the order book.

*/

struct Order
{
    int side;
    int id;
    //string trader;
    int price;
    int size;
};

class OrderBook
{
public:
    OrderBook()
    {

    }
    
    void Trade(const Order& order);
private:
    struct OrderEntry
    {
        int size;
        int id;
    };
    
    //using Entry = pair<int, int>;
    //using PricePoint = list<Entry>;
    //std::vector<PricePoint> pricePoints;
    void AddOrder(const Order& order);
    
    map<int, list<OrderEntry>> sellOrders;
    map<int, list<OrderEntry>> buyOrders;
    unordered_map<int, list<OrderEntry>::iterator> orderIdMap;
    //map<int, unordered_map<int, int>> buyOrderMap;
    // 这个要写customer comparator
    //map<Entry, int> testOrders;
    
    // Sell
    int askMin;
    // Buy
    int bidMax;
};

void OrderBook::Trade(const Order& order)
{
    // Buy order
    if (order.side == 0)
    {
        /*
        我们要买股票的时候，因为市场上有很多证券交易商，
        就让经纪人去找哪个证券交易商愿意卖的asked price最低，我们就以这个价格买入。
        卖股票的时候就是找最高的bid price。
        你问一个经纪人，最佳的买方报价（asked price）是90块钱，
        最佳的卖方报价（bid price）是91块，所以你需要买一只股票要花90块，
        卖一只股票要花91，这1块钱就是买卖价差（bid-ask price）
        */
        int orderSize = order.size;
        if (sellOrders.empty() == true || sellOrders.begin()->first > order.price)
        {
            AddOrder(order);
        }
        else
        {
            auto sellIt   = sellOrders.begin();
            while (sellIt != sellOrders.end())
            {
                auto sellList   = sellIt->second;
                auto sellListIt = sellList.begin();
                while (sellListIt != sellList.end())
                {
                    if (sellListIt->size < orderSize)
                    {
                        //executeOrder
                        orderSize -= sellListIt->size;
                        sellOrders.begin()->second.pop_front();
                        sellListIt++;
                    }
                    else
                    {
                        //executeOrder
                        if (sellListIt->size == orderSize)
                        {
                            sellOrders.begin()->second.pop_front();
                            sellListIt++;
                        }
                        else
                        {
                            sellListIt->size -= orderSize;
                        }

                        orderSize = 0;
                        break;
                    }
                }
                
                if (orderSize == 0)
                {
                    break;
                }
                
                sellIt++;
            }
            
            sellOrders.erase(sellOrders.begin(), sellIt);
        }
    }
}

void OrderBook::AddOrder(const Order& order)
{
    int price = order.price;
    int orderId = order.id;
    
    if (orderIdMap.find(orderId) != orderIdMap.end())
    {
        orderIdMap[orderId]->size += order.size;
    }
    else
    {
        // Buy order
        if (order.side == 0)
        {
            /*
            get a performance bump in special cases if you use the 'hint' function on the insert         
            operator. 
            So, looking at option 2 from here:iterator insert 
            (const_iterator position, const value_type& val);
            */
            /*
            insert is not a recommended way - it is one of the ways to insert into map.
            The difference with operator[] is that the insert can tell whether the element 
            is inserted into the map. Also, if your class has no default constructor, 
            you are forced to use insert.
            operator[] needs the default constructor because the map checks if the element 
            exists. If it doesn't then it creates one using default constructor and 
            returns a reference (or const reference to it).

            */
            buyOrders[price].push_front({order.size, order.id});
            orderIdMap[order.id] = buyOrders[price].begin();
        }
        else
        {
            sellOrders[price].push_front({order.size, order.id});
            orderIdMap[order.id] = sellOrders[price].begin();
        }
    }
}

// void OrderBook::AddOrder(const Order& order)
// {
//     int price = order.price;
//     // Buy order
//     if (order.side == 0)
//     {
//         /* Look for outstanding sell orders that cross with the incoming order */
//         if (price >= askMin)
//         {
//             auto entryIt = pricesPoins.begin()
//         }
        
//     }
//     else
//     {
        
//     }
// }

int main()
{
    
    return 0;
}
