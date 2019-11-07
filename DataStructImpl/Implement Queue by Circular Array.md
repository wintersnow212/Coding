### Implement Queue by Circular Array

https://www.lintcode.com/problem/implement-queue-by-circular-array/description

Implement queue by circulant array. You need to support the following methods:

1. `CircularQueue(n):` initialize a circular array with size n to store elements
2. `boolean isFull():` return `true` if the array is full
3. `boolean isEmpty():` return `true` if there is no element in the array
4. `void enqueue(element):` add an element to the queue
5. `int dequeue():` pop an element from the queue

### Example

```
CircularQueue(5)
isFull()   => false
isEmpty() => true
enqueue(1)
dequeue()  => 1
```

### Notice

it's guaranteed in the test cases we `won't call enqueue if it's full` and we also `won't call dequeue if it's empty`. So it's ok to raise exception in scenarios described above.



## Solution 1 引入eleNum

关键是引入eleNum 记录element的个数大大简化了isFull 和 isEmpty的判断

```c++
class CircularQueue {
public:
    /**
     * @return:  return true if the array is full
     */
    CircularQueue(int n) 
    {
        // do intialization if necessary
        s = new int [n];
        write = 0;
        read = 0;
        eleNum = 0;
        size = n;
    }

    bool isFull() {
        // write your code here
        return eleNum == size;
    }

    /**
     * @return: return true if there is no element in the array
     */
    bool isEmpty() {
        // write your code here
        return (eleNum == 0);
    }

    /**
     * @param element: the element given to be added
     * @return: nothing
     */
    void enqueue(int element) 
    {
        // write your code here
        if (eleNum == size)
        {
            throw "The stack is full";
        }
        
        s[write++] = element;
        write = write % size;
        eleNum++;
    }

    /**
     * @return: pop an element from the queue
     */
    int dequeue() 
    {
        // write your code here
        if (eleNum == 0)
        {
            throw "The stack is empty";
        }
        
        int val = s[read++];
        read = read % size;
        eleNum--;
        return val;
    }
private:
    int* s;
    int size;
    int eleNum;
    int write;
    int read;
};
```



#### 上面的实现有一个问题 就是 eleNum是shared between multiple thread的

### 不用eleNum的方法

困难点就在于区分full empty 上面的方法的话 都是write == read所以难以区分

## Solution 2 多预留一个slot 

We assume that the **circular buffer**  is  **full** when There is **one empty slot** left in the **circular buffer**:

所以size 也要+1

```c++
class CircularQueue {
public:
    /**
     * @return:  return true if the array is full
     */
    CircularQueue(int n) 
    {
        // do intialization if necessary
        s = new int [n + 1];
        write = 0;
        read  = 0;
        // 关键这里要多留一个 其实就是浪费一个用来作为
        // 是否为判断full and empty
        size = n + 1;
    }

    bool isFull() {
        // write your code here
      	// 也就是write 要追上了 read pointer 就是full !!!!
        return ((write + 1) % size == read);
    }

    /**
     * @return: return true if there is no element in the array
     */
    bool isEmpty() {
        // write your code here
        return (write == read);
    }

    /**
     * @param element: the element given to be added
     * @return: nothing
     */
    void enqueue(int element) 
    {
        // write your code here
        // There is one empty slot left in the circular buffer:
        if ((write + 1) % size == read)
        {
            throw "The stack is full";
        }
        
        s[write++] = element;
        write = write % size;
    }

    /**
     * @return: pop an element from the queue
     */
    int dequeue() 
    {
        // write your code here
        if (write == read)
        {
            throw "The stack is empty";
        }
        
        int val = s[read++];
        read = read % size;
        return val;
    }
private:
    int* s;
    int size;
    int write;
    int read;
};
```



### 多线程 Solution 1

```c++
class CircularQueue {
public:
    /**
     * @return:  return true if the array is full
     */
    CircularQueue(int n) 
    {
        // do intialization if necessary
        s = new int [n];
        write = 0;
        read = 0;
        eleNum = 0;
        size = n;
    }

    /**
     * @param element: the element given to be added
     * @return: nothing
     */
    void enqueue(int element) 
    {
        // write your code here
      	lock();
        if (eleNum == size)
        {
            throw "The stack is full";
        }
        unlock();
        s[write++] = element;
        write = write % size;
        eleNum++;
    }

    /**
     * @return: pop an element from the queue
     */
    int dequeue() 
    {
        // write your code here
      	lock();
        if (eleNum == 0)
        {
            throw "The stack is empty";
        }
        unlock();
      
        int val = s[read++];
        read = read % size;
        eleNum--;
        return val;
    }
private:
    int* s;
    int size;
    int eleNum;
    int write;
    int read;
};
```



### 多线程 2 with conditional varible 

https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/

https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

[http://www.cs.fsu.edu/~baker/realtime/restricted/notes/prodcons.html](http://www.cs.fsu.edu/~baker/realtime/restricted/notes/prodcons.html)

Any thread that intends to wait on `std::condition_variable` has to

1. acquire a std::unique_lock on the same mutex as used to protect the shared variable
2. execute [wait](https://en.cppreference.com/w/cpp/thread/condition_variable/wait), [wait_for](https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for), or [wait_until](https://en.cppreference.com/w/cpp/thread/condition_variable/wait_until). The wait operations atomically release the mutex and suspend the execution of the thread.
3. When the condition variable is notified, a timeout expires, or a [spurious wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup) occurs, the thread is awakened, and the mutex is atomically reacquired. The thread should then check the condition and resume waiting if the wake up was spurious.

`std::condition_variable` works only with [std::unique_lock](http://en.cppreference.com/w/cpp/thread/unique_lock)<[std::mutex](http://en.cppreference.com/w/cpp/thread/mutex)>; this restriction allows for maximal efficiency on some platforms. [std::condition_variable_any](https://en.cppreference.com/w/cpp/thread/condition_variable_any) provides a condition variable that works with any [*BasicLockable*](https://en.cppreference.com/w/cpp/named_req/BasicLockable) object, such as [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock).

```c++
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
```



### Linux pthread

```c++
class CircularQueue {
public:
    /**
     * @return:  return true if the array is full
     */
    CircularQueue(int n) 
    {
        // do intialization if necessary
        s = new int [n + 1];
        write = 0;
        read  = 0;
        // 关键这里要多留一个 其实就是浪费一个用来作为
        // 是否为判断full and empty
        size = n + 1;
    }

    /**
     * @param element: the element given to be added
     * @return: nothing
     */
    void enqueue(int element) 
    {
        // write your code here
        // There is one empty slot left in the circular buffer:
      	pthread_mutex_lock (&M);
        if ((write + 1) % size == read)
        {
            pthread_cond_wait(&Out_CV, &M);
        }
        
        s[write++] = element;
        write = write % size;
       	pthread_mutex_unlock (&M);
      	pthread_cond_signal (&In_CV);
    }

    /**
     * @return: pop an element from the queue
     */
    int dequeue() 
    {
        // write your code here
      	pthread_mutex_lock (&M);
        while (write == read)
        {
            pthread_cond_wait(&In_CV, &M);
        }
        
        int val = s[read++];
        read = read % size;
       	pthread_mutex_unlock (&M);
      	pthread_cond_signal (&Out_CV);
      	return val;
    }
private:
    int* s;
    int size;
    int write;
    int read;
};

pthread_mutex_init (&M, NULL);
pthread_cond_init (&Out_CV, NULL);
pthread_cond_init (&In_CV, NULL);

```

### 再扩展 multiple reader one writer

read writer lock







### 参考程序

```c++
#define _XOPEN_SOURCE 500
#define _REENTRANT
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <values.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define BUF_SIZE 10 

int b[BUF_SIZE];
int in = 0, out = 0;

#define N_CONSUMERS 6
int arguments[N_CONSUMERS];
/* arguments[i] = i */
pthread_t consumer_id[N_CONSUMERS];
pthread_t producer;

pthread_mutex_t M;
pthread_mutexattr_t mattr;
pthread_cond_t  C;
pthread_condattr_t cattr;

void * consumer_body (void *arg) {
/* create one unit of data and put it in the buffer
   Assumes arg points to an element of the array id_number,
   identifying the current thread.
 */
  int tmp;
  int self = *((int *) arg);

  fprintf(stdout, "consumer thread starts\n"); 
  for (;;) {
     /* enter critical section */
     pthread_mutex_lock (&M);
     /* wait for data in the buffer */
     while (out == in)
       if (pthread_cond_wait (&C, &M)) {
          fprintf (stdout, "pthread_cond_wait: consumer\n");
         exit (-1);
       }
     tmp = b[out];
     /* wake up the producer if the buffer was full; the producer
        will stay blocked until we exit the critical section, below
      */
     if (((in + 1) % BUF_SIZE) == out) pthread_cond_signal (&C);
     out = (out + 1) % BUF_SIZE;     
     /* exit critical section */
     pthread_mutex_unlock (&M);
     /* with the output outside the critical section
        we should expect some interleaving and reordering
      */
     fprintf (stdout, "thread %d:", self);
     fprintf (stdout, "%d\n", tmp); fflush (stdout);
  }
  fprintf(stdout, "consumer thread exits\n"); 
  return NULL;
}
void * producer_body (void * arg) {
/* takes one unit of data from the buffer
 */
   int i;
   fprintf(stdout, "producer thread starts\n"); 
   for (i = 0; i < 100; i++) {
     /* enter critical section */
     pthread_mutex_lock (&M);
     /* wait for space in buffer */
     while (((in + 1) % BUF_SIZE) == out)
       if (pthread_cond_wait (&C, &M)) {
          fprintf (stdout, "pthread_cond_wait: producer\n");
          exit (-1);
       }
     /* put value i into the buffer */
     b[in] = i;
     in = (in + 1) % BUF_SIZE;
     /* leave critical section */
     pthread_mutex_unlock (&M);
     /* wake up at least one consumer */
     pthread_cond_signal (&C);
  }
  return NULL;
}

int main () {
   int i, result;
   pthread_attr_t attrs;
   sigset_t set;

   /* initialize the mutex M and condition variable C */

   pthread_mutex_init (&M, NULL);
   pthread_cond_init (&C, NULL);

   /* start with default attributes */
   pthread_attr_init (&attrs);
   /* add an attribute that will cause us to use all processors
      if we have a machine with multiple processors
    */
   pthread_attr_setscope (&attrs, PTHREAD_SCOPE_SYSTEM);

   /* create producer thread */
   if ((result = pthread_create (
       &producer, /* place to store the id of new thread */
       &attrs,
       producer_body,
       NULL)))  /* no need for argument */ {
      fprintf (stdout, "pthread_create: %d\n", result);
      exit (-1);
   } 
   fprintf(stdout, "producer thread created\n"); 
   /* create consumer threads */
   for (i = 0; i < N_CONSUMERS; i++) {
      arguments[i] = i;
      if ((result = pthread_create (
          &consumer_id[i],
          &attrs,
          consumer_body,
          &arguments[i]))) {
        fprintf (stdout, "pthread_create: %d\n", result);
        exit (-1);
      } 
   }
   fprintf(stdout, "consumer threads created\n");

   /* Put the main thread to sleep so that the program will
      not terminate right away. This gives the other threads
      a chance to run indefinitely.  We can still kill the
      process with SIGINT or a signal that cannot be masked. */
   sigfillset (&set);
   sigdelset (&set, SIGINT);
   sigsuspend (&set);
   return 0;
}
```



### MSDN

```c++
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 10
#define PRODUCER_SLEEP_TIME_MS 500
#define CONSUMER_SLEEP_TIME_MS 2000

LONG Buffer[BUFFER_SIZE];
LONG LastItemProduced;
ULONG QueueSize;
ULONG QueueStartOffset;

ULONG TotalItemsProduced;
ULONG TotalItemsConsumed;

CONDITION_VARIABLE BufferNotEmpty;
CONDITION_VARIABLE BufferNotFull;
CRITICAL_SECTION   BufferLock;

BOOL StopRequested;

DWORD WINAPI ProducerThreadProc (PVOID p)
{
    ULONG ProducerId = (ULONG)(ULONG_PTR)p;

    while (true)
    {
        // Produce a new item.

        Sleep (rand() % PRODUCER_SLEEP_TIME_MS);

        ULONG Item = InterlockedIncrement (&LastItemProduced);

        EnterCriticalSection (&BufferLock);

        while (QueueSize == BUFFER_SIZE && StopRequested == FALSE)
        {
            // Buffer is full - sleep so consumers can get items.
            SleepConditionVariableCS (&BufferNotFull, &BufferLock, INFINITE);
        }

        if (StopRequested == TRUE)
        {
            LeaveCriticalSection (&BufferLock);
            break;
        }

        // Insert the item at the end of the queue and increment size.

        Buffer[(QueueStartOffset + QueueSize) % BUFFER_SIZE] = Item;
        QueueSize++;
        TotalItemsProduced++;

        printf ("Producer %u: item %2d, queue size %2u\r\n", ProducerId, Item, QueueSize);

        LeaveCriticalSection (&BufferLock);

        // If a consumer is waiting, wake it.

        WakeConditionVariable (&BufferNotEmpty);
    }

    printf ("Producer %u exiting\r\n", ProducerId);
    return 0;
}

DWORD WINAPI ConsumerThreadProc (PVOID p)
{
    ULONG ConsumerId = (ULONG)(ULONG_PTR)p;

    while (true)
    {
        EnterCriticalSection (&BufferLock);

        while (QueueSize == 0 && StopRequested == FALSE)
        {
            // Buffer is empty - sleep so producers can create items.
            SleepConditionVariableCS (&BufferNotEmpty, &BufferLock, INFINITE);
        }

        if (StopRequested == TRUE && QueueSize == 0)
        {
            LeaveCriticalSection (&BufferLock);
            break;
        }

        // Consume the first available item.

        LONG Item = Buffer[QueueStartOffset];

        QueueSize--;
        QueueStartOffset++;
        TotalItemsConsumed++;

        if (QueueStartOffset == BUFFER_SIZE)
        {
            QueueStartOffset = 0;
        }

        printf ("Consumer %u: item %2d, queue size %2u\r\n", 
            ConsumerId, Item, QueueSize);

        LeaveCriticalSection (&BufferLock);

        // If a producer is waiting, wake it.

        WakeConditionVariable (&BufferNotFull);

        // Simulate processing of the item.

        Sleep (rand() % CONSUMER_SLEEP_TIME_MS);
    }

    printf ("Consumer %u exiting\r\n", ConsumerId);
    return 0;
}

int main ( void )
{
    InitializeConditionVariable (&BufferNotEmpty);
    InitializeConditionVariable (&BufferNotFull);

    InitializeCriticalSection (&BufferLock);

    DWORD id;
    HANDLE hProducer1 = CreateThread (NULL, 0, ProducerThreadProc, (PVOID)1, 0, &id);
    HANDLE hConsumer1 = CreateThread (NULL, 0, ConsumerThreadProc, (PVOID)1, 0, &id);
    HANDLE hConsumer2 = CreateThread (NULL, 0, ConsumerThreadProc, (PVOID)2, 0, &id);

    puts ("Press enter to stop...");
    getchar();

    EnterCriticalSection (&BufferLock);
    StopRequested = TRUE;
    LeaveCriticalSection (&BufferLock);

    WakeAllConditionVariable (&BufferNotFull);
    WakeAllConditionVariable (&BufferNotEmpty);

    WaitForSingleObject (hProducer1, INFINITE);
    WaitForSingleObject (hConsumer1, INFINITE);
    WaitForSingleObject (hConsumer2, INFINITE);

    printf ("TotalItemsProduced: %u, TotalItemsConsumed: %u\r\n", 
        TotalItemsProduced, TotalItemsConsumed);
    return 0;
}
```

