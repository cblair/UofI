/* testArray.cpp
 
  Test array manipulation functions.  All array manipulation
  functions are defined in an external library.
 
  Bruce M. Bolden
  September 7, 2007
 
  http://www.cs.uidaho.edu/~bruceb/

  Modified by Colby Blair 04/01/08
  ---------------------------------------------------------
 */
 
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "array.h"

int main()
{
    const int MAX_ELEMENTS = 10;
    int iArray[MAX_ELEMENTS];
    
    int  n;               //  number of elements in the array
    int index;

        //  Open Input/Output file streams (no error checking!)
    ifstream fIn( "array.dat", ios::in );
    ofstream fOut( "array.out", ios::out );
    
    n = ReadArray( fIn, iArray, MAX_ELEMENTS );
    fIn.close();
    WriteArray( fOut, iArray, n );

    fOut << "\n\nMinimum value" << endl;
    int min = MinArray( iArray, n );
    fOut << "\tMinimum: " << min << endl;

    //mod
    fOut << "\n\nMaximum value\n";
    int max = MaxArray(iArray, n);
    fOut << "\tMaximum: " << max << '\n';

    fOut << "\n\nMedian value\n";
    int median = MedianArray(iArray, n);
    fOut << "\tMedian: " << median << '\n';

    fOut << "\n\nStandard Deviation value\n";
    int standD = StandardDeviatonArray(iArray, n);
    fOut << "\tStandard Deviation: " << standD << '\n';

    //end mod
    
        //  Test array search function
    fOut << "\nSearching for 5" << endl;
    index = SearchArray( iArray, n, 5 );
    fOut << "\tindex: " << index << endl;
    
    fOut << "\nSearching for 25" << endl;
    index = SearchArray( iArray, n, 25 );
    fOut << "\tindex: " << index << endl;
    
        //  Test array initialization function
    InitializeArray( iArray, MAX_ELEMENTS, 5 );
    WriteArray( fOut, iArray, n );

    fOut.close();
    
    return 0;
}
