/////////////////////////////////////////////////
//Class: CS 120 Sect. 3
//Date: 11-27-07
//Author: Colby Blair
//Lab 13-1
/////////////////////////////////////////////////
/*
Saving old lab13-1.cpp as lab13-1.cpp.bak
Submission of lab13-1.cpp completed on Thu Nov 29 20:12:07 PST 2007
Names of files you have checked in:
  Thu Nov 29 20:12:07 2007    7735 blai4021.lab13-1.cpp
  Thu Nov 29 20:10:42 2007    8345 blai4021.lab13-1.cpp.bak
End of list
*/
#include <iostream>

using namespace std;

class IntStack
{
private:
  struct Node
  {
    int m_v;
    Node *m_next;
  };
public:
  Node *head, *pI, *pTail;
  IntStack() :head(NULL), pI(NULL), pTail(NULL) {}

  //Returns false if head is NULL, true otherwise
  bool empty()
  {
    if(head == NULL) return(true);
    else return(false);
  }

  //Prints an error and returns 0 if the stack is empty, returns m_v of head otherwise.
  int front()
  {
    if( empty() ) 
      { 
	cerr << "ERROR: IntStack::front() detected an attempt to "
	     << "access a value on an empty stack.\n";
	return(0);
      }
    return(head->m_v);
  }

  //Creates a new Node, sets its m_next to head,
  // and then head to the new Node
  int push_front(int v)
  {
    pI = head;
    head = new Node;
    head->m_next = pI;
    head->m_v = v;
    
    int count = 0;
    for(pI = head; pI != NULL; pI = pI->m_next) ++count;

    return(count);
  }

  bool pop_front()
  {
    if( empty() ) return(false);

    pI = head->m_next;
    delete head;
    head = NULL;
    head = pI;
    return(true);
  }

  //Just loops pop() until empty returns a true, indicating 
  // the stack is empty.
  void clear()
  {
    //TODO: I want to explore not only deleting entire
    // dynamically allocated classes not only here,
    // but using it for recursion. This would be more
    // efficient here.
    //TODO: try while(!pop());

    while( !empty() ) pop_front();
  }
  
  //TODO: get inline and print() back into the class dec.
  void print(ostream &out = cout);
};

inline
ostream& operator<<(ostream &out, IntStack &b)
{
  b.print(out);
  return(out);
}

//If empty() is true, then cout's *empty*, otherwise
// cout's *top, the m_v of each node with a newline,
// and then bottom.
void IntStack::print(ostream &out)
{
  if( empty() ) out << "*empty*\n";
  else 
    {
      out << "*top*\n";
      for(pI = head; pI!= NULL; pI = pI->m_next) 
	out << pI->m_v << '\n';
      out << "*bottom*\n";
    }
}

int main()
{
  cout << "--------------------------------------\n"
       << "Lab 13-1: Stack\nAuthor: Colby Blair\n"
       << "--------------------------------------\n" ;

  IntStack stack1, stack2;

  //testing emtpy()
  cout << "Stack 1 ";
  if( stack1.empty() ) cout << "is empty.\n";
  else cout << "contains data.\n";
  cout << "Stack 2 ";
  if( stack2.empty() ) cout << "is empty.\n";
  else cout << "contains data.\n";

  //testing push_front()
  cout << "Please enter a single integer in which to "
       <<"add to the top of Stack 1: ";
  int itemp;
  cin >> itemp;
  stack1.push_front(itemp);
  cout << "Please enter a single integer in which to "
       <<"add to the top of Stack 2: ";
  cin >> itemp;
  stack2.push_front(itemp);

  //testing print()
  cout << "The contents of Stack 1 are:\n";
  stack1.print();
  cout << "\nAnd the contents of Stack 2 are:\n";
  stack2.print();

  //Now performing specific tests required by the lab.
  stack1.clear();
  cout << "Now performing 12321 for-loop test.\n";
  //Pushes 1-3 in new nodes, then 4-5 in new nodes.
  for(itemp = 1; itemp <= 5; ++itemp)
    {
      if(itemp <= 3)
	{
	  stack1.push_front(itemp);
	  stack1.print();
	}
      else
	{
	  stack1.push_front(6 - itemp);
	  stack1.print();
	}
    }
  cout << "Performing print-pop-print-pop test.\n";
  stack1.print();
  stack1.pop_front();
  stack1.print();
  stack1.pop_front();
  cout << "Now pushing on 4, 5, and 6.\n";
  for(itemp = 4; itemp <= 6; ++itemp) stack1.push_front(itemp);
  stack1.print();
  cout << "Now clearing and printing.\n";
  stack1.clear();
  stack1.print();

  //Now we had the user 'full' control.
  // Starting with Stack 1 manipulations.
  //used for the user input commands, stemp's default is 'help'
  // so we display the possible commands first thing in the while
  // loop.
  string stemp = "help"; 
  cout << "Now we are going to dictate the 2 stacks "
       << "to add, remove, or print values to the "
       << "stacks. First is Stack 1.\n";
  //While the user does not command an exit, this while loop does 
  // exactly what the help if-then statements displays
  while(stemp != "exit")
    {
      if(stemp == "help")
	{
	  cout << "\'help\' = command list\n"
	       << "\'push'\ = add a value to the "
	       << "front of the stack\n"
	       << "\'pop\' = remove the value at the "
	       << "front of the stack\n"
	       << "\'front\' = print the value at the front of "
	       << "the stack.\n"
	       << "\'print\' = print the stack\n"
	       << "\'clear\' = clear the entire stack\n";
	}
      else if(stemp == "push")
	{
	  cout << "Number to push on stack: ";
	  cin >> itemp;
	  stack1.push_front(itemp);
	  cout << "Stack 1 is now:\n";
	  stack1.print();
	}
      else if(stemp == "pop")
	{
	  if( stack1.pop_front() ) cout << "Stack 1 has been popped. "
					<< "The stack now is:\n";
	  else cerr << "ERROR: int main() detected a "
		    << "pop on an empty stack.\n";
	  stack1.print();
	} 
      else if(stemp == "front")
	{
	  cout << "The value on the front of Stack 1 is " << stack1.front()
	       << ".\n"; 
	}
      else if(stemp == "print") stack1.print();
      else if(stemp == "clear") 
	{
	  stack1.clear();
	  cout << "Stack 1 has been cleared; contents are now:\n";
	  stack1.print();
	}
      //Just in case the input is unrecognizable...
      else cout << "Sorry, \'" << stemp << "\' is not a recognized command. "
		<< "For a list of commands, type \'help\'.\n"; 
      //We want to make sure to prompt the user again after every input.
      cout << "\nWhich action would you like to do next for Stack 1?: ";
      //..and evaluate the new command.
      cin >> stemp;
    }
  // And now for Stack 2 manipulations.
  // First, lets see if we can copy Stack 1 to Stack 2 and muck things up
  // a bit.
  cout << "Now, for testing purposes, copying Stack 1 to Stack 2.\n";
  stack2 = stack1;
  stemp = "help";
  cout << "Now Stack 2.\n";
  //This while is the same as Stack 1's.
  while(stemp != "exit")
    {
      if(stemp == "help")
	{
	  cout << "\'help\' = command list\n"
	       << "\'push'\ = add a value to the "
	       << "front of the stack\n"
	       << "\'pop\' = remove the value at the "
	       << "front of the stack\n"
	       << "\'front\' = print the value at the front of "
	       << "the stack.\n"
	       << "\'print\' = print the stack\n"
	       << "\'clear\' = clear the entire stack\n";
	}
      else if(stemp == "push")
	{
	  cout << "Number to push on stack: ";
	  cin >> itemp;
	  stack2.push_front(itemp);
	  cout << "Stack 2 is now:\n";
	  stack2.print();
	}
      else if(stemp == "pop")
	{
	  if( stack2.pop_front() ) cout << "Stack 2 has been popped. "
					<< "The stack now is:\n";
	  else cerr << "ERROR: int main() detected a "
		    << "pop on an empty stack.\n";
	  stack2.print();
	} 
      else if(stemp == "front")
	{
	  cout << "The value on the front of Stack 2 is " << stack2.front()
	       << ".\n"; 
	}
      else if(stemp == "print") stack2.print();
      else if(stemp == "clear") 
	{
	  stack2.clear();
	  cout << "Stack 2 has been cleared; contents are now:\n";
	  stack2.print();
	}
      else cout << "Sorry, \'" << stemp << "\' is not a recognized command. "
		<< "For a list of commands, type \'help\'.\n"; 
      cout << "\nWhich action would you like to do next for Stack 2?: ";
      cin >> stemp;
    }
  //Now printing both stacks to see what we have done...
  cout << "Stack 1:\n";
  stack1.print();
  cout << "Stack 2:\n";
  stack2.print();
  //Everything seems to have come out fine here, no problems with copying.
}
