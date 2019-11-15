##include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <mutex>
using namespace std;

typedef void (*Callable)(int id);
//typedef void (*Callable)();
class CallClass;
void Signal(int id);
void Register(int id, const CallClass& call);
void Remove(int id, const CallClass& call);

class CallClass
{
public:
    CallClass()
    {
        
    }
    
    CallClass(int i)
        : id(i)
    {

    }
    void Call() const
    {
        cout << id << endl;
        //Signal(id);
    }
    
    // 用set 你就得的custom operator < 因为是balanced bst 要排序啊
    bool operator < (const CallClass& other) const
    {
        return this->id < other.id;
    }
    
private:
    int id;
};

// 这里用unordered_set 你就得写custom hash!!
// 用set 你就得的custom operator < 因为是balanced bst 要排序啊
// https://stackoverflow.com/questions/16118889/c-11-equivalent-of-java-util-concurrenthashmap
// https://github.com/intel/tbb
unordered_map<int, set<CallClass>> classMap;

void Register(int id, const CallClass& call)
{
    classMap[id].insert(call);
    //funcMap[id] = cb;
}
    
void Remove(int id, const CallClass& call)
{
    classMap[id].erase(call);
}
    
void Signal(int id)
{
    std::mutex lock;
    std::unique_lock<mutex> l(lock);
    for (auto it = classMap[id].begin(); it != classMap[id].end(); ++it)
    {
        (*it).Call();
    }
}

class SignalApi
{
public:
    void Register(int id, Callable cb)
    {
        funcMap[id].insert(cb);
        //funcMap[id] = cb;
    }
    
    void Remove(int id, Callable cb)
    {
        funcMap[id].erase(cb);
    }
    
    void Signal(int id)
    {
        for (auto it = funcMap[id].begin(); it != funcMap[id].end(); ++it)
        {
             // 这样可以
            (*it)(id);

            // 这样就不行
            //it->();
        }
    }
private:
    unordered_map<int, unordered_set<Callable>> funcMap;
};

void Func1(int id)
{
    cout << id << endl;
}


void Func2(int id)
{
    cout << id << endl;
}

// To execute C++, please define "int main()"
int main() {
    // SignalApi s;
    // s.Register(1, Func1);
    // s.Register(2, Func2);
    // s.Signal(1);
    // s.Signal(2);
    // cout << "------------remove-------------" << endl;
    // s.Remove(2, Func2);
    // s.Signal(1);
    // s.Signal(2);
    
    Register(1, CallClass(1));
    Register(2, CallClass(2));
    Register(2, CallClass(3));
    Signal(1);
    Signal(2);
    cout << "------------remove-------------" << endl;
    Remove(2, CallClass(2));
    Signal(1);
    Signal(2);
    return 0;
}
