#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

int main()
{
  ofstream fout;

  cout << "Welcome to data generator; please list the data file name\n"
       << "you would like to write to: "
    ;
  string filenametemp = "";
  const char *filename;
  cin >> filenametemp;
  filename = filenametemp.c_str();
  fout.open(filename);
  if(fout.fail())
    {
      cerr << "File '" << filename << "' not found. Exiting...\n";
      exit(1);
    }

  double x, n;
  /*
  cout << "Please enter the value and magnitude of date elements for the "
       << "file (x and n, x^n): ";
  cin >> x >> n;
  for(int i = 0; i < pow(x, n); ++i)
   fout << rand() << '\n';
  */

  cout << "Please enter the amount of data elements: ";
  cin >> x;
  for(int i = 0; i < x; ++i)
    fout << rand() << '\n';

  fout.close();
}
