////////////////////////////////////////////////////////////////////
//Class: CS 120 Sect 3
//Date: 11/13/07
//Author: Colby Blair
//Lab 12
////////////////////////////////////////////////////////////////////
//Notes: This is the upgraded version. This version is designed to
// be used as much as possible for a database displaying cutomer
// information. So far, the program has:
// Upgrade 1: seperate print function for the list
// Upgrade 2: Prompts user for new entries to the list
//
//In conclusion, it is worth noting that the final product is definitely not
// a sorted list! It also has some quirks, partially because of the GNU
// compiler, that need revised, as well as some redundancy taking out.
/*
Submission of lab12-1-upgrade.cpp completed on Tue Nov 13 23:20:53 PST 2007
Names of files you have checked in:
  Tue Nov 13 15:32:28 2007    2335 blai4021.lab12-1.cpp
  Tue Nov 13 23:20:53 2007    6074 blai4021.lab12-1-upgrade.cpp
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

void print_current(CustNode *pI); //prints the current node in the program.
void print_list(CustNode *head);  //prints the entire list; we have to make
                                  // sure to pass the function the pointer
                                  // of the first node we want to start from;
                                  // for a full list, this means the 'head'
CustNode* new_entry();            //adds a new entry, returns the pointer
                                  // address to add to the m_next of the 
                                  //existing list

int main()
{
  CustNode *head = NULL;
  char temp[4];

  //Let's creat a few test entries
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
  print_current(pI);

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
  print_current(pI);
  
  //Prompts user for entries into the database, then loops for either a 
  // print of the list, or a new entry to the list, until the user is
  // done.
  //The GNU compiler did not like my cin in this loop, then switching
  // to cin.getling() in the called function, so I had to make this 
  // loop use cin.getline() as well. That lead to some static casting
  // as well... oi. With more time, I would revise this and see if I
  // avoid the cin to cin.getline() logic error in a better way.
  cout << "Please type commands \'new\' to enter a new entry\n" 
       << "or \'list\' to print list, or \'done\' to exit: ";
  cin.getline(temp, 5, '\n');
  while(static_cast<string>(temp) != "done")
  {
    if(static_cast<string>(temp) == "new") 
      pI->m_next = new_entry();
    else if(static_cast<string>(temp) == "list")
      print_list(head);
    else 
      { 
	cout << "Command not recognized. Exiting.\n";
	exit (1); //This has to be here, or the loop cycles forever
	          // due to the nature of cin.getline. 
      }
    cout << "Next command please (\'new\', \'list\', or \'done\'): ";
      cin.getline(temp, 5, '\n');
    cout << '\n';
  }
}

//prints the node that belongs to the address it is passed
void print_current(CustNode *p_pI)
{
  cout << p_pI->m_name << '\n';
  cout << p_pI->m_street << '\n';
  cout << p_pI->m_city << ", ";
  cout << p_pI->m_state << ' ';
  cout << p_pI->m_zip << '\n';
  cout << p_pI->m_number << '\n';
  cout << p_pI->m_note << "\n\n";
}

//prints from the address it recieves on; pass this function the head to 
// print the entire list.
void print_list(CustNode *pl_head)
{
  for(CustNode *pI = pl_head; pI != NULL; pI = pI->m_next)
    print_current(pI);
}

//This function creates a new CustNode with input from the user. It then 
// returns a pointer to the new node, so that the existing database from main()
// can put that address into the m_next at the end of it.
//I realize this is pretty rundundant, but I wanted to move on to other
// upgrade features. It definitely needs optimization, but it is functional.
CustNode* new_entry()
{
  const int MAX_LENGTH = 100;
  char ne_temp[MAX_LENGTH];  //alright: I cheat hear! Since I am not going to make
                      // a seperate function here for inputing multi-word
                      // strings, I set up an array of char.s, and will
                      // use cin.getline to get all of the text up to 100 chars
                      // for each field
  CustNode *ne_pI = new CustNode;
  
  //Now let's enter the new data for our new node...
  cout << "Enter the new name: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_name = ne_temp;
  cout << "Enter the new street: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_street = ne_temp;
  cout << "Enter the new city: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_city = ne_temp;
  cout << "Enter the new state: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_state = ne_temp;
  cout << "Enter the new zip: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_zip = ne_temp;
  cout << "Enter the new number: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_number = ne_temp;
  cout << "Enter the new note: ";
  cin.getline(ne_temp, MAX_LENGTH, '\n');
  ne_pI->m_note = ne_temp;
  //... and finally, set this node's m_next to NULL as it is now the end of 
  // the list.
  ne_pI->m_next = NULL;
  
  return(ne_pI);
}
