#include <iostream>

using namespace std;

int main()
{
    int num = 5682;
    char* pOut = static_cast<char*>(malloc(32));
    char* lookUp = "0123456789abcdef";

    for (int i = 7; i >= 0; i--)
    {
        cout << (num&0xf)<< endl;
        pOut[i] = lookUp[num&0x0f];
        num >>= 4;
    }
    
    //cout << pOut << endl;
    printf ("Output is 0x%s\n", pOut);
    return 0;
}