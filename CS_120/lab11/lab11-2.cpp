/////////////////////////////////////////////
//  Class: CS120 Sect. 3
//  Date:  11/06/07
//  Author: Colby Blair
//  Lab:   Lab 11.2
/////////////////////////////////////////////
/*
Submission of lab11-2.cpp completed on Tue Nov  6 21:02:48 PST 2007
Names of files you have checked in:
  Tue Nov  6 20:37:27 2007     498 blai4021.lab11-1.txt
  Tue Nov  6 21:02:48 2007    2930 blai4021.lab11-2.cpp
End of list

Saving old lab11-2.cpp as lab11-2.cpp.bak
Submission of lab11-2.cpp completed on Tue Nov  6 22:26:14 PST 2007
Names of files you have checked in:
  Tue Nov  6 20:37:27 2007     498 blai4021.lab11-1.txt
  Tue Nov  6 22:26:14 2007    3786 blai4021.lab11-2.cpp
  Tue Nov  6 21:02:48 2007    2930 blai4021.lab11-2.cpp.bak
End of list

*/

#include <iostream>

using namespace std;

void print(int *);

int main()
{
  const int DEST_SIZE = 100;
  
  int a1[] = { 58810 , 64012 , 65677 , 67723 , 62836 , 57575 , 61915 , 48213 ,
               99872 , 71542 , 44601 , 29791 , 18632 , 66865 , 2268 , 46842 ,
               80184 , 40008 , 42619 , 81987 , -1 };
  int a2[] = { 77678 , 88074 , 53920 , 45777 , 74156 , 87268 , 87774 , 94988 ,
               83855 , 77481 , 88858 , 68732 , 85327 , 32891 , 55251 , 78338 ,
               52176 , 36662 , 95724 , 32970 , -1 };
  
  int dest1[DEST_SIZE];
  int dest2[DEST_SIZE];
  int dest3[DEST_SIZE];
  
  //1. dest1[] - using simple array mechanics
  //loops until the dest[] element presently is negative
  cout << "dest1[] = \n";
  int i = 0;
  do
  {
    if(a1[i] > a2[i]) dest1[i] = a2[i];
    else if(a1[i] < a2[i]) dest1[i] = a1[i];
    else if(a1[i] == a2[i]) dest1[i] = a1[i];
    //since the loop terminates from a negative, we increment here, and then
    //check if one of the arrays is negative before the loop checks it. If so,
    //the value is set in the last element, and the loop exits.
    ++i;
    if(a1[i] < 0) dest1[i] = a1[i];
    if(a2[i] < 0 && a2[i] < a1[i]) dest1[i] = a2[i]; 
  }
  while(a1[i] >= 0 && a2[i] >= 0);
  print(dest1);
  
  //2. dest2[] - using pointer accesses.
  //same function and flow as 1., but with pointer accesses
  cout << "dest2[] = \n";
  i = 0;
  do
    {
      if( *(a1 + i) > *(a2 + i) ) *(dest2 + i) = *(a2 + i);
      else if( *(a1 + i) < *(a2 + i) ) *(dest2 + i) = *(a1 + i);
      else if( *(a1 + i) == *(a2 + i) ) *(dest2 + i) = *(a1 + i);
      //negative check before terminating the loop
      ++i;
      if( *(a1 + i) < 0 ) *(dest2 + i) = *(a1 + i);
      if( *(a2 + i) < 0 && *(a2 + i) < *(a1 + i ) ) *(dest2 + i) = *(a1 + i);
    }
  while( *(a1 + i) >= 0 && *(a2 + i) >= 0 );
  print(dest2);
  
  //3. dest3[] - using point increments... without additions to the pointer values,
  // like in 2. Same logic flow as 2. and 3, except that we have to declare
  // the pointers before the loop. As you notice, the pointers themselves are
  // incremented, so each loop through the program is dealing with the [0] of 
  // the array, as far as the pointer is concerned... but by ++ the pointers,
  // we make pointer[0] refer to the actual array[++i]. 
  cout << "dest3[] = \n";
  int *pa1, *pa2, *pdest3; 
  pa1 = a1;
  pa2 = a2;
  pdest3 = dest3;

  do
    {
      if(*pa1 > *pa2) *pdest3 = *pa2;
      else if(*pa1 < *pa2) *pdest3 = *pa1;
      else if(*pa1 == *pa2) *pdest3 = *pa1;
      ++pa1;
      ++pa2;
      ++pdest3;
      if(*pa1 < 0) *pdest3 = *pa1;
      if(*pa2 < 0 && *pa2 < *pa1) *pdest3 = *pa2;
    }
  while(*pa1 >= 0 && *pa2 >= 0);
  print(dest3);
}

//array pointer is a reference to whatever point OR array it is passed.
// It prints the first element, then increments its address by 1,
// similar to 3. in main(). We use the do...while style, similar to
// the style found throughout the program, with the usual 'execute
// body, increment, check for negative, loop' flow found throughout 
// the lab as well.
void print(int *array)
{
  do
    {
      cout << *array << '\n';
      ++array;
      if(*array < 0) cout << *array << '\n';
    }
  while(*array >= 0);
}

