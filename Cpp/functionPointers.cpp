#include <iostream>
using namespace std;
//------------------------------------------------------------------------------------
// 2.8 How to Use Arrays of Function Pointers

// C ---------------------------------------------------------------------------------
int DoIt  (float a, char b, char c)
{ 
    printf("DoIt\n");
    return a+b+c; 
}
int DoMore(float a, char b, char c)
{
    printf("DoMore\n"); return a-b+c; 
}
// type-definition: 'pt2Function' now can be used as type
typedef int (*pt2Function)(float, char, char);

// illustrate how to work with an array of function pointers
void Array_Of_Function_Pointers()
{
   printf("\nExecuting 'Array_Of_Function_Pointers'\n");

   // define arrays and ini each element to NULL, <funcArr1> and <funcArr2> are arrays
   // with 10 pointers to functions which return an int and take a float and two char

   // first way using the typedef
   pt2Function funcArr1[10] = {NULL};

   // 2nd way directly defining the array
   int (*funcArr2[10])(float, char, char) = {NULL};


   // assign the function's address - 'DoIt' and 'DoMore' are suitable functions
   // like defined above in 2.1-4
   funcArr1[0] = funcArr2[1] = &DoIt;
   funcArr1[1] = funcArr2[0] = &DoMore;

   /* more assignments */

   // calling a function using an index to address the function pointer
   printf("%d\n", funcArr1[1](12, 'a', 'b'));         //  short form
   printf("%d\n", (*funcArr1[0])(12, 'a', 'b'));      // "correct" way of calling
   printf("%d\n", (*funcArr2[1])(56, 'a', 'b'));
   printf("%d\n", (*funcArr2[0])(34, 'a', 'b'));
}


// C++ -------------------------------------------------------------------------------
class TMyClass
{
public:
   int DoIt(float a, char b, char c){ cout << "TMyClass::DoIt"<< endl; return a+b+c;};
   int DoMore(float a, char b, char c)
         { cout << "TMyClass::DoMore" << endl; return a-b+c; };

   /* more of TMyClass */
};

// type-definition: 'pt2Member' now can be used as type
typedef int (TMyClass::*pt2Member)(float, char, char);

// illustrate how to work with an array of member function pointers
void Array_Of_Member_Function_Pointers()
{
   cout << endl << "Executing 'Array_Of_Member_Function_Pointers'" << endl;

   // define arrays and ini each element to NULL, <funcArr1> and <funcArr2> are
   // arrays with 10 pointers to member functions which return an int and take
   // a float and two char

   // first way using the typedef
   pt2Member funcArr1[10] = {NULL};

   // 2nd way of directly defining the array
   int (TMyClass::*funcArr2[10])(float, char, char) = {NULL};


   // assign the function's address - 'DoIt' and 'DoMore' are suitable member
   //  functions of class TMyClass like defined above in 2.1-4
   funcArr1[0] = funcArr2[1] = &TMyClass::DoIt;
   funcArr1[1] = funcArr2[0] = &TMyClass::DoMore;
   /* more assignments */

   // calling a function using an index to address the member function pointer
   // note: an instance of TMyClass is needed to call the member functions
   TMyClass instance;
   cout << (instance.*funcArr1[1])(12, 'a', 'b') << endl;
   cout << (instance.*funcArr1[0])(12, 'a', 'b') << endl;
   cout << (instance.*funcArr2[1])(34, 'a', 'b') << endl;
   cout << (instance.*funcArr2[0])(89, 'a', 'b') << endl;
}

int main()
{
    Array_Of_Function_Pointers();
    Array_Of_Member_Function_Pointers();
    return 0;
}
