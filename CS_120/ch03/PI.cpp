#include <iostream>
#include <iomanip>

using namespace std;

int main ()
{
	const double PI = 3.14159;

	cout << "1. PI=[[" << PI << "]]" << endl;							// display PI
	cout << "2. PI=[[" << setw(15) << PI << "]]" << endl;						// PI with 15 space before it
	cout << "3. PI=[[" << setprecision(2) << PI << "]]" << endl;					// PI to the 10thns decimal spot
	cout << "4. PI=[[" << setw(20) << setfill('*') << PI << "]]" << endl;				// PI with (20 - PI chars) *s before it
	cout << "5. PI=[[" << setiosflags(ios::left) << setw(20) << PI << "]]" << endl; 		// PI with (20 - PI chars) *s after it, set 
													// fill still *

	cout << setprecision(4);								
	cout << "6. PI=]]" << setiosflags(ios::scientific) << PI << "]]" << endl;				// PI in sci. not.
	cout << "7. PI=[[" << setiosflags(ios::left | ios::scientific) << setw(20) << PI << "]]" << endl;	// PI in sci. not., (20 - PI chars) *s 
														// to the right
	

}
