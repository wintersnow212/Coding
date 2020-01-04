#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    int val;
    Node* next;
    Node* prev;
    Node()
        : val(0)
        , next(nullptr)
        , prev(nullptr)
    {
        
    }
    Node(int v)
        : val(v)
        , next(nullptr)
        , prev(nullptr)
    {
        
    }
};

Node* buildList(vector<int> vec)
{
    Node* head = nullptr;
    Node** trav = &head;
    Node* prev = nullptr;
    for (int i : vec)
    {
        *trav = new Node(i);
        (*trav)->prev = prev;
        prev = *trav;
        trav  = &((*trav)->next);
    }
    
    return head;
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

void testDoubleList(Node* node)
{
    for (int i = 0; i < 3; ++i)
    {
        node = node->next;
    }
    
    cout << node->prev->val << endl;
}

// To execute C++, please define "int main()"
int main() 
{
    vector<int> vec = {1,3, 4, 5, 6, 7};
    Node* head = buildList(vec);
    travList(head);
    testDoubleList(head);
    freeList(head);
}
