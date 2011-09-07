/* array.cpp

   Array manipulation functions.

   Bruce M. Bolden
   September 7, 2007

   http://www.cs.uidaho.edu/~bruceb/

   ---------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

#include "array.h"


/*  ReadArray -- Reads array elements.
 */
int ReadArray( ifstream& fIn, int A[], const int nMax )
{
    int i = 0;
    int nTmp;

    while( (i < nMax) && (fIn >> nTmp) )
    {
        A[i] = nTmp;
        ++i;
    }

    return i;    //  number of elements stored
}


/*  WriteArray -- Writes one element to a line.
 */
void WriteArray( ofstream &fOut, const int A[], const int nMax )
{
    for( int i = 0 ; i < nMax ; i++ )
        fOut << A[i] << endl;
}


/*  InitializeArray

    Initializes array elements to a programmer defined value.
 */
void InitializeArray( int A[], const int nMax, const int iVal )
{
    for( int i = 0 ; i < nMax ; i++ )
        A[i] = iVal;
}


/*  MinArray

    Finds the minimum value in an array. 
 */
int MinArray( const int A[], const int nMax )
{
    int  min = A[0];

    for( int i = 1 ; i < nMax ; i++ )
    {
        if( A[i] < min )
            min = A[i];
    }

    return  min;
}

//mod

int MaxArray(const int A[], const int nMax)
{
  int max = A[0];

  for(int i = 1; i < nMax; i++)
    {
      if(A[i] > max)
	max = A[i];
    }
  return max;
}

int MedianArray(const int A[], const int nMax)
{
  int median = A[nMax / 2];

  return(median);
}

int StandardDeviatonArray(const int A[], const int nMax)
{

  //1.
  int mean = 0;
  for(int i = 0; i < nMax; ++i)
    {
      mean += A[i];
    }
  mean /= nMax;

  int  min = A[0];
  for( int i = 1 ; i < nMax ; i++ )
    {
      if( A[i] < min )
	min = A[i];
    }

  int max = A[0];
  for(int i = 1; i < nMax; i++)
    {
      if(A[i] > max)
	max = A[i];
    }

  //2.
  min = min - mean;
  max = max - mean;

  //3.
  min = min * min;
  max = max * max;

  //4.
  int standD = min + max;

  //5.
  standD = standD / nMax;

  //6.
  standD = sqrt(standD);

  return(standD);
}

//end mod

//  ------------------------------------------------------------------


/*  SearchArray 
 
    Searches for the value, keyVal, in an array. 
    If successful, the index is returned.
    If unsuccessful, -1 is returned.
 */
int SearchArray( const int A[], const int nMax, const int keyVal )
{
    for( int i = 0 ; i < nMax ; i++ )
    {
        if( A[i] == keyVal )
            return i;
    }

    return  -1;
}
