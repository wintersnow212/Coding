#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

//Definition for an interval.
struct Interval 
{
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

// This is function call operator
class myFunctor {
public:
    // Function Objects (Functors) - 
    // C++ allows the function call operator() to be overloaded, 
    // such that an object instantiated from a class can be "called"
    // like a function
    bool operator() (Interval a, Interval b)
    {
        return a.end > b.end;
    }
};

int minMeetingRooms(vector<Interval>& intervals) {
    int len = intervals.size();
    if (len == 0)
    {
        return 0;
    }

    // 注意这里两个地方take in 不同的object 
    // sort是take in function objet 所以光是compEnd是不够的
    // priority_queue take in的是type 所以CompEnd就够了
    sort(intervals.begin(), intervals.end(), myFunctor());
    priority_queue<Interval, vector<Interval>, myFunctor> pq;
    
    
    
    pq.push(intervals[0]);
    int ret = 1;
    for (size_t i = 1; i < intervals.size(); ++i)
    {
        Interval top = pq.top();

        if (top.end <= intervals[i].start)
        {
            top.end = intervals[i].end;
            pq.pop();
            pq.push(top);

        }
        else
        {
            ret++;
            pq.push(intervals[i]);
        }
    }

    return ret;
}

struct complx
{
    double real;
    double imag;
    // If you have own constructor, then compiler will not generate 
    // Then you need to define your own
    complx()
    {
        
    }
    complx( double r, double i )
    {
        real = r;
        imag = i;
    }
     
    complx operator+ (const complx& other) const
    {
        complx result;
        result.real = (this->real + other.real);
        result.imag = (this->imag + other.imag);
        return result;
    }
    
    // complx& never return local varibale by reference
    // complx& operator* (const complx& other) const
    complx operator* (const complx& other) const
    {
        complx result;
        result.real = (this->real * other.real);
        result.imag = (this->imag * other.imag);
        return result;
    }
    
    bool operator > (const complx& other)
    {
        if (this->real == other.real)
        {
            return this->imag > other.imag;
        }
        
        return this->real > other.real;
    }
};

std::ostream& operator<<(std::ostream& stream, const complx& other)
{
    stream << other.real << "." << other.imag << endl;
    
    return stream;
}

int main()
{
    complx x(4,4);
    complx y(4,6);
    complx z = x + y; // calls complx::operator+()
    // cout << z.real << endl;
    // cout << z.imag << endl;
    cout << z << endl;
    complx m = x * y;
    // cout << m.real << endl;
    // cout << m.imag << endl;
    cout << m << endl;
    if (x > y)
    {
        cout << "correct" << endl;
    }
    else
    {
        cout << "wrong" << endl;
    }
}    
