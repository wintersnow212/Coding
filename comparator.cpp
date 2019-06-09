//custom comparator 多种写法
/******************************************第一种  define the </> operator in the class type*****************************/
// 这个新建一个pqElement struct的思想很重要！！！因为我们要记录val是来自哪个row以及一个row的哪个index
struct pqElement
{
    int val;
    int rowId;
    int index;
    pqElement(int value, int row, int i) : val(value), rowId(row), index(i) {}
    // 必须要写成
    // bool operator < (const pqElement& pq) const 一个const都不能少！！！
    bool operator < (const pqElement& pq) const {
        return (val < pq.val);
    }
};

priority_queue<pqElement> pq;

struct Node {
    int value;
    int from_id;
    int index;
    Node(int _v, int _id, int _i):
        value(_v), from_id(_id), index(_i) {}

    // c++ 的 priority_queue 底层实现 default 就是 std::less 然后每次push node的时候在priority_queue里面会进行一个<的比较 所以要override <
    bool operator < (const Node & obj) const {
        return value < obj.value;
    }
};
priority_queue<Node> queue;


/************************************************第二种 overload function call () operator*************************/
// Custom comparator function object/也叫做functor 本质上和function pointer 一致
// https://en.wikipedia.org/wiki/Function_object
// http://neutrofoton.github.io/blog/2016/12/29/c-plus-plus-priority-queue-with-comparator/

// A functor is pretty much just a class which defines the operator(). That lets you create objects which "look like" a function:
// The line,
// MyFunctor(10);

// Is same as
// MyFunctor.operator()(10);

class MyComparator{
public:
    // override the operator "()"
    bool operator() (ListNode* l1, ListNode* l2)
    {
        return (l1->val > l2->val);
    }
};

// 注意how to declare the priority queue with custom comparator function
        // 1. type 2. container 3. custom comparator

std::vector<ListNode> items

//Pass a functor to std::sort which calls the functor on every element
std::sort(items.begin(), items.end(), MyComparator());
// 区别这里是提供template的type所以不用（）
priority_queue<ListNode*, vector<ListNode *>, MyComparator> q;



// comparator predicate: returns true if a < b, false otherwise
struct IntComparator
{
  bool operator()(const int &a, const int &b) const
  {
    return a < b;
  }
};

int main()
{
    std::vector<int> items { 4, 3, 1, 2 };
    std::sort(items.begin(), items.end(), IntComparator());
    return 0;
}
// Notice that the syntax for providing the callback to the std::sort() function is identical, but an object is passed instead of a function pointer. 
// When invoked, the callback function is executed just as any other member function, and therefore has full access to the other members (data or functions) of the object. 
// Of course, this is just a trivial example. To understand what power a functor provides more than a regular function, consider the common use case of sorting objects by a particular field. 
// In the following example, a functor is used to sort a simple employee database by each employee's ID number.

struct CompareBy
{
    const std::string SORT_FIELD;
    CompareBy(const std::string& sort_field="name")
      : SORT_FIELD(sort_field)
    {
        /* validate sort_field */
    }
    
    bool operator()(const Employee& a, const Employee& b)
    {
        if (SORT_FIELD == "name")
            return a.name < b.name;
        else if (SORT_FIELD == "age")
            return a.age < b.age;
        else if (SORT_FIELD == "idnum")
            return a.idnum < b.idnum;
        else
            /* throw exception or something */
    }
};

int main()
{
    std::vector<Employee> emps;
    
    /* code to populate database */
    
    // Sort the database by employee ID number
    std::sort(emps.begin(), emps.end(), CompareBy("idnum"));
    
    return 0;
}