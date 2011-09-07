/* TestHash.cpp
 *   Test the Hash table code.
 */

#include <iostream>
#include <fstream>
//#include "stdlib.h"
using namespace std;

#include "hash.cpp"
#include "add_functions.h"

int main()
{  
  cout << "Welcome to the hash dictionary. Please enter the file name\n"
       << "of the dictionary database to insert: "
    ;
  ifstream fin;
  string stemp;
  const char *inputName;
  cin >> stemp;
  inputName = stemp.c_str();
  fin.open(inputName);
  if(fin.fail())
    {
      cerr << "ERROR: File '" << inputName << "' not found. Exiting...\n";
      exit(1);
    }

  //3.
  char *name = " ", *defn = " ";
  //This can only be used for files with names only; no definitions
  while(!fin.eof())
    {
      fin >> stemp;
      char ctemp[stemp.size() - 1];
      string_to_chararray(stemp, ctemp);
      name = ctemp;
      (void)Insert(name, defn);
      //cout << name << " inserted, ";
    }

  PrintHashTable();

  //5.
  NListPtr pBucket;
  while(true)
    {
      cout << "Now searching for words, enter the world to search for, "
	   << "or press Ctrl + C to quit: "
	;
      cin >> stemp;
      char ctemp[stemp.size() - 1];
      string_to_chararray(stemp, ctemp);
      name = ctemp;
      pBucket = Lookup(name);
      
      while( pBucket != NULL )
        {
	  cout << setw(3) << ":    ";
          cout << pBucket->name << ", " << pBucket->defn;
          cout << endl;
          pBucket = pBucket->next;
        }
    }


  fin.close();
  PrintHashTable();
  
  return EXIT_SUCCESS;
}
