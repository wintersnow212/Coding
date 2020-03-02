// https://lokiastari.com/blog/2016/02/29/vector-resource-management-ii-copy-assignment/index.html
// https://lokiastari.com/blog/2016/03/12/vector-resize/index.html
// https://lokiastari.com/blog/2016/03/19/vector-simple-optimizations/index.html
template<typename T>
class Vector
{
    std::size_t     capacity;
    std::size_t     length;
    T*              buffer;
    struct Deleter
    {
        void operator()(T* buffer) const
        {
            ::operator delete(buffer);
        }
    };

    public:
        /* 太屌了 Lazy Construction of elements. 利用placement new!!!
        his attempt improves on that by allowing efficient pre-allocating of space (capacity) for the buffer. 
        New members are then added by constructing in-place using placement new.
        */
        Vector(int capacity = 10)
            : capacity(capacity)
            , length(0)
            , buffer(static_cast<T*>(::operator new(sizeof(T) * capacity)))
        {}
        ~Vector()
        {
            // Make sure the buffer is deleted even with exceptions
            // This will be called to release the pointer at the end
            // of scope.
            std::unique_ptr<T, Deleter>     deleter(buffer, Deleter());

            // Call the destructor on all the members in reverse order
            // Because elements are constructed in-place using
            // placement new. Then we must manually call the destructor
            // on the elements.
            for(int loop = 0; loop < length; ++loop)
            {
                // Note we destroy the elements in reverse order.
                buffer[length - 1 - loop].~T();
            }
        }
        Vector(Vector const& copy)
            : capacity(copy.length)
            , length(0)
            , buffer(static_cast<T*>(::operator new(sizeof(T) * capacity)))
        {
            try
            {
                for(int loop = 0; loop < copy.length; ++loop)
                {
                    push_back(copy.buffer[loop]);
                }
            }
            catch(...)
            {
                std::unique_ptr<T, Deleter>     deleter(buffer, Deleter());
                // If there was an exception then destroy everything
                // that was created to make it exception safe.
                // Copy constructor is simple.
                // We create a new resource area of the required length.
                // But these elements are not initialized so we use push_back to copy them
                // into the new object. This is an improvement because we
                // only construct the members of the vector once.
                for(int loop = 0; loop < length; ++loop)
                {
                    buffer[length - 1 - loop].~T();
                }

                // Make sure the exceptions continue propagating after
                // the cleanup has completed.
                throw;
            }
        }
        // Copy assignment
        Vector& operator=(Vector const& copy)
        {
            // Copy and Swap idiom
            Vector<T>  tmp(copy);
            tmp.swap(*this);
            return *this;
        }

        // Move constructor
        Vector(Vector&& move) noexcept
            : capacity(0)
            , length(0)
            , buffer(nullptr)
        {
            move.swap(*this);
        }

        // Move assignment
        Vector& operator=(Vector&& move) noexcept
        {
            move.swap(*this);
            return *this;
        }

        void swap(Vector& other) noexcept
        {
            using std::swap;
            swap(capacity,      other.capacity);
            swap(length,        other.length);
            swap(buffer,        other.buffer);
        }
        void push_back(T const& value)
        {
            resizeIfRequire();
            pushBackInternal(value);
        }
        void pop_back()
        {
            // When removing elements need to manually call the destructor
            // because we created them using placement new.
            // 所以是不是不是placement new 就不用管了？？？
            --length;
            buffer[length].~T();
        }
        void reserve(std::size_t capacityUpperBound)
        {
            if (capacityUpperBound > capacity)
            {
                reserveCapacity(capacityUpperBound);
            }
        }
    private:
        void resizeIfRequire()
        {
            if (length == capacity)
            {
                std::size_t     newCapacity  = std::max(2.0, capacity * 1.5);
                reserveCapacity(newCapacity);
            }
        }
        void pushBackInternal(T const& value)
        {
            // Use placement new to copy buffer into the new buffer
            new (buffer + length) T(value);
            ++length;
        }
        void reserveCapacity(std::size_t newCapacity)
        {
            Vector<T>  tmpBuffer(newCapacity);
            std::for_each(buffer, buffer + length, [&tmpBuffer](T const& v){tmpBuffer.pushBackInternal(v);});

            tmpBuffer.swap(*this);
        }
};
