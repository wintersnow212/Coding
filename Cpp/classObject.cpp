#include <iostream>
#include <cstring>
using namespace std;

struct Node
{


    Node()
    {
        
    }
    Node(int a, int b, int c)
    {
        x = a;
        y = b;
        z = c;
    }
 
    /*
    Copy constructor is called when a new object is created from an existing     
    object, as a copy of the existing object (see this G-Fact). 
    And assignment operator is called when an already initialized object is 
    assigned a new value from another existing object.
    */
    Node(const Node& a)
    {
        cout << "copy constructor called " << endl;
    }
    
    Node& operator = (const Node &a) 
    { 
        cout<<"Assignment operator called "<<endl; 
        return *this; 
    }  
    
    int x;
    int y;
    int z;
};


class Box1
{
public:
    Box1(const Node& minNode, const Node& maxNode)
    {
        m_minNode = new Node();
        m_maxNode = new Node();
        *m_minNode = minNode;
        *m_maxNode = maxNode;
    }
    
    ~Box1()
    {
        delete m_minNode;
        m_minNode = nullptr;
        delete m_maxNode;
        m_maxNode = nullptr;
    }
    
    bool isInside(Node* node)
    {
        return (node->x >= m_minNode->x && node->x <= m_maxNode->x &&
                node->y >= m_minNode->y && node->y <= m_maxNode->y);
    }
    
private:
    // If we are using struct/class pointer 
    // I think it is better we manager its memory inside class itself!!
    Node* m_minNode;
    Node* m_maxNode;
};


class Box2
{
public:
    /*
        solution.cpp: In constructor ‘Box2::Box2(const Node&, const Node&)’:
        error: no matching function for call to ‘Node::Node();
        This is becasue I declare two struct object as class member
        If I declare as pointer to struct object that will be fine!!!
    */
    Box2(const Node& minNode, const Node& maxNode)
    {
        m_minNode = minNode;
        m_maxNode = maxNode;
    }
    
    // This will invoke the copy constructor
    // Box2(Node minNode, Node maxNode)
    // {
    //     m_minNode = minNode;
    //     m_maxNode = maxNode;
    // }
    
    ~Box2()
    {
        
    }
    
    bool isInside(Node* node)
    {
        return (node->x >= m_minNode.x && node->x <= m_maxNode.x &&
                node->y >= m_minNode.y && node->y <= m_maxNode.y);
    }
    
private:
    // This will invoke the struct's default constructor
    Node m_minNode;
    Node m_maxNode;
};



// To execute C++, please define "int main()"
int main() 
{
 
    //de minNode(0 ,0, 0);
    Node minNode = {0, 0, 0};
    //Node* minNode = new Node (0, 0, 0);
    Node  maxNode(2, 2, 2);
    Node  testNode(1, 1, 1);
    Box1* B = new Box1(minNode, maxNode);
    
    if (B->isInside(&testNode) == true)
    {
        cout << "Inside" << endl;
    }
    
    delete(B);
    
    Box2 B2(minNode, maxNode);
    
    if (B2.isInside(&testNode) == true)
    {
        cout << "Inside" << endl;
    }
}