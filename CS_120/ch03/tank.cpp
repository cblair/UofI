//////////////////////////////////////////////
//	Application Name:	Tank
//	Date: 			08/22/07
//	Author:			Colby Blair
//	Notes:
//		pg. 69 of textbook.
/////////////////////////////////////////////

#include <iostream>
using namespace std;
int main ()
{
	int tank_id;
	double diameter, pressure;

	tank_id = 12;
	diameter = 111.1;
	pressure = 100. ;

	cout << "Tank_id=" << tank_id << ", Diameter=" << diameter << endl;

	tank_id = 11;
	diameter = 82.1;

	cout << "Tank_id=" << tank_id
		<< ",Diameter=" << diameter
		<< ", Pressure=" << pressure << "\n";
}

