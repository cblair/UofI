/////////////////////////////////////////////
//	CS-120 Sect. #3
//	Date: 	09/04/07
// 	Author:	Colby Blair
//	Lab 3.2
/////////////////////////////////////////////


#include <iostream>

using namespace std;

int main ()
{

	char chVersion;	
	int inVersion;
	int trueNumVersion;


	cout << "Type in ONE character for analysis, please..." << endl;		//This section asks user for a char, then
	cin >> chVersion;								//transfers the ASCII version to intVersion,
	inVersion = static_cast<int>(chVersion);					//and displays the ASCII version for 
	cout << "The ASCII decimal equivalent is: " << inVersion << endl;		//clarity

											//This section does different things with the 
											//char.
	if ( (inVersion > 47) && (inVersion < 58) )						//Here, we find if it is a #...
	{
		cout << chVersion << " is a number 1-9." << endl;
		trueNumVersion = inVersion - 48;						//Display and use trueNumVersion to do
		trueNumVersion = trueNumVersion * trueNumVersion;				//our math manip.s.
		cout << chVersion << " squared is: " << trueNumVersion << endl; 		//In this case, I just square it and
	}											//display.

	else if ( (inVersion > 64) && (inVersion < 91) )					//Find if it is a capital alpha...
	{
		cout << chVersion << " is a alpha character in CAPITAL form." << endl;		
		inVersion = inVersion + 32;							//and manip. it to a lower case.
		cout << "The lowercase form is: " << static_cast<char>(inVersion) << endl;
	}

	else if ( (inVersion > 96) && (inVersion < 123) )					//Find if it is a lower alpha...
	{
		cout << chVersion << " is a alpa character in lowercase form." << endl;
		inVersion = inVersion - 32;							//and change to uppercase.
		cout << "The UPPERCASE form is: " << static_cast<char>(inVersion) << endl;
	}
	else if ( (inVersion > 6) && (inVersion < 14))						//Find if it is a special character...
	{
		cout << chVersion << " is a special character, such as a newline." << endl;	
	}
	else											//Display if it is nothing else.
	{
		cout << chVersion << " was none of the character we could easily analyze." << endl;
	}
}
