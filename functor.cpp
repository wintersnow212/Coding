// Reference http://neutrofoton.github.io/blog/2016/12/29/c-plus-plus-priority-queue-with-comparator/

#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <queue>
using namespace std;

bool compFunc (int i,int j) { return (i<j); }

struct CompStruct {
  bool operator() (int i,int j) { return (i<j);}
};

class Comparator {
public:
    bool operator() (int i, int j) { return (i < j); }
};

void printHelper(vector<int> myvector)
{
    // print out content:
    std::cout << "myvector contains:";
    for (vector<int>::iterator it = myvector.begin(); it!=myvector.end(); ++it){
        std::cout << ' ' << *it;
    }
    
    std::cout << '\n';
}

template<class T>
void printQueue(T& q)
{
    while (!q.empty())
    {
        cout << q.top() << endl;
        q.pop();
    }
}

void SamplePriorityQueue()
{
    // template <
    //     class Type,
    //     class Container=vector<Type>,
    //     class Compare=less<typename Container::value_type> >
    // class priority_queue
    
    // use std built-in comparator
    std::cout << "std style \n";
    priority_queue<int, std::vector<int>, std::greater<int>> q1;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q1.push(n);

    printQueue(q1);
    
    
    std::cout << "functor style \n";
    /*The template parameter should be the type of the comparison function. The function is then either default-constructed or you pass a function in the constructor of priority_queue 
    https://stackoverflow.com/questions/20826078/priority-queue-comparison/21691121
    */
    /* The thing is: you need to tell the priority queue both the type and a pointer to your comparison function. The type goes into the template parameter list, but the type is not enough, so the constructor accepts a value of this type, and since the type is a function pointer type, you can then pass a function pointer. The ampersand & gives you a pointer to a function.*/
    priority_queue<int, std::vector<int>, decltype(&compFunc)> qTest(&compFunc);
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        qTest.push(n);

    printQueue(qTest);
    
    std::cout << "lamdba style \n";
    // lamdba 返回值写法2
    auto compare = [](int lhs, int rhs)
                {
                    return lhs < rhs;
                };
    // Inspects the declared type of an entity or the type and value category of an expression.
    // 1. define the lambda object, then pass it to the template's type using decltype and 
    // 2. also pass it directly to the constructor.
    std::priority_queue<int, std::vector<int>, decltype(compare)> q2(compare);
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q2.push(n);

    printQueue(q2);
    
    std::cout << "functor class style \n";
    // sort 的 Comparator can have () but priorty queue cannot
    std::priority_queue<int, std::vector<int>, Comparator> q3;
    
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q3.push(n);

    printQueue(q3);
}


int main () 
{
    int myints[] = {32,71,12,45,26,80,53,33};
    std::vector<int> myvector (myints, myints+8);               // 32 71 12 45 26 80 53 33
    
    // using default comparison (operator <):
    std::sort (myvector.begin(), myvector.begin()+4);           //(12 32 45 71)26 80 53 33
    
    // using function as comp
    std::sort (myvector.begin()+4, myvector.end(), compFunc); // 12 32 45 71(26 33 53 80)
    
    // using lamabda
    // lamdba 返回值写法1
    std::sort (myvector.begin(), myvector.end(),
               [] (int a, int b) -> bool {
               return a < b;
               }
    );
    printHelper(myvector);
    // using object as comp
    CompStruct cmpStruct;
    //std::sort (myvector.begin(), myvector.end(), cmpStruct);//(12 26 32 33 45 53 71 80)
    std::sort (myvector.begin(), myvector.end(), CompStruct());
    printHelper(myvector);
    
    Comparator cmpClass = Comparator();
    //std::sort (myvector.begin(), myvector.end(), cmpClass);
    std::sort (myvector.begin(), myvector.end(), Comparator());
    printHelper(myvector);
    
    
    
    SamplePriorityQueue();
    return 0;
}