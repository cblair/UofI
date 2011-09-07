#ifndef _functions_h
#define _functions_h

//1. For lab
void FilterAbove(int A[], int &nA, int t)
{
  int temp[nA];

  //This is a seperate index counter for the 
  // temp array. See the below for loop
  int nT = 0;
  for(int i = 0; i < nA; ++i)
    {
      //If the element meets the theshold
      //requirement, copy it to the temp array
      //and increments the temp array's count
      //to the next element
      if(A[i] <= t) 
	{ 
	  temp[nT] = A[i];
	  ++nT;

	}
    }
  
  //We will now set the A array to the new temp 
  //array's values, and set it's count to the
  //temp array's count. They are then 'returned'
  //because they were passed by reference.
  for(int i = 0; i < nT; ++i) A[i] = temp[i];
  nA = nT;
}

//2. For lab
void FilterBelow(int A[], int &nA, int t)
{
  int temp[nA];

  //This is a seperate index counter for the 
  // temp array. See the below for loop
  int nT = 0;
  for(int i = 0; i < nA; ++i)
    {
      //If the element meets the theshold
      //requirement, copy it to the temp array
      //and increments the temp array's count
      //to the next element
      if(A[i] >= t) 
	{ 
	  temp[nT] = A[i];
	  ++nT;
	}
    }
  //We will now set the A array to the new temp 
  //array's values, and set it's count to the
  //temp array's count. They are then 'returned'
  //because they were passed by reference.
  for(int i = 0; i < nT; ++i) A[i] = temp[i];
  nA = nT;
}

//3. For lab - loaded functions, same as above
// except they do not destroy the original
void FilterAbove(int A[], int B[], int &nA, int &nB, int t)
{
  nB = 0;
  for(int i = 0; i < nA; ++i)
    {
      if(A[i] <= t) 
	{ 
	  B[nB] = A[i];
	  ++nB;
	}
    }
}
void FilterBelow(int A[], int B[], int &nA, int &nB, int t)
{
  nB = 0;
  for(int i = 0; i < nA; ++i)
    {
      if(A[i] >= t) 
	{ 
	  B[nB] = A[i];
	  ++nB;
	}
    }
}

#endif
