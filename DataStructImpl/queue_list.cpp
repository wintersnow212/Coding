/*
 * File: queue.h
 * -------------
 * This interface defines a general queue abstraction that uses
 * templates so that it can work with any element type.
 */
#ifndef _queue_h
#define _queue_h
/*
 * Template class: Queue<ValueType>
 * --------------------------------
 * This class template models a queue, which is a linear collection
 * of values that resemble a waiting line. Values are added at
 * one end of the queue and removed from the other. The fundamental
 * operations are enqueue (add to the tail of the queue) and dequeue
 * (remove from the head of the queue). Because a queue preserves
 * the order of the elements, the first value enqueued is the first
 * value dequeued. Queues therefore operate in a first-in-first-out
 * (FIFO) order. For maximum generality, the Queue class is defined
 * using a template that allows the client to define a queue that
 * contains any type of value, as in Queue<string> or Queue<taskT>.
 */
template <typename ValueType>
class Queue {
public:
/*
 * Constructor: Queue
 * Usage: Queue<ValueType> queue;
 * ------------------------------
 * Initializes a new empty queue containing the specified value type.
 */
 Queue();
/*
 * Destructor: ~Queue
* -------------------
 * Deallocates any heap storage associated with this queue.
 */
 ~Queue(); 

 /*
 * Method: size
 * Usage: int nElems = queue.size();
 * ---------------------------------
 * Returns the number of elements in this queue.
 */
 int size();
/* . . . */
 bool isEmpty();
/* . . . */
 void clear();
/* . . . */
 void enqueue(ValueType value);
/* . . . */
 ValueType dequeue();
/* . . . */
 ValueType peek(); 
/*
 * Implementation notes: Queue data structure
 * ------------------------------------------
 * The list-based queue uses a linked list to store the elements
 * of the queue. To ensure that adding a new element to the tail
 * of the queue is fast, the data structure maintains a pointer to
 * the last cell in the queue as well as the first. If the queue is
 * empty, both the head pointer and the tail pointer are set to NULL.
 */
private:
/* Type for linked list cell */
struct Cell {
    ValueType data; /* The data value */
    Cell *link; /* Link to the next cell */
 };

/* Instance variables */
Cell *head; /* Pointer to the cell at the head */
Cell *tail; /* Pointer to the cell at the tail */
int count; /* Num*/
};

 /*
 * Implementation notes: Queue constructor
 * ---------------------------------------
 * The constructor must create an empty linked list and then
 * initialize the fields of the object.
 */
template <typename ValueType>
Queue<ValueType>::Queue() {
    head = tail = NULL;
    count = 0;
}
/*
 * Implementation notes: ~Queue destructor
 * ---------------------------------------
 * The destructor frees any memory that is allocated by the implementation.
 * Freeing this memory guarantees the client that the queue abstraction
 * will not "leak memory" in the process of running an application.
 * Because clear frees each element it processes, this implementation
 * of the destructor simply calls that method.
 */
template <typename ValueType>
Queue<ValueType>::~Queue() {
    clear();
} 

/*
 * Implementation notes: size, isEmpty, clear
 * ------------------------------------------
 * These implementations should be self-explanatory.
 */
template <typename ValueType>
int Queue<ValueType>::size() {
    return count;
}

template <typename ValueType>
bool Queue<ValueType>::isEmpty() {
    return count == 0;
}

template <typename ValueType>
void Queue<ValueType>::clear() {
    while (count > 0) 
    {
        dequeue();
    }
} 

/*
 * Implementation notes: enqueue
 * -----------------------------
 * This method allocates a new list cell and chains it in
 * at the tail of the queue. If the queue is currently empty,
 * the new cell must also become the head pointer in the queue.
 */
template <typename ValueType>
void Queue<ValueType>::enqueue(ValueType value) 
{ 
    Cell *cp = new Cell;
    cp->data = value;
    cp->link = NULL;
    if (head == NULL) {
        head = cell;
    } 
    else 
    {
        tail->link = cell;
    }
    tail = cell;
    count++;
} 

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty queue and report an
 * error if there is no first element. The dequeue method
 * must also check for the case in which the queue becomes
 * empty and set both the head and tail pointers to NULL.
 */
template <typename ValueType>
ValueType Queue<ValueType>::dequeue() 
{
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    Cell *cp = head;
    ValueType result = cp->data;
    head = cp->link;
    if (head == NULL) 
        tail = NULL;
 
    count--;
    delete cell;
    return result;
}


template <typename ValueType>
ValueType Queue<ValueType>::peek() 
{
 if (isEmpty()) error("peek: Attempting to peek at an empty queue");
 return head->data;
} 