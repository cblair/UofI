#include <iostream>
#include <fstream>

using namespace std;

int main()
{

	double income = 123.45, expenses = 987.65;
	int week = 7, year = 2006;

	ofstream outfile("/users/b/blai4021/ch4/write.out");

	outfile << "Week = " << week << << endl << "Year = " << year << endl;
	outfile << "Income = " << income << endl << "Expenses = " << expenses << endl;
	outfile.close();

}
