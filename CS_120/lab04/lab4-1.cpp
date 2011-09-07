////////////////////////////////////////
//    Class/Section:  CS-120/#3
//    Date:           09/11/07
//    Author:         Colby Blair
//    Lab:            4-1
////////////////////////////////////////

#include <iostream>

using namespace std;

int main ()
{
    cout << "Please enter up to 10 integers to be added together.\n";

    int sum = 0;
    int number = 1;

    //I use cnt to count how many time cin has occured to make sure 
    //there are only 100 numbers.  I also don't want the number to
    //equal 0.

    for ( int cnt = 0; (cnt <= 100) && (number != 0); cnt++ )
      {
	cin >> number;                                          //cin to number
	sum = sum + number;                                     //and then add that to the
	                                                        //total sum
	
      }                                                        
    
    cout << "The sum of the numbers is: " << sum << '\n';
}
