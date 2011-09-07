/////////////////////////////////////////////////
//CS120 Sect. 3
//Date: 10/01/07
//Author:  Colby Blair
//Lab 7.1
////////////////////////////////////////////////
//Note:  This program has trouble keeping building.name value
//from function to function, so see the below notes for using var
//name in main to help pass the value from function to 
//function.

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

string input_data(const string &);
void print_data(string);

int main()
{
  Building_properties building;
  
  string name = input_data(&building.name);  //recieves the data, gives
                               //name the value of building.string
                               //for passing to the next function: 
  print_data(name);            //prints the data
}

string input_data(const string &building.name)
{
  Building_properties building;
  cout << "Building name: ";
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

  cout << "\nRoom " << building.number << " in building " << building.name 
       << " has telephone number ";

  //if statement for input filtering; doing some math here, so I don't
  //want a bad format to make a muck of things.
  if( (building.telephone >= 1000000) && (building.telephone <= 9999999) )
    {
      //telephone_weird is used to manip the original val from 
      //building.name so we can insert a '-' between the numbers.
      //It takes the first 3 numbers, multiplies them back by 10000,
      //and then subracts that from building.name to get the last 4.
      int telephone_weird = building.telephone * .0001;
      cout << telephone_weird << '-';
      building.telephone -= (telephone_weird * 10000);
      cout << building.telephone << '\n';
    }
   else if(building.telephone = 0)
      cout << '0' << '\n';
   else
      cout << "***-****\nYou did not enter a correct telephone number"
	   << " format!\n";
}
