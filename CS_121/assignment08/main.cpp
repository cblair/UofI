#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

#include "sort_stat.h"
#include "functions.h"

int main()
{
  ifstream fin;
  ofstream fout;

  cout << "Welcome to the sorting program. Enter the file containing the "
       << "double info to sort: "
    ;
  string infiletemp;
  const char *infile;
  cin >> infiletemp;
  infile = infiletemp.c_str();
  fin.open(infile);
  if(fin.fail())
    {
      cerr << "File '" << infile << "' not creatable. Exiting...\n";
      exit(1);
    }

  int ival = 0;

  //input the data into an array
  int array_size = 0;
  while(!fin.eof())
    {
      fin >> ival;
      ++array_size;
    }
  //--array_size;
  fin.clear();
  fin.seekg(0, ios::beg);

  /*array1 will be used for Insertion Sorting, array2 will be
    used for Selection Sorting, array3 for Quicksort, and
    array4 for Mergesort
  */
  int array1[array_size], array2[array_size];
  int array3[array_size], array4[array_size];
 
  for(int i = 0; i < array_size; ++i)
    {
    fin >> array1[i];
    array2[i] = array1[i];
    array3[i] = array1[i];
    array4[i] = array1[i];
    }

  //Insertion Sort
  cout << "Insertion Sort\n"
       << "---------------------------------------------\n"
       << "Array 1 - Unsorted\n"
    ;
  print_array(array1, array_size);
  Stats insertion = InsertionSort(array1, array_size);
  cout << "Now sorted:\n";
  print_array(array1, array_size);
  cout << "Stats on this kind of sorting:\n"
       << insertion.find_mins << " times looking for a min val,\n"
       << insertion.shifts << " times shifting an array,\n"
       << insertion.copies << " times copying values.\n"
       << "---------------------------------------------\n"
    ;

  //Selection Sort
  cout << "Selection sort\n"
       << "---------------------------------------------\n"
       << "Array 2 - Unsorted\n"
    ;
  print_array(array2, array_size);
  Stats selection = SelectionSort(array2, array_size);
  cout << "Now sorted:\n";
  print_array(array2, array_size);
  cout << "Stats on this kind of sorting:\n"
       << selection.find_mins << " times looking for a min val,\n"
       << selection.swaps << " times swaping values,\n"
      << "---------------------------------------------\n"
    ;

  //Mergesort
  cout << "Mergesort\n"
       << "---------------------------------------------\n"
       << "Array 3 - Unsorted\n"
    ;
  print_array(array3, array_size);
  Stats mergesortstat;
  Mergesort(array3, array3[0], 
	    array3[array_size - 1], mergesortstat);
  cout << "Now sorted:\n";
  print_array(array3, array_size);
  cout << "Stats on this kind of sorting:\n"
       << mergesortstat.copies << " times copying,\n"
       << mergesortstat.comparisons << " times comparing.\n"
      << "---------------------------------------------\n"
    ;

  //Quicksort
  cout << "Quicksort\n"
       << "---------------------------------------------\n"
       << "Array 4 - Unsorted\n"
    ;
  print_array(array4, array_size);
  Stats quicksortstat;
  Quicksort(array4, array4[0], array4[array_size -1 ],
	    quicksortstat);
  cout << "Now sorted:\n";
  print_array(array4, array_size);
  cout << "Stats on this kind of sorting:\n"
       << quicksortstat.pivots << " times pivoting.\n"
       << "---------------------------------------------\n"
    ;

  fin.close();
}
