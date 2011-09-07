//Don't ask...
// Alright, this is a program to return arrays. Why? Because this format was 
// required for lab07, and the alternative is something I can't wrap my mind
// around right now. Not to mention that it is much more painful.

#include <iostream>

using namespace std;

int *return_pointer(int A[])
{
  int *temp;
  temp = A;
  return(temp);
}

int main()
{
  int A[] = {0, 1, 2, 3};
  int *B;

  /*
  B = A;
  for(int i = 0; i < 4; ++i)
    cout << "B[" << i << "] = " << B[i] << '\n';
  */

  B = return_pointer(A);
  for(int i = 0; i < 4; ++i)
    cout << "B[" << i << "] = " << B[i] << '\n';

}

