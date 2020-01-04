#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    int val;
    Node* next;
    Node()
        : val(0)
        , next(nullptr)
    {
        
    }
    Node(int v)
        : val(v)
        , next(nullptr)
    {
        
    }
};

Node* buildList(vector<int> vec)
{
    Node* head = nullptr;
    Node** trav = &head;
    for (int i : vec)
    {
        *trav = new Node(i);
        trav  = &((*trav)->next);
    }
    
    return head;
    
    /***
    借助next pointer 个人不喜欢
    */
//     Node* head = new Node(vec[0]);
    
//     Node* trav = head;
//     int len = vec.size();
//     for (int i = 1; i < len; ++i)
//     {
//         trav->next = new Node(vec[i]);
//         trav = trav->next;
//     }
    
//     return head;
}

void travList(Node* node)
{
    while (node != nullptr)
    {
        cout << node->val << " ";
        node = node->next;
    }
    
    cout << endl;
}

void freeList(Node* node)
{
    if (node == nullptr)
        return;
    
    Node* nxt = node->next;
    delete(node);
    node = nullptr;
    freeList(nxt);
}

// To execute C++, please define "int main()"
int main() 
{
    vector<int> vec = {1,3, 4, 5, 6};
    Node* head = buildList(vec);
    travList(head);
    freeList(head);
}
