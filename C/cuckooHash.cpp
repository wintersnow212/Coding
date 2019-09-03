/*
Cuckoo Hash 是一种hash冲突解决方法， 其目的是即时使用简易的hash function 也能够实现hash key的均匀分布。

基本思想是使用2个hash函数来处理碰撞，从而每个key都对应到2个位置。

插入操作如下：

1. 对key值hash，生成两个hash key值，hashk1和 hashk2, 如果对应的两个位置上有一个为空，那么直接把key插入即可。

2. 否则，任选一个位置，把key值插入，把已经在那个位置的key值踢出来。

3. 被踢出来的key值，需要重新插入，直到没有key被踢出为止。
cuckoo hashing处理碰撞的方法，就是把原来占用位置的这个元素踢走，不过被踢出去的元素还要比鸟蛋幸运，因为它还有一个备用位置可以安置，
如果备用位置上还有人，再把它踢走，如此往复。直到被踢的次数达到一个上限，才确认哈希表已满，并执行rehash操作

Lookups take worstcase time O(1) because only two locations must be checked

Deletions take worstcase time O(1) because only two locations must be checked.
*/

/* Insert 
To insert an element x, start by inserting it into table 1.
● If h₁(x) is empty, place x there.
● Otherwise, place x there, evict the old element y, and try placing y into table 2.
● Repeat this process, bouncing between tables, until all elements stabilize.

An insertion fails if the displacements form an infinite cycle.
● If that happens, perform a rehash by choosing a new h₁ and h₂ and inserting all elements back into the tables.
● Multiple rehashes might be necessary before this succeeds.
*/

// http://www.keithschwarz.com/interesting/code/cuckoo-hashmap/CuckooHashMap.java.html


// C++ program to demonstrate working of Cuckoo 
// hashing. 
#include<bits/stdc++.h> 
  
// upper bound on number of elements in our set 
#define MAXN 11
  
// choices for position 
#define ver 2 
  
// Auxiliary space bounded by a small multiple 
// of MAXN, minimizing wastage 
int hashtable[ver][MAXN]; 
  
// Array to store possible positions for a key 
int pos[ver]; 
  
/* function to fill hash table with dummy value 
 * dummy value: INT_MIN 
 * number of hashtables: ver */
void initTable() 
{ 
    for (int j=0; j<MAXN; j++) 
        for (int i=0; i<ver; i++) 
            hashtable[i][j] = INT_MIN; 
} 
  
/* return hashed value for a key 
 * function: ID of hash function according to which 
    key has to hashed 
 * key: item to be hashed */
int hash(int function, int key) 
{ 
    if (function == 1)
    { 
        return key%MAXN;
    }
    else
    {
        return (key/MAXN)%MAXN; 
    }
} 

  
/* function to place a key in one of its possible positions 
 * tableID: table in which key has to be placed, also equal 
   to function according to which key must be hashed 
 * cnt: number of times function has already been called 
   in order to place the first input key 
 * n: maximum number of times function can be recursively 
   called before stopping and declaring presence of cycle */
void insert(int key, int tableID, int cnt, int n) 
{ 
    /* if function has been recursively called max number 
       of times, stop and declare cycle. Rehash. */
    if (cnt==n) 
    { 
        printf("%d unpositioned\n", key); 
        printf("Cycle present. REHASH.\n"); 
        return; 
    } 
  
    /* calculate and store possible positions for the key. 
     * check if key already present at any of the positions. 
      If YES, return. */
    for (int i= 0; i<ver; i++) 
    { 
        pos[i] = hash(i+1, key); 
        if (hashtable[i][pos[i]] == key) 
           return; 
    } 
  
    /* check if another key is already present at the 
       position for the new key in the table 
     * If YES: place the new key in its position 
     * and place the older key in an alternate position 
       for it in the next table */
    if (hashtable[tableID][pos[tableID]]!=INT_MIN) 
    { 
        int dis = hashtable[tableID][pos[tableID]]; 
        hashtable[tableID][pos[tableID]] = key; 
        // 把它挤出去了 换一个table继续insert
        insert(dis, (tableID+1)%ver, cnt+1, n); 
    } 
    else //else: place the new key in its position 
       hashtable[tableID][pos[tableID]] = key; 
} 
  
/* function to print hash table contents */
void printTable() 
{ 
    printf("Final hash tables:\n"); 
  
    for (int i=0; i<ver; i++, printf("\n")) 
        for (int j=0; j<MAXN; j++) 
            (hashtable[i][j]==INT_MIN)? printf("- "): 
                     printf("%d ", hashtable[i][j]); 
  
    printf("\n"); 
} 
  
/* function for Cuckoo-hashing keys 
 * keys[]: input array of keys 
 * n: size of input array */
void cuckoo(int keys[], int n) 
{ 
    // initialize hash tables to a dummy value (INT-MIN) 
    // indicating empty position 
    initTable(); 
  
    // start with placing every key at its position in 
    // the first hash table according to first hash 
    // function 
    for (int i= 0, cnt = 0; i < n; i++, cnt=0) 
        insert(keys[i], 0, cnt, n); 
  
    //print the final hash tables 
    printTable(); 
} 
  
/* driver function */
int main() 
{ 
    /* following array doesn't have any cycles and 
       hence  all keys will be inserted without any 
       rehashing */
    int keys_1[] = {20, 50, 53, 75, 100, 67, 105, 
                    3, 36, 39}; 
  
    int n = sizeof(keys_1)/sizeof(int); 
  
    cuckoo(keys_1, n); 
  
    /* following array has a cycle and hence we will 
       have to rehash to position every key */
    int keys_2[] = {20, 50, 53, 75, 100, 67, 105, 
                    3, 36, 39, 6}; 
  
    int m = sizeof(keys_2)/sizeof(int); 
  
    cuckoo(keys_2, m); 
  
    return 0; 
} 
