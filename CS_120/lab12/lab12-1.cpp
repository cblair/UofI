////////////////////////////////////////////////////////////////////
//Class: CS 120 Sect 3
//Date: 11/13/07
//Author: Colby Blair
//Lab 12
////////////////////////////////////////////////////////////////////
//Notes: this source is to show the minimal code to emphasize function
// flow. To see the upgraded and useful source, see the checked file
// 'lab12-1-upgrade. The upgrade source has all the useful features,
// like an input prompt and print function.

/*Submission of lab12-1.cpp completed on Tue Nov 13 15:32:28 PST 2007
Names of files you have checked in:
  Tue Nov 13 15:32:28 2007    2335 blai4021.lab12-1.cpp
End of list
*/

#include <iostream>

using namespace std;

//records all the customers information
struct CustNode
{
public:
  string m_name;
  string m_street;
  string m_city;
  string m_state;
  string m_number;
  string m_zip;
  string m_note;
  CustNode *m_next;
};

int main()
{
  CustNode *head = NULL;
  
  //creating a customer entry for Colby Blair
  CustNode *pI = new CustNode;
  pI->m_name = "Colby Blair";
  pI->m_street = "843 Travois Way";
  pI->m_city = "Moscow";
  pI->m_state = "ID";
  pI->m_zip = "83843";
  pI->m_number = "208.631.1059";
  pI->m_note = "Pays yearly, so far, no issues.";
  pI->m_next = NULL;

  //setting the list head to the entry for Colby Blair:
  //Colby is now in the list as the 1st and only 
  head = pI;

  //printing out entry for Colby
  cout << pI->m_name << '\n';
  cout << pI->m_street << '\n';
  cout << pI->m_city << ", ";
  cout << pI->m_state << ' ';
  cout << pI->m_zip << '\n';
  cout << pI->m_number << '\n';
  cout << pI->m_note << '\n';

  cout << '\n';

  //making new entry for Jim Bob
  pI = new CustNode;
  pI->m_name = "Jim Bob";
  pI->m_street = "1234 Wrong Way";
  pI->m_city = "Lost";
  pI->m_state = "WY";
  pI->m_zip = "00000";
  pI->m_number = "800.1HELPME";
  pI->m_note = "We could not find this customer in the physical world.";
  pI->m_next = NULL;

  //We now set the m_next address in the first element of the list,
  // the entry for Colby Blair, to Jim Bob's.
  head->m_next = pI;
  //Now Jim Bob is official part of the list.

  //Now let's print out for Jim Bob
  cout << pI->m_name << '\n';
  cout << pI->m_street << '\n';
  cout << pI->m_city << ", ";
  cout << pI->m_state << ' ';
  cout << pI->m_zip << '\n';
  cout << pI->m_number << '\n';
  cout << pI->m_note << '\n';

  //This is where I have stopped consideration for functionality to this code,
  // and will be focusing on the upgraded source.
}
