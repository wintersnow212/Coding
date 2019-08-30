#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
 
template <typename T>
class Queue
{
 public:
 
  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }
 
  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    // 这里用while loop 而不是if 也解决了spurious wakeup issue
    // spurious wakeup : a thread might be awoken from its waiting state even though no 
    // thread signaled the condition variable
    // 所以我们需要while loop 再检查一遍
    // https://www.justsoftwaresolutions.co.uk/threading/condition-variable-spurious-wakes.html
    // http://blog.vladimirprus.com/2005/07/spurious-wakeups.html
    while (queue_.empty())
    {
      	/* why the “pop”s don’t need “mlock.unlock();
      	All unlocking is taken care of by the condition_variable’s wait function and by the 				unique_lock’s destructor.
      	这里的 
      	1. 首先 我们在这wait了 就会立马release mutex 不然别人怎么notify你呢
      	2. suspend the execution of the thread.
      	3. When the condition variable is notified, the mutex is atomically reacquired
      	4. 在出function scope destory lock object的时候 If the object currently owns a lock 
      	   on the managed mutex object,its unlock member is called before destroying object.
      	*/
      	cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void push(const T& item)
  {
    	// 貌似在push的时候都没有管full的情况??? 下面例子有condition variable
    	std::unique_lock<std::mutex> mlock(mutex_);
    	queue_.push(item);
    	mlock.unlock();
    	cond_.notify_one();
  }
 
  void push(T&& item)
  {
    	std::unique_lock<std::mutex> mlock(mutex_);
    	queue_.push(std::move(item));
    	// Manual unlocking is done before notifying, to avoid waking up
    	// the waiting thread only to block again (see notify_one for details)
    	mlock.unlock();
    	/*
      the waiting thread might wake up immediately it is notified, and before the mutex is 	
      unlocked, in which case it will have to block when the mutex is reacquired on the exit 
      from wait. By rewriting the function so that the notification comes after the mutex is 
      unlocked, the waiting thread will be able to acquire the mutex without blocking
      也就是如果不manually unlock的话 wait thread 在被notify后准备re-acquire lock时会被迫等待！！
      */
    	cond_.notify_one();
  }
 
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};