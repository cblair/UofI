///////////////////////////////////////
//	Name:		Lab 2-1
//	Date Created:	08/28/07
//	Class:		CS-120 Sect. 03
//	Author:		Colby Blair
///////////////////////////////////////


#include <iostream>

using namespace std;

int main()
{

	double x, y;
	double sum = 0, average;


	cout << "Please enter a number: " << flush;		//This section inputs x and y, adds
	cin >> x;						//them together into sum,
	cout << "Please enter another number: " << flush;
	cin >> y;
	sum = x + y;
	cout << "The sum of " << x << " plus " << y;
	cout << " is " << sum << '.';

								//This section averages them and
								//displays them.
	average = sum/2;
	cout << "  The average of your numbers is ";
	cout << average << '.' << endl;

	return 0;
	


}
