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
