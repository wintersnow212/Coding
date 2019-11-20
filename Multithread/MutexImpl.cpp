/*
Test-And-Set
 The semantics of test-and-set are:
◆ Record the old value and
◆ Set the value to indicate available and
◆ Return the old value
*/

struct lock 
{
  int held = 0;
};

void acquire (lock) 
{
    // 这里的操作就是
    // 1. 记住old value
    // 2. 设置 lock->held to 1
    // 3. return old value
    // 所以如果old value 是0就会obtains the lock； otherwise the while-loop spins waiting to acquire the lock
    while (test-and-set(&lock->held) == 1);
}
void release (lock) 
{
    lock->held = 0;
}
