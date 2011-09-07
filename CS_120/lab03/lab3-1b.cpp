/////////////////////////////////////////////
//	CS-120 Sect. #3
//	Date: 	09/04/07
// 	Author:	Colby Blair
//	Lab 3.1b (if only statements)
/////////////////////////////////////////////


#include <iostream>

using namespace std;

int main ()
{

	int x = 0, y = 0;

	cout << "Enter 2 numbers to compare:" << endl;
	cin >> x >> y;

	if (x < y)
	{
		cout << "x < y, " << x << " is the smaller number!" << endl;
	}

	if (x > y)
	{
		cout << "x > y, " << y << " is the smaller number!" << endl;
	}

	if (x == y)
	{
		cout << "x == y!" << endl;
	}

}
