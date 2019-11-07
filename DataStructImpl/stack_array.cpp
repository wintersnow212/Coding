include <stdio.h>
#include <string.h> //for strlen（）

/* Implement stack using Resizeable array 其实就是用vector来实现stack只不过这里用array自己实现了vector

借助realloc或者自己实现

malloc一个新的再memcpy
*/
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
            // 可以直接用relloc来实现！！！
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

/* 
Implement stack using fixed- sized array
*/
#define STACK_MAX 100

typedef struct 
{
	int data[STACK_MAX];
	int size;
}Stack;

void Stack_Init (Stack* s)
{
	s->size = 0;
}

int Stack_Top (Stack *s)
{
	if(s->size == 0)
	{
		fprintf (stderr, "Error : stack empty\n");
		return -1;
	}
	
	//这个思想和shell project 很像啊 用一个structure track 同时
	//反映了 size的变化
	return s->data[s->size-1];

}

void Stack_Push (Stack *s, int d)
{
	if (s->size == STACK_MAX)
	{
		fprintf (stderr, "Error : stack full\n");
		return -1;
	}
	else
	{
		s->data[s->size] = d;
		s->size ++;
	}
	
int Stack_Pop(Stack *s, int d)
{
	if(s->size == 0)
	{
		fprintf (stderr, "Error : stack empty\n");
		return -1;
	}
	else 
	{
		int ret = s->data[s->size -1];
		size -- ;
		return ret;
	
	}
}

