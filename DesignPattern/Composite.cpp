#include <iostream>
using namespace std;

class ExpressNode
{
public:
    //friend class Express;
    int getUse(void) const;
    void setUse(int);
    
    friend ostream& operator<<(ostream& os,const ExpressNode& ExprNode)//(1)输出表达式自身
    {
        ExprNode.print(os);
        return os;
    }
    
    ExpressNode(void):use(1){}
    virtual ~ExpressNode(void);
    virtual int eval() const=0;//(2)计算表达式的值
protected:
    virtual void print(ostream& os) const=0;
private:
    int use;
};

ExpressNode::~ExpressNode(void)
{
}
 
int ExpressNode::getUse() const
{
    return use;
}
 
void ExpressNode::setUse(int use1)
{
    use=use1;
}

class Express
{
public:
    //ValueNode(int) Express(3)
    Express(int);
    //UnaryNode(char,int) Express('+',t,12)
    Express(char,const Express);
    //BinaryNode(char,int,int) Express('+',3,4)
    Express(char,const Express&, const Express&);
    Express(const Express&);
    Express& operator=(const Express&);
    ~Express(void);
    friend ostream& operator<<(ostream& os, const Express& e)
    {
        os << *(e.pExpressNode);
        return os;
    }
    int eval() const;
private:
    ExpressNode* pExpressNode;//具体的功能由这个类实现，这个类派生了各种各样的表达式
};

class ValueNode : public ExpressNode
{
 
public:
    //friend class Express;
    ValueNode(void);
    ValueNode(int value1);
    ~ValueNode(void);
private:
    void print(ostream& os) const;
    int eval() const {return value;}
    int value;
};

ValueNode::ValueNode(void)
{
}
 
ValueNode::ValueNode(int value1):value(value1)
{
}
 
ValueNode::~ValueNode(void)
{
}
 
void ValueNode::print(std::ostream& os) const
{
    os<<value;
}

class UnaryNode : public ExpressNode
{
    
public:
    //friend class Express;
    UnaryNode(void);
    UnaryNode(char c, class Express left1);
    ~UnaryNode(void);
private:
    void print(ostream& os) const;
    int eval() const ;
    char opend;
    Express left;
};

UnaryNode::UnaryNode(char c,class Express left1):opend(c),left(left1)
{
}
 
UnaryNode::~UnaryNode(void)
{
}
 
void UnaryNode::print(std::ostream &os) const
{
    os<<"("<<opend<<left<<")";
}
 
int UnaryNode::eval() const
{
    if(opend=='-')
        return (-1)*left.eval();
    throw "error, bad op int UnaryNode";
}

class BinaryNode : public ExpressNode
{
    
public:
    //friend class Express;
    BinaryNode(void);
    BinaryNode(char, const Express& , const Express&);
    ~BinaryNode(void);
private:
    void print(ostream&) const;
    int eval() const;
    char opend;
    class Express left;
    class Express right;
};

BinaryNode::BinaryNode(char c, const Express& left1, const Express& right1)
        : opend(c)
        , left(left1)
        , right(right1)
{
}
 
BinaryNode::~BinaryNode(void)
{
}
 
void BinaryNode::print(ostream& os) const
{
    os<<"("<<left<<opend<<right<<")";    
}
 
int BinaryNode::eval() const
{
    int op1=left.eval();
    int op2=right.eval();
 
    if(opend=='+') return op1+op2;
    if(opend=='-') return op1-op2;
    if(opend=='*') return op1*op2;
    if(opend=='/'&& op2!=0) return op1/op2;
 
    throw "error, bad operation  in BinaryNode";
}

Express::Express(int a)
{
    pExpressNode = new ValueNode(a);
}
 
Express::Express(char c, const Express e)
{
    pExpressNode = new UnaryNode(c,e);
}

//BinaryNode(char,int,int)
// 这里应该是体现composite的地方 
Express::Express(char c, const Express& el, const Express& er) 
{
    pExpressNode = new BinaryNode(c,el,er);
}
 
Express::Express(const Express& e1)
{
    pExpressNode = e1.pExpressNode;
    pExpressNode->setUse(pExpressNode->getUse()+1);
}
 
Express& Express::operator=(const Express& e1)
{
    (e1.pExpressNode)->setUse((e1.pExpressNode)->getUse()+1);
    
    pExpressNode->setUse(pExpressNode->getUse()-1);
    
    if(pExpressNode->getUse()==0)
        delete pExpressNode;
    
    pExpressNode= e1.pExpressNode;
    return *this;
}
 
Express::~Express(void)
{
    pExpressNode->setUse(pExpressNode->getUse()-1);
    if(pExpressNode->getUse()==0)
        delete pExpressNode;
}
 
int Express::eval() const
{
    // 这里体现了polymoprihm的关键！！！！！！！！！
    return pExpressNode->eval();
}

int main()
{
    Express t = Express(3);
    t = Express('+',t,12);
    cout<< t <<" = "<<t.eval()<<endl;
    Express y = Express('-',4);
    cout<<y<<" = "<< y.eval()<<endl;
    Express t1= Express('*',Express('-',5),Express('+',3,4));
    cout<<t1<<" = "<<t1.eval()<<endl;
    t=Express('*',t1,t1);
    Express t2=Express('*',t,t);
    cout<<t2<<" = "<<t2.eval()<<endl;
    return 0;
}
