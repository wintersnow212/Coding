#include <iostream> 
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
using namespace std;

class RWLock 
{
public:
    RWLock()
        : active_readers(0)
        , active_writers(0)
        , waiting_writers(0)
    {
        
    }
    
    void ReadLock()
    {
        unique_lock<mutex> lock(shared);
        while (noWrite() == false)
        {
            readerCV.wait(lock);
        }
        
        active_readers++;
    }
    
    void ReadUnlock()
    {
        unique_lock<mutex> lock(shared);
        --active_readers;
        lock.unlock();
        writerCV.notify_one();
    }
    
    void WriteLock()
    {
        unique_lock<mutex> lock(shared);
        waiting_writers++;
        /* 等价于
        while (!f()) 
        {
            wait(lk);
        }
        要特别注意啊 上面和下面两个的条件是相反的！！！！！！
        wait(lk, f)    
        等价于
         while (noWriteAndNoRead() == false)
        {
             writerQ.wait(lock);
        }
        看到没有 两个的判断条件是相反的！！！！！下面是表示要meet那个条件
        */
        // Write 必须是no one writer and no one read!!!! 
        // lamdba的写法
        writerCV.wait(lock, [this](){return noWriteAndNoRead() == true;});
        // Bind 的写法
        //writerCV.wait(lock, bind(&RWLock::noWriteAndNoRead, this));
        waiting_writers--;
        ++active_writers;
    }
    
    void WriteUnlock()
    {
        unique_lock<mutex> lock(shared);
        --active_writers;
        
        if (waiting_writers > 0)
        {
            writerCV.notify_all();
        }
        else
        {
            readerCV.notify_all();
        }
    }
private:
    //那Write-preferring的RW Lock怎麼改呢 很簡單 就改變reader做事的condition即可

    // bool no_one_writing(){
    //     return waiting_writers == 0 && active_writers == 0;
    // }
    
    // reader prefered
    bool noWrite()
    {
        // 其实active_reader > 0 已经表示了active writers = 0
        // 貌似直接active_writer == 0 更好？？？
        return active_writers == 0;
        //return active_readers > 0 || active_writers == 0;
    }
    
    bool noWriteAndNoRead()
    {
        return active_readers == 0 && active_writers == 0;
    }
    mutex shared;
    condition_variable readerCV;
    condition_variable writerCV;
    int active_readers;
    int active_writers;
    int waiting_writers;
    // 不需要waiting_readers 的原因是因为我们总是notify all readers!!!!
};

int result = 0;
void func(RWLock &rw, int i)
{
    if(i % 2 == 0){
        rw.WriteLock();
        result += 1;
        rw.WriteUnlock();
        rw.ReadLock();
        rw.ReadUnlock();
    }else{
        rw.ReadLock();
        rw.ReadUnlock();
        rw.WriteLock();
        result += 2;
        rw.WriteUnlock();

    }
}
void not_safe(int i)
{
    if(i%2 == 0){
        result += 1;
    }else{
        result += 2;
    }
}
int main(){
    RWLock rw;
    std::vector<std::thread> threads;
    for(int i = 0; i < 1000; i++){
        threads.push_back(std::thread(func, ref(rw), i));
        //threads.push_back(std::thread(not_safe, i));
    }
    for(int i = 0; i < threads.size() ; i++)
    {
        threads[i].join();
    }
    cout << result << endl;
    return 0;
}
