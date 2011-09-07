///////////////////////////////////////
//	Name:		Lab 2-2
//	Date Created:	08/28/07
//	Class:		CS-120 Sect. 03
//	Author:		Colby Blair
///////////////////////////////////////


#include <iostream>

using namespace std;

int main()
{
						//This sections display the problems of 
						//initialization w/o assignment
	int x;
	float y;

	cout << "int x = " << x << endl;
	cout << "float y = " << y << endl;

						//This section displays problems w/ 
						//assignment of a number too big for the type.

	int largeInt = 1000;
	cout << largeInt << endl;
	largeInt = largeInt * largeInt;
	cout << largeInt << endl;
	largeInt = largeInt * largeInt;
	cout << largeInt << endl;
	largeInt = largeInt * largeInt;
	cout << largeInt << endl;
	largeInt = largeInt * largeInt;
	cout << largeInt << endl;
	largeInt = largeInt * largeInt;
	cout << largeInt << endl;

						//This section tries the limit of a float
						//number

	double z = (3.5 *(10^38));

	cout << "double z = " << z << endl;


}
