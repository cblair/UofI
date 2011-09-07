#ifndef _functions_h
#define _functions_h

//1. For lab
int *FilterAbove(int A[], int &nA, int t)
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

  //function pointer mod
  int *pTemp = A;
  return(pTemp);
}

//2. For lab
int *FilterBelow(int A[], int &nA, int t)
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

  //function pointer mod
  int *pTemp = A; 
  return(pTemp);
}

int *Evaluate(int A[], int &nA, int t, string op)
{
  if(op == "above") return(FilterAbove(A, nA, t));
  if(op == "below") return(FilterBelow(A, nA, t));

  //if op is somehow not either of the above options,
  //we will return a poiner to an empty array
  int *pFail;
  pFail[0] = 1111;
  return(pFail);
}


//3. For lab - loaded functions, same as above
// except they do not destroy the original
int *FilterAbove(int A[], int B[], int &nA, int &nB, int t)
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
  //function pointer mod
  int *pTemp = B;
  return(pTemp);
}

int *FilterBelow(int A[], int B[], int &nA, int &nB, int t)
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
  //function pointer mod
  int *pTemp =B;
  return(pTemp);
}

int *Evaluate(int A[], int B[], int &nA, int &nB, int t, string op)
{
  if(op == "above") return(FilterAbove(A, B, nA, nB, t));
  if(op == "below") return(FilterBelow(A, B, nA, nB, t));

  //if op is somehow not either of the above options,
  //we will return a poiner to an empty array
  int *pFail;
  pFail[0] = 1111;
  return(pFail);
}

//4. 
//A.Destructive
//The process here is that we copy the listA size to a temp int
// 'count'. If the front list element meets the filter case,
// it is pushed to the back and the list size is incremented to
// reflect that one element was saved. Then, in either case, the
// front of the list is popped so we can check the rest of the
// elements.
IQueue FilterAbove(IQueue &listA, int& nlistA, int t)
{
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
  return(listA);
}

IQueue FilterBelow(IQueue &listA, int& nlistA, int t)
{
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
  return(listA);
}

IQueue Evaluate(IQueue &listA, int& nlistA, int t, string op)
{
  if(op == "above") return(FilterAbove(listA, nlistA, t));
  if(op == "below") return(FilterBelow(listA, nlistA, t));
  
  //if op is somehow not either of the above options,
  //we will return a poiner to an empty queue
  IQueue listFail;
  return(listFail);
}

//B. Non-destructive
//Filter without destructing the original. There
// are many different ways to do this, like copying all the
// list values and reusing the previous 2 sections' code.
// But I wanted to illustrate a step by step approach.

//Here, we just evaluate listA's front element. If it passes
// the filter, we copy push it to the back of listB, and move
// push it to the back of its own listA. We then revaluate for
// the count size of the list like before until  we have copied
// everything to listB that meets the filter.
IQueue FilterAbove(IQueue &listA, IQueue &listB, int &nlistA, 
		   int &nlistB, int t)
{
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
  return(listB);
}

IQueue FilterBelow(IQueue &listA, IQueue &listB, int &nlistA, 
		   int &nlistB, int t)
{
  for(int count = nlistA; count > 0; --count)
    {
      if(t <= listA.front())
        {
          listB.push_back(listA.front());
          ++nlistB;
        }
      listA.push_back(listA.front());
      listA.pop_front();
    }
  return(listB);
}

IQueue Evaluate(IQueue &listA, IQueue &listB, int &nlistA, int &nlistB,
		int t, string op)
{
  if(op == "above") return(FilterAbove(listA, listB, nlistA, nlistB, t));
  if(op == "below") return(FilterBelow(listA, listB, nlistA, nlistB, t));
  
  //if op is somehow not either of the above options,
  //we will return a poiner to an empty queue
  IQueue listFail;
  return(listFail);
}

#endif
