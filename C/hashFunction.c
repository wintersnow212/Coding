/*
hashcode("abcd") = (ascii(a) * 333 + ascii(b) * 332 + ascii(c) *33 + ascii(d)) % HASH_SIZE 

                              = (97* 333 + 98 * 332 + 99 * 33 +100) % HASH_SIZE

                              = 3595978 % HASH_SIZE

here HASH_SIZE is the capacity of the hash table (you can assume a hash table is like an array with index 0 ~ HASH_SIZE-1).



*/


int hashCode(char* key, int HASH_SIZE) {
    // write your code here
    int ans = 0;
    //for(int i = 0; i < key.size();i++) {
    int i = 0;
    int len = strlen(key);
    printf("Len is %d \n", len);
    while (key[i] != '\0') {
        ans = (ans * 33 + key[i]) % HASH_SIZE; 
        i++;
    }
    return ans;
}
