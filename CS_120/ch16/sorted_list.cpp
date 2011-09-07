//Notes: in the Customer class, the pointer called pI is used to move through
// the list for almost every class function. Therefore, when you decide to use it,
// you should always reset it to head first, then move it where you want, like to the
// end. Otherwise, another function may have it set to NULL, and then there you
// go dereferencing it. There should probably be a pI reset function, this may come
// depending on need. You can see an example of this in Customer::print_current();
// pI will be set to the last entry you made, but as soon as you use another function
// like Customer::print_entire(), the side affect from that function is pI == NULL.

#include <iostream>

using namespace std;

class Customer
{
private:
  struct Node
    {
    public:
      string m_name;
      string m_street;
      string m_city;
      string m_state;
      int m_zip;
      string m_number;
      string m_note;
      Node *m_next;
    };
  public:
  Node *head, *pI, *pCI, *pCTail;
  Customer() : head(NULL), pI(NULL), pCI(NULL), pCTail(NULL) {}
  void new_node()
  {
    //empty list condition
    if(head == NULL) 
      {
	head = new Node; 
	head->m_next = NULL;
	pI = head;
      }
    //1 element list condition
    else if(head->m_next == NULL) 
      {
        head->m_next = new Node; 
        pI = head->m_next;
        pI->m_next = NULL;
      }
    //2 or more element list condition
    else 
      {
	//This first section resets pI to the last node in the list
	pI = head;
	while(pI->m_next != NULL) pI = pI->m_next;
	//Now, we set m_next to a newly created Node, and then set pI to it.
        pI->m_next = new Node;
        pI = pI->m_next;
        pI->m_next = NULL;
      }
  }

  void insert_info(string ii_name, string ii_street, string ii_city, string ii_state, 
		   int ii_zip, string ii_number, string ii_note)
  {
    //This error condition will only occur if the programmer outside the class
    // tries to insert info with an empty list. Watch out! If this is used with a 
    // normal list, it will overwrite the last node. Hmm...
    //TODO: If this is used without creating a new node on a non-empty list,
    // there needs to be some sort of protection for that node.
    if(head == NULL) 
      {
	cerr << "ERROR: Customer::insert_info() detected an attempt of insertion "
	     << "of data without the creation of a new node.\n";
	return;
      }
    //Standard pI routine to put it back on the last node of the list.
    pI = head;
    while(pI->m_next != NULL) pI = pI->m_next;

    pI->m_name = ii_name;
    pI->m_street = ii_street;
    pI->m_city = ii_city;
    pI->m_state = ii_state;
    pI->m_zip = ii_zip;
    pI->m_number = ii_number;
    pI->m_note = ii_note;
  }

  void print_current()
  {
    if(head == NULL) return;
    if(pI == NULL)
      {
	cout << "Sorry, another command has erased the reference to whichever "
	     << "entry was current. The current entry is now default 0.\n";
	return;
      }
    cout << pI->m_name << '\n';
    cout << pI->m_street << '\n';
    cout << pI->m_city << ", " << pI->m_state << ' ' << pI->m_zip << '\n';
    cout << pI->m_number << '\n';
    cout << pI->m_note << '\n';
  }

  bool print_entire()
  {
    //Resets pI to head, and then tranverses through the list.
    for(pI = head; pI != NULL; pI = pI->m_next) print_current(); 
    return(head != NULL);
  }

  int name_sort()
  {
    //empty list condition
    if(head == NULL) return(1);
    //1 element list condition
    else if(head->m_next = NULL) return(2);
    //2 or more element list condition
    else
      { 
	//Switches the first 2 if they are out of order.
	//TS:
	cout << "TS: Comparing head elements\n";
	if(head->m_name > head->m_next->m_name)
	  {
	    cout << "TS: setting pCI = head->m_next\n";
	    pCI = head->m_next;
	    cout << "TS:setting pCTail to head\n";
	    pCTail = head;
	    cout << "TS: setting PCTail->m_next to pCI->m_next\n";
	    pCTail->m_next = pCI->m_next;
	    cout << "TS: setting head t0 pCI\n";
	    head = pCI;
	    cout << "TS: setting head->m_next to pCTail\n";
	    head->m_next = pCTail;
	  }
	//from here, we are going to set pI on the current node we are comparing,
	// and send...
	return(3);
      }
  }
};

//See the definitions below for operation descriptions
void insert(Customer &db1);
void print_commands();

int main()
{
  Customer db1;
  
  cout << " ---------------------------------------\n"
       << "|         Customer Test Database        |\n"
       << " ---------------------------------------\n";
  
  cout << "Welcome to the database; which actions would you like to activate?\n"
       << "(\'help\' for the list of commands): ";
  string stemp;
  bool btemp;
  cin >> stemp;
  while(stemp != "exit")
    {
      if(stemp == "new") insert(db1);
      else if(stemp == "lprint") 
	{
	  btemp = db1.print_entire();
	  if(btemp == false) cout << "The list is empty\n";
	}
      else if(stemp == "cprint") db1.print_current();
      else if(stemp == "help") print_commands();
      else if(stemp == "namesort") db1.name_sort();
      else cout << "Sorry, that command was not recognized. Type \'help\' for "
		<< "a list of commands.\n";
      cout << "\nWhich action would you like to perform now?: ";
      cin >> stemp;
    }
}

//The Class variable in main(db1, for example) has to be passed by reference, or
// this function will recreate it, and the 2 will never really make anything!
void insert(Customer &db1)
{
  db1.new_node();

  const int FIELD_SIZE = 50;
  char i_name[FIELD_SIZE];
  char i_street[FIELD_SIZE];
  char i_city[FIELD_SIZE];
  char i_state[FIELD_SIZE];
  int i_zip;
  char i_number[FIELD_SIZE];
  char i_note[5 * FIELD_SIZE];

  //cin.ignore is used here to avoid the issue we have from hanging on the input when
  // switching from cin to cin.getline()
  cin.ignore(1, '\n');
  cout << "Please, enter the name of the new Customer: ";
  cin.getline(i_name, FIELD_SIZE);
  cout << "Enter the street and location number: ";
  cin.getline(i_street, FIELD_SIZE);
  cout << "Enter the city: ";
  cin.getline(i_city, FIELD_SIZE);
  cout << "Enter the state: ";
  cin.getline(i_state, FIELD_SIZE);
  cout << "Enter the zip code: ";
  cin >> i_zip;
  cin.ignore(1, '\n'); 
  cout << "Enter the telephone number: ";
  cin.getline(i_number, FIELD_SIZE);
  cout << "Enter any notes about the new customer:\n";
  cin.getline(i_note, (5 * FIELD_SIZE) );

  db1.insert_info(i_name, i_street, i_city, i_state, i_zip, i_number, i_note);
  
  cout << "The new entry is as follows: \n"
       << "----------------------------\n";
  db1.print_current();
}

//This function may need to be reinserted to the code where it is only used once.
void print_commands()
{
  cout << "\'new\' = new customer entry.\n"
       << "\'lprint\' = print the entire list.\n"
       << "\'cprint\' = print the current \\ latest entry.\n"
       << "\'namesort'\ = sorts the list alphabetical order.\n"
       << "\'help\' = list the commands.\n"
       << "\'exit\' = exit the program.\n";
}
