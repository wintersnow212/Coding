//https://www.jyt0532.com/2017/01/02/c++-multi-thread-p4-2/
#include <iostream> 
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;
class RWLock {
    public:
        RWLock()
            : shared()
              , readerQ(), writerQ()
              , active_readers(0), waiting_writers(0), active_writers(0){}

        bool no_one_writing(){
            return active_readers > 0 || active_writers == 0;
        }
        bool no_one_read_and_no_one_write(){
            return active_readers == 0 && active_writers == 0;
        }
        void ReadLock() {
            std::unique_lock<std::mutex> lk(shared);
            readerQ.wait(lk, bind(&RWLock::no_one_writing, this));
            ++active_readers;
            lk.unlock();
        }

        void ReadUnlock() {
            std::unique_lock<std::mutex> lk(shared);
            --active_readers;
            lk.unlock();
            writerQ.notify_one();
        }

        void WriteLock() {
            std::unique_lock<std::mutex> lk(shared);
            ++waiting_writers;
            writerQ.wait(lk, bind(&RWLock::no_one_read_and_no_one_write, this));
            --waiting_writers;
            ++active_writers;
            lk.unlock();
        }

        void WriteUnlock() {
            std::unique_lock<std::mutex> lk(shared);
            --active_writers;
            if(waiting_writers > 0)
                writerQ.notify_one();
            else
                readerQ.notify_all();
            lk.unlock();
        }

    private:
        std::mutex              shared;
        std::condition_variable readerQ;
        std::condition_variable writerQ;
        int                     active_readers;
        int                     waiting_writers;
        int                     active_writers;
};
int result = 0;
void func(RWLock &rw, int i){
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
void not_safe(int i){
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
