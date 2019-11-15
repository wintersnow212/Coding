#include <iostream>
#include <vector>
using namespace std;

/*
std::vector<T>::iterator is a dependent name, so you need a typename here to specify that it refers to a type. Otherwise it is assumed to refer to a non-type:
*/
template<typename T>using Iter = typename vector<T>::iterator;

template<typename T>
class CommonIterator
{
public:
    CommonIterator(Iter<T> it1Begin,
                   Iter<T> it1End,
                   Iter<T> it2Begin,
                   Iter<T> it2End)
        : m_it1Begin(it1Begin)
        , m_it1End(it1End)
        , m_it2Begin(it2Begin)
        , m_it2End(it2End)
    {
        
    }
    
    bool hasNext()
    {
        while (m_it1Begin != m_it1End &&
               m_it2Begin != m_it2End)
        {
            if (*m_it1Begin == *m_it2Begin)
            {
                return true;
            }
            else if (*m_it1Begin < *m_it2Begin)
            {
                m_it1Begin++;
            }
            else
            {
                m_it2Begin++;
            }
        }
        
        return false;
    }
    
    T next()
    {
        T ret = *m_it1Begin;
        m_it1Begin++;
        m_it2Begin++;
        return ret;
    }
    
private:
    Iter<T> m_it1Begin;
    Iter<T> m_it1End;
    Iter<T> m_it2Begin;
    Iter<T> m_it2End;
};


// To execute C++, please define "int main()"
int main() {
    
    vector<int> v1  = {1,2,3,4,5,6};
    vector<int> v2 = {2, 3, 6,7};
    
    CommonIterator<int> CI(v1.begin(), v1.end(), v2.begin(), v2.end());
    while (CI.hasNext() == true)
    {
        cout << CI.next() << endl;
    }
  
    return 0;
}