/* main.cpp
 *
 * CS 121.Bolden.........GNU Version 2...........Colby Blair
 * 04/02/08 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu
 * Assignment 05
*/

#include <iostream>

using namespace std;

#include <fstream>
#include <string>
#include <cmath>
#include "tree.h"
#include "functions.h"


int main()
{
    ifstream fin;
    Tree tree1, tree2, tree3;
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
    //1. Add to tree by State order (lexographically)
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
    InsertStateOrder(tree1, state, capital, ipopulation);
    
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
         state += stemp;
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
              stemp += capital;
              capital = stemp;
             }
     if(stemp == "Salt")
     {
      fin >> capital;
      stemp += capital;
      fin >> capital;
      stemp += capital;
      capital = stemp;     
     }
     else capital = stemp;
     fin >> spopulation;
     ipopulation = strtoi(spopulation);   
     cout << "Inserting ---> " << state << "       " << capital 
         << "       " << ipopulation 
         << '\n'
         ;                
     InsertStateOrder(tree1, state, capital, ipopulation);   
     fin >> state;
    }
    
    cout << "Printing Tree1: \n";
    PrintTreeStateOrder(tree1);
    wait(2);
    
    //////////////////////////////////////
    //2. Add to tree by Capital order (lexographically)
    //////////////////////////////////////
    
    //Resetting the file reading position...
    fin.clear();
    fin.seekg (0, ios::beg);
    
    //filter out header...
    stemp = "";
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
    InsertCapitalOrder(tree2, state, capital, ipopulation);
      
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
         state += stemp;
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
              stemp += capital;
              capital = stemp;
             }
     if(stemp == "Salt")
     {
      fin >> capital;
      stemp += capital;
      fin >> capital;
      stemp += capital;
      capital = stemp;     
     }
     else capital = stemp;
     fin >> spopulation;
     ipopulation = strtoi(spopulation);   
     cout << "Inserting ---> " << state << "       " << capital 
         << "       " << ipopulation 
         << '\n'
         ;                
     InsertCapitalOrder(tree2, state, capital, ipopulation);   
     fin >> state;
    }
    
    cout << "Printing Tree2: \n";
    PrintTreeCapitalOrder(tree2);
    wait(2);

    //////////////////////////////////////
    //3. Add to tree by Population order
    //////////////////////////////////////
    
    //Resetting the file reading position...
    fin.clear();
    fin.seekg (0, ios::beg);
    
    //filter out header...
    stemp = "";
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
    cout << "\n\nInserting ---> " << state << "       " 
         << capital << "       " << ipopulation 
         << '\n'
         ;   
    InsertPopulationOrder(tree3, state, capital, ipopulation);
      
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
         state += stemp;
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
              stemp += capital;
              capital = stemp;
             }
     if(stemp == "Salt")
     {
      fin >> capital;
      stemp += capital;
      fin >> capital;
      stemp += capital;
      capital = stemp;     
     }
     else capital = stemp;
     fin >> spopulation;
     ipopulation = strtoi(spopulation);   
     cout << "Inserting ---> " << state << "       " << capital 
         << "       " << ipopulation 
         << '\n'
         ;                
     InsertPopulationOrder(tree3, state, capital, ipopulation);   
     fin >> state;
    }
    
    cout << "Printing Tree3: \n";
    PrintTreePopulationOrder(tree3);
    wait(2);
    
    cout << "\n\n4. Printing less than 5 Million:\n";
    PrintTreeLess5Mil(tree3);
    cout << "\n\nPrinting more than 10 Million:\n";
    PrintTreeMore10Mil(tree3);

    cout << "Done.\n";


    //EXTRA - adding on Node removal abilities
    cout << "Now entering Tree editing tool. First option: removal.\n"
	 << "Enter the state name of the state you would like to "
	 << "delete: "
      ;
    cin >> stemp;
    tree1.DeleteNode(stemp);
    wait(120);    
    fin.close();

}
