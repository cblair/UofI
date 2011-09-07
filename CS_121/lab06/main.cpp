#include <iostream>
#include "queue.h"
#include "functions.h"

using namespace std;

int main()
{
  //Initialize all the data structures
  //A. Used for 1., 2., and 3.
  int nA = 9;
  int A[] = {1, 2, 3, 4, 5, 2, 3, 4, 6};
  //B. Used for 2. and 3.
  int nB = 9;
  int B[] = {1, 2, 3, 4, 5, 2, 3, 4, 6};
 
  IQueue listA, listB, listC;
  int nlistA = 9;
  int nlistB = 0;
  int nlistC = 0;
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

  //////////////////
  //1. Filtering above
  //////////////////
  cout << "Filter above function. Here is an array: \n"
       << "A[] = {"
    ;
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';

  cout << "Set t (the threshold) to filter at (greater "
       << "values will be filtered out): "
    ;
  cin >> t;
  FilterAbove(A, nA, t);

  cout << "Now the array looks like this: \n"
       << "A[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';

  //////////////////
  //2. Filtering below
  //////////////////
  cout << "Set t (the threshold) to filter at (less "
       << "values will be filtered out): "
    ;
  cin >> t;
  FilterBelow(A, nA, t);
  
  cout << "Now the array looks like this: \n"
       << "A[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';

  ///////////////////
  //3. Filter above and below without destroying the 
  //   original array; these functions are similar
  //   to the above, but are overloaded
  ///////////////////

  //Above
  cout << "Filtering again, only no destruction of "
       << "original.\n"
       << "Set t (the threshold) to filter at (greater "
       << "values will be filtered out): "
    ;
  cin >> t;
  FilterAbove(A, B, nA, nB, t);
   
  cout << "A[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';
  cout << "B[] = {";
  for(int i = 0; i < nB; ++i)
    {
      cout << B[i];
      if(i != (nB - 1)) cout << ' '; 
    }
  cout << "}, nB = " << nB << ", t = " << t << '\n';

  //Below
  cout << "Set t (the threshold) to filter at (less "
       << "values will be filtered out): "
    ;
  cin >> t;
  FilterBelow(A, B, nA, nB, t);
   
  cout << "A[] = {";
  for(int i = 0; i < nA; ++i)
    {
      cout << A[i];
      if(i != (nA - 1)) cout << ' '; 
    }
  cout << "}, nA = " << nA << ", t = " << t << '\n';

  cout << "B[] = {";
  for(int i = 0; i < nB; ++i)
    {
      cout << B[i];
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

  //A. Filter above, destruct original
  cout << "Please enter the threshold to filtee greater "
       << "values out at: "
    ;
  cin >> t;
  //The process here is that we copy the listA size to a temp int
  // 'count'. If the front list element meets the filter case,
  // it is pushed to the back and the list size is incremented to
  // reflect that one element was saved. Then, in either case, the
  // front of the list is popped so we can check the rest of the 
  // elements.
  for(int count = nlistA; count > 0; --count)
    {
      if(t >= listA.front())
	{
	  listA.push_back(listA.front());
	  ++nlistA;
	}
      listA.pop_front();
      --nlistA;
    }
  cout << "The list is now as follows:\n";
  listA.print_list();
  cout << "List size = " << nlistA << '\n';


  //B. Filter below, destruct original. Similar to A.
  cout << "Please enter the threshold to filter less "
       << "values out at: "
    ;
  cin >> t;
  for(int count = nlistA; count > 0; --count)
    {
      if(t <= listA.front())
	{
	  listA.push_back(listA.front());
	  ++nlistA;
	}
      listA.pop_front();
      --nlistA;
    }
  cout << "The list is now as follows:\n";
  listA.print_list();
  cout << "List size = " << nlistA << '\n';


  //C. Filter above without destructing the original. There
  // are many different ways to do this, like copying all the
  // list values and reusing the previous 2 sections' code.
  // But I wanted to illustrate a step by step approach.
  cout << "Now using non-destructive methods towards the "
       << "original data. Please enter a threshold to filter "
       << "greater values out at: "
    ;
  cin >> t;
  //Here, we just evaluate listA's front element. If it passes
  // the filter, we copy push it to the back of listB, and move
  // push it to the back of its own listA. We then revaluate for
  // the count size of the list like before until  we have copied
  // everything to listB that meets the filter.
  for(int count = nlistA; count > 0; --count)
    {
      if(t >= listA.front())
	{
	  listB.push_back(listA.front());
	  ++nlistB;
	}
      listA.push_back(listA.front());
      listA.pop_front();
    }
  cout << "Original list data untouched:\n";
  listA.print_list();
  cout << "...and the filtered data:\n";
  listB.print_list();


  //D. Filter below without destructing the original. Similar to
  // C.
  cout << "Please enter a threshold to filter "
       << "less values out at: "
    ;
  cin >> t;
  for(int count = nlistA; count > 0; --count)
    {
      if(t <= listA.front())
	{
	  listC.push_back(listA.front());
	  ++nlistC;
	}
      listA.push_back(listA.front());
      listA.pop_front();
    }
  cout << "Original list data untouched:\n";
  listA.print_list();
  cout << "...and the filtered data:\n";
  listC.print_list();
}
