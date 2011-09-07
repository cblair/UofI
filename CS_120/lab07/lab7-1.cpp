/////////////////////////////////////////////////
//CS120 Sect. 3
//Date: 10/01/07
//Author:  Colby Blair
//Lab 7.1
////////////////////////////////////////////////
//Note:  This program has trouble keeping building.name from 
//function to function, so see the below notes for using var
//name in main to help pass the value from function to 
//function
/*
Submission of lab7-1.cpp completed on Tue Oct  2 15:43:36 PDT 2007
Names of files you have checked in:
  Tue Sep 25 14:25:29 2007    3508 blai4021.lab6.txt
  Tue Oct  2 15:43:36 2007    2527 blai4021.lab7-1.cpp
End of list
*/

#include <iostream>
#include <string>

using namespace std;

struct Building_properties
{
    public:
      string name;
      int number;
      int telephone;
};

string input_data();
void print_data(string);

int main()
{
  string name = input_data();  //recieves the data, gives
                               //name the value of building.string
                               //for passing to the next function: 
                               //prints the data
  if(name == "none")           //if building name is 'none', we 
    exit (1);                  //exit.

  print_data(name);
}

string input_data()
{
  Building_properties building;
  cout << "Building name (spaces are fine): ";
  getline (cin, building.name);
  cout << "Room number: ";
  cin >> building.number;
  cout << "Room telephone (just digits, no \'-\', 0 for none): ";
  cin >> building.telephone;

  return (building.name);     //returns building name since it is
}                             //not retaining its value after the 
                              //function ends.
void print_data(string name)  //using 'name' her to take place of
{                             //building.name
  Building_properties building;
  
  cout << "\nRoom " << building.number << " in building " << name; 
      
  //if statement for input filtering; doing some math here, so I don't
  //want a bad format to make a muck of things.
  if( (building.telephone >= 1000000) && (building.telephone <= 9999999) )
    {
      //telephone_weird is used to manip the original val from 
      //building.name so we can insert a '-' between the numbers.
      //It takes the first 3 numbers, multiplies them back by 10000,
      //and then subracts that from building.name to get the last 4.
      cout << " has phone number: ";
      int telephone_weird = building.telephone * .0001;
      cout << telephone_weird << '-';
      building.telephone -= (telephone_weird * 10000);
      cout << building.telephone << '\n';
    }
   else if(building.telephone == 0)
      cout << " has no telephone.\n";
   else
      cout << "telephone number***-****\nYou did not enter a"
	   <<" correct telephone number format!\n";
}
