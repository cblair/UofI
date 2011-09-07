#include <fstream>

int maint()
{
ofstream fout;

string ofname;
  const char* outname;
	string ofname;
  //Input from user. Ofname is the landing spot for the cin, const char* outname.
  // will be the final destination for the file name input
  
  cout << "Enter the name of the output file: ";
  cin >> ofname;

  //opens file for output, if fail gives an error
  ofstream fout;
  outname = ofname.c_str();
  fout.open(outname);
  //ofstream fout(ofname);
  if(fout.fail())
    {
      cerr << "Failed to open output file.\n";
      exit(1);
    }
}