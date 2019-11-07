###  Implement Stack

Implement a stack. You can use any data structure inside a stack except stack itself to implement it.\

https://www.lintcode.com/problem/implement-stack/description

### Example

Example 1:

```
Input:
push(1)
pop()
push(2)
top()  // return 2
pop()
isEmpty() // return true
push(3)
isEmpty() // return false
```

Example 2:

```
Input:
isEmpty()
```



## Resizeable array

借助realloc或者自己实现

malloc一个新的再memcpy

```c++
class Stack {
public:
    Stack () :
        m_index(-1),
        size(2)
    {
        m_storage = (int*)calloc(size, sizeof(int));
    }
    /*
     * @param x: An integer
     * @return: nothing
     */
    void push(int x) {
        // write your code here
        if (isFull() == true)
        {
            // 如果这样能保证内存是连续的吗
            // Most implementations start off with a small array, and whenever it reaches its capacity, a new array of
            // double the size is allocated and the old elements are copied to the new array.
            int oldSize = size;
            size = oldSize*2;
            //m_storage = (int*)realloc(m_storage, size * sizeof(int));
            int* temp = m_storage;
            m_storage = (int*)calloc(size, sizeof(int));
            memcpy(m_storage, temp, oldSize* sizeof(int));
        }
        m_index++;
        m_storage[m_index] = x;
    }

    /*
     * @return: nothing
     */
    void pop() {
        // write your code here
        m_index --;
    }

    /*
     * @return: An integer
     */
    int top() {
        // write your code here
        if (isEmpty() == false)
        {
            return m_storage[m_index];
        }
        else
        {
            return 0;
        }
    }

    /*
     * @return: True if the stack is empty
     */
    bool isEmpty() {
        // write your code here
        return (m_index < 0);
    }
    
    bool isFull()
    {
        return (m_index == size - 1);
    }
private:
    int* m_storage;
    int size;
    int m_index;
};
```

