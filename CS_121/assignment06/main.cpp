/* main.cpp
 *
 * CS 121.Bolden.........GNU Version 2...........Colby Blair
 * 04/16/08 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu
 * Assignment 06
 */

#include <iostream>

using namespace std;

#include <fstream>
#include <string>
#include <cmath>
#include "functions.h"
#include "hash.h"

int main()
{
  ifstream fin;

  string state = "", capital = "", spopulation;
  int ipopulation;
  
  cout << "Opening states2000.dat...\n";
  fin.open("states2000.dat");
  if(fin.fail())
    {
      cerr << "ERROR: could not open \'states2000.dat\'. Exiting...\n";
      exit(1);              
    }
  
  //////////////////////////////////////
  //Insert and Display into the hash table
  //////////////////////////////////////
  
  //filter out header...
  string goodHeader = 
      "  State               Capital               Population      ";
    string stemp = "";
    do
      {
	getline(fin, stemp);
      }
    while(stemp != goodHeader);
    //and following white spaces.
    do
      {
	fin >> state; 
      }
    while(state == "");
    //state is now loaded with the first state name
    fin >> capital;
    fin >> spopulation;       
    ipopulation = strtoi(spopulation);
    cout << "\n\nInserting ---> " << state << "       " << capital 
         << "       " << ipopulation 
         << '\n'
      ;   
    insert(state, capital, ipopulation);
    
    //The following is a tricky way of getting the States and capitals apart.
    //Looks scarey, because the data file is less than ideal. This code could
    //still use revision though.  
    fin >> state; 
    while(state != "Total")
      {
	fin >> stemp;
	if(stemp == "Dakota" 
	   || stemp == "Carolina"
	   || stemp == "Hampshire"
	   || stemp == "Virginia"
	   || stemp == "Jersey"
	   || stemp == "York"
	   || stemp == "Island"
	   || stemp == "Mexico")
	  {
	    state += " " + stemp;
	    fin >> stemp;
	  }
	if(stemp == "Little"
	   || stemp == "Baton"
	   || stemp == "Des"
	   || stemp == "Oklahoma"
	   || stemp == "Carson"
	   || stemp == "Santa"
	   || stemp == "St.")
	  {
	    fin >> capital;
	    stemp += " " + capital;
	    capital = stemp;
	  }
	if(stemp == "Salt")
	  {
	    fin >> capital;
	    stemp += " " + capital;
	    fin >> capital;
	    stemp += " " + capital;
	    capital = stemp;     
	  }
	else capital = stemp;
	fin >> spopulation;
	ipopulation = strtoi(spopulation);   
	cout << "Inserting ---> " << state << "       " << capital 
	     << "       " << ipopulation 
	     << '\n'
	  ;                
	insert(state, capital, ipopulation);
	fin >> state;
      }
    
    cout << "Printing Hash Table: \n";
    print_hash_table();
    wait(2);
    
}
