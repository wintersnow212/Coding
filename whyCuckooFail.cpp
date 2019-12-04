#include<iostream>
#include <climits>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
int maxCnt = 100;

size_t hasher1(string key)
{
    size_t hashVal = 0;
    for (auto c : key)
    {
        hashVal = 33 * hashVal + c;
    }
    
    return hashVal;
}

size_t hasher2(string key)
{
    size_t hashVal = 0;
    for (auto c : key)
    {
        hashVal = 37 * hashVal + c;
    }
    
    return hashVal;
}

template<int tableNum, int tableSize>
class CuckooHash
{
public:
    CuckooHash()
    {
        for (int t = 0; t < tableNum; ++t)
        {
            for (int c = 0; c < tableSize; ++c)
            {
                hashTable[t][c] = "";
            }
        }

        hashFuncMap[0] = &hasher1;
        hashFuncMap[1] = &hasher2;
    }
    
    
    
    void insert(string key)
    {
        int tableId = 0;
        for (int cnt = 0; cnt < maxCnt; ++cnt)
        {
            pos[tableId] = hashFuncMap[tableId](key) % tableSize;

            if (hashTable[tableId][pos[tableId]] != "")
            {
                if (hashTable[tableId][pos[tableId]] == key)
                {
                    break;
                }

                string movStr = hashTable[tableId][pos[tableId]];

                tableId = (tableId + 1) % tableNum;
                key     = movStr;
            }
            else
            {
                hashTable[tableId][pos[tableId]] = key;
                break;
            }
        }
    }

    bool find(string key)
    {
        for (int tId = 0; tId < tableNum; ++tId)
        {
            pos[tId] = hashFuncMap[tId](key);
            if (hashTable[tId][pos[tId]] == key)
            {
                return true;
            }
        }

        return false;
    }

    bool removeKey(string key)
    {
        for (int tId = 0; tId < tableNum; ++tId)
        {
            pos[tId] = hashFuncMap[tId](key);
            if (hashTable[tId][pos[tId]] == key)
            {
                hashTable[tId][pos[tId]] = "";
                return true;
            }
        }

        return false;
    }
    
    void printFunc()
    {
        printf("Final hash tables:\n"); 
  
        for (int t = 0; t < tableNum; ++t, printf("\n"))
        {
            for (int c = 0; c < tableSize; ++c)
            {
                if (hashTable[t][c] == "")
                {
                    cout << " - ";
                }
                else
                {
                    cout << hashTable[t][c];
                }
                // (hashTable[t][c]== "") ? cout << "- " :
                //                          cout << hashTable[t][c]);
            }
        }
  
        printf("\n"); 
    }
private:
    typedef size_t (*HashFunc) (string key);
    unordered_map<int, HashFunc> hashFuncMap;
    string hashTable[tableNum][tableSize];
    int pos[tableNum];
};


/* driver function */
int main() 
{ 
    /* following array doesn't have any cycles and 
       hence  all keys will be inserted without any 
       rehashing */
    string keys_1[] = {"20", "50", "53", "75", "100", "67", "105", 
                        "3", "36", "39"}; 
  
    int n = sizeof(keys_1)/sizeof(string); 
    CuckooHash<2, 11> cuckoo;
    for (int i = 0; i < n; ++i)
    {
        cuckoo.insert(keys_1[i]);
    }
    
    cuckoo.printFunc();
   
    //cuckoo(keys_1, n); 
  
    /* following array has a cycle and hence we will 
       have to rehash to position every key */
    // int keys_2[] = {20, 50, 53, 75, 100, 67, 105, 
    //                 3, 36, 11, 39, 6, 13}; 
  
    // int m = sizeof(keys_2)/sizeof(int); 
  
    // cuckoo(keys_2, m); 
  
    return 0; 
} 
