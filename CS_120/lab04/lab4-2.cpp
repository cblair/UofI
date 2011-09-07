////////////////////////////////////////
//    Class/Section:  CS-120/#3
//    Date:           09/11/07
//    Author:         Colby Blair
//    Lab:            4-2
////////////////////////////////////////

#include <iostream>

using namespace std;

int main ()
{

    /**********************************************
	This is what the output table's columns' and
	rows' headers will look like, for reference.
	
	     0  1  2  3  4  5  6  7  8  9
	40
	50
	60
	70
	80
	90
	100
	110
 
    **********************************************/

	cout << "     0  1  2  3  4  5  6  7  8  9";	//column header

	for ( int row = 40; row <= 110; row += 10 ) //40-110 rows, 	
		{						//increments of 10
			cout << '\n' << row;		//print row
			if ( row <= 90 ) cout << " ";	//if 40-90, give extra 								//space

							//values for the columns
				for( int numChar = 0; numChar <= 9; numChar++ )
					{
					cout << "  "	//space for columns 
					<< static_cast<char>((row + numChar));
					}
		}
	cout << '\n';
}
