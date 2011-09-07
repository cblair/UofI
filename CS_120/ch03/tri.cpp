#include <iostream>

using namespace std;

int main()
{

	double horizleg, vertleg, area1, area2, area3, area4, area5;

	horizleg = 7.0;
	vertleg = 5.0;

	area1 = 0.5 * horizleg * vertleg;

	horizleg /= 2;
	vertleg += 1;
	area2 = 0.5 * horizleg * vertleg;

	horizleg /= 2;
	vertleg += 1;
	area3 = 0.5 * horizleg * vertleg;

	horizleg /= 2;
	vertleg += 1;
	area4 = 0.5 * horizleg * vertleg;

	horizleg /= 2;
	vertleg += 1;
	area5 = 0.5 * horizleg * vertleg;

	cout 	<< "Area 1 = " << area1 << endl
		<< "Area 2 = " << area2 << endl
		<< "Area 3 = " << area3 << endl
		<< "Area 4 = " << area4 << endl
		<< "Area 5 = " << area5 << endl;

}
