#include <iostream>
#include "queue.h"
#include "functions.h"

using namespace std;

int main()
{
  //Initialize all the data structures
  int nA = 9;
  int A[] = {1, 2, 3, 4, 5, 2, 3, 4, 6};
  int *pA = NULL;
  int nB = 9;
  int B[] = {1, 2, 3, 4, 5, 2, 3, 4, 6};
  int *pB = NULL;
 
  IQueue listA, listB;
  int nlistA = 9;
  int nlistB = 0;
  listA.push_front(6);
  listA.push_front(4);
  listA.push_front(3);
  listA.push_front(2);
  listA.push_front(5);
  listA.push_front(4);
  listA.push_front(3);
  listA.push_front(2);
  listA.push_front(1);
  /////////////

  //Set default threshold to 0
  int t = 0;
  //Set op to nothing; this var lets the user decide what to do
  string op = "";

  //////////////////
  //1. and 2. Filtering above and below
  //////////////////
  cout << "Filter above and below functions. Here is an array: \n"
       << "A[] = {"
    ;
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';

  do
    {
      cout << "Please enter a filter function to perform on the array\n"
	   << "(\'above\' or \'below\' exactly): "
	;
      cin >> op;
    }
  while(op != "above" && op != "below");

  cout << "Set t (the threshold) to filter at (" << op
       << " values will be filtered out): "
    ;
  cin >> t;

  pA = Evaluate(A, nA, t, op);

  cout << "Now the pointer array looks like this: \n"
       << "pA[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << pA[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';


  ///////////////////
  //3. Filter above and below without destroying the 
  //   original array; these functions are similar
  //   to the above, but are overloaded
  ///////////////////

  do
    {
  cout << "Filtering again, only no destruction of "
       << "original. \'above\' or \'below\' please: "
    ;
      cin >> op;
    }
  while(op != "above" && op != "below");

  cout << "Set t (the threshold) to filter at (" << op
       << " values will be filtered out): "
    ;
  cin >> t;

  pB = Evaluate(A, B, nA, nB, t, op);

  cout << "Original array looks like this: \n"
       << "A[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';


  cout << "Now the pointer array looks like this: \n"
       << "pB[] = {";
  for(int i = 0; i < nB; ++i)
    {
      cout << pB[i];
      if(i != (nB - 1)) cout << ' '; 
    }
  cout << "}, nB = " << nB << ", t = " << t << '\n';


  ////////////////////////////
  //4. Using a linked list
  ///////////////////////////
  cout << "Here is the same data set, only using a linked "
       << "list.\n"
    ;
  listA.print_list();

  //A. Destruct original
  do
    {
      cout << "Please enter a filter function to perform on the array\n"
	   << "(\'above\' or \'below\' exactly): "
	;
      cin >> op;
    }
  while(op != "above" && op != "below");

  cout << "Please enter the threshold to filter " << op
       << " values out at: "
    ;
  cin >> t;

  listA = Evaluate(listA, nlistA, t, op);

  cout << "The list is now as follows:\n";
  listA.print_list();
  cout << "List size = " << nlistA << '\n';


  //B. Filter without destructing the original.
  cout << "Now we will do the same filter, but without changing "
       << "the current data in the list.\n"
    ;
    do
    {
      cout << "Please enter a filter function to perform on the list\n"
	   << "(\'above\' or \'below\' exactly): "
	;
      cin >> op;
    }
  while(op != "above" && op != "below");

  cout << "Please enter the threshold to filter " << op
       << " values out at: "
    ;
  cin >> t;

  listB = Evaluate(listA, listB, nlistA, nlistB, t, op);

  cout << "Original list data untouched:\n";
  listA.print_list();
  cout << "...and the filtered data:\n";
  listB.print_list();
}
