//custom comparator 多种写法

// 这个新建一个pqElement struct的思想很重要！！！因为我们要记录val是来自哪个row以及一个row的哪个index
struct pqElement
{
    int val;
    int rowId;
    int index;
    pqElement(int value, int row, int i) : val(value), rowId(row), index(i) {}
    // 必须要写成
    // bool operator > (const pqElement& pq) const 一个const都不能少！！！
    bool operator > (const pqElement& pq) const {
        return (val > pq.val);
    }
};

priority_queue<pqElement, vector<pqElement>, std::greater<pqElement>> pq;


/***********************************************************************************************************/
// Custom comparator function 
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
priority_queue<ListNode*, vector<ListNode *>, MyComparator> q;

/***********************************************************************************************************/


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