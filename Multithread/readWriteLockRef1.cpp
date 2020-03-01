/*
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html#shared_mutex
The algorithm is derived from an old newsgroup posting of Alexander Terekhov. It starves neither readers nor writers.

There are two "gates", gate1_ and gate2_. Readers and writers have to pass gate1_, and can get blocked in trying to do so. Once a reader gets past gate1_, 
it has read-locked the mutex. Readers can get past gate1_ as long as there are not a maximum number of readers with ownership, and as long as a writer has not gotten past gate1_.

Only one writer at a time can get past gate1_. And a writer can get past gate1_ even if readers have ownership. But once past gate1_, a writer still does not have ownership. 
It must first get past gate2_. A writer can not get past gate2_ until all readers with ownership have relinquished it. Recall that new readers can't get past gate1_ while a writer is waiting at gate2_. 
And neither can a new writer get past gate1_ while a writer is waiting at gate2_.

The characteristic that both readers and writers are blocked at gate1_ with (nearly) identical requirements imposed to get past it, is what makes this algorithm fair to both readers and writers, starving neither.

The mutex "state" is intentionally kept in a single word so as to suggest that the partial use of atomics (as an optimization) for certain state changes is a possibility (i.e. for an uncontended "fast path"). 
However that optimization is not demonstrated here. One example would be if a writer thread could atomically change state_ from 0 to write_entered then he obtains the lock without having to block or even lock/unlock mut_. 
And unlock() could be implemented with an atomic store. Etc. These optimizations are not shown herein because they are much harder to implement correctly than this simple description makes it sound.


*/
class shared_mutex
{
    mutex    mut_;
    condition_variable gate1_;
    condition_variable gate2_;
    unsigned state_;

    static const unsigned write_entered_ = 1U << (sizeof(unsigned)*CHAR_BIT - 1);
    static const unsigned n_readers_ = ~write_entered_;

public:

    shared_mutex() : state_(0) {}

// Exclusive ownership

    void lock();
    bool try_lock();
    void unlock();

// Shared ownership

    void lock_shared();
    bool try_lock_shared();
    void unlock_shared();
};

// Exclusive ownership

void
shared_mutex::lock()
{
    unique_lock<mutex> lk(mut_);
    while (state_ & write_entered_)
        gate1_.wait(lk);
    state_ |= write_entered_;
    while (state_ & n_readers_)
        gate2_.wait(lk);
}

bool
shared_mutex::try_lock()
{
    unique_lock<mutex> lk(mut_, try_to_lock);
    if (lk.owns_lock() && state_ == 0)
    {
        state_ = write_entered_;
        return true;
    }
    return false;
}

void
shared_mutex::unlock()
{
    {
    lock_guard<mutex> _(mut_);
    state_ = 0;
    }
    gate1_.notify_all();
}

// Shared ownership

void
shared_mutex::lock_shared()
{
    unique_lock<mutex> lk(mut_);
    while ((state_ & write_entered_) || (state_ & n_readers_) == n_readers_)
        gate1_.wait(lk);
    unsigned num_readers = (state_ & n_readers_) + 1;
    state_ &= ~n_readers_;
    state_ |= num_readers;
}

bool
shared_mutex::try_lock_shared()
{
    unique_lock<mutex> lk(mut_, try_to_lock);
    unsigned num_readers = state_ & n_readers_;
    if (lk.owns_lock() && !(state_ & write_entered_) && num_readers != n_readers_)
    {
        ++num_readers;
        state_ &= ~n_readers_;
        state_ |= num_readers;
        return true;
    }
    return false;
}

void
shared_mutex::unlock_shared()
{
    lock_guard<mutex> _(mut_);
    unsigned num_readers = (state_ & n_readers_) - 1;
    state_ &= ~n_readers_;
    state_ |= num_readers;
    if (state_ & write_entered_)
    {
        if (num_readers == 0)
            gate2_.notify_one();
    }
    else
    {
        if (num_readers == n_readers_ - 1)
            gate1_.notify_one();
    }
}
