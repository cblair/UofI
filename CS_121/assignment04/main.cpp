/*
Implementation: 03/02/08
               Work Time: 4 hours
	       Notes: 
	       A lot of work on passing classes to and from functions,
	       and timing so that the lines are processes correctly.
	       Current Bugs:
	       pop() pop's the end of a queue
	       counter1 reconstructs in every pass to a function,
	       setting the intioned 'global' variable to 0 or whatnot,
	       which ends in only processing of the express line.
*/

#include "bank_queue.h"
#include "process_counter.h"
#include "main_functions.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/*
void move(BQueue &line_from, BQueue &line_to)
{
  //This didn't work. How about this-- just switch 
  // the lines!!!!

  string stemp;
  stemp = line_from.back();
  line_to.push(stemp);
  line_from.pop_back();
}
*/

//This function checks to see if the first queue argument is empty, and if so,
// asks the back of the other 2 lines if they want to move chronologically. The
// first to accept is moved, and the function is exited.
// Should really change name of these queues passed by reference
void check_empty_line(BQueue &pos_empty_line, BQueue &line_from1, BQueue &line_from2)
{
  string name = "";
  bool dep_or_wit = false;
  double amount = 0.0;
  string stemp = "";  

  if(pos_empty_line.empty())
    {
      if(!line_from1.empty()) 
	{
	  cout << line_from1.back() << ", a line has freed up. Would you like to move\n"
	       << "to the empty line? (\'y\' for yes or anything else for no): "
	    ;
	  cin >> stemp;
	  if(stemp[0] == 'y' || stemp[0] == 'Y')
	    {
	      line_from1.get_back(name, dep_or_wit, amount);
	      line_from1.pop_back();
	      pos_empty_line.push(name, dep_or_wit, amount);
	    }
	  else
	    {
	      if(!line_from2.empty()) 
		{
		  cout << line_from2.back() << ", a line has freed up. Would you like to move\n"
		       << "to the empty line? (\'y\' for yes or anything else for no): "
		    ;
		  cin >> stemp;
		  if(stemp[0] == 'y' || stemp[0] == 'Y')
		    {
		      line_from2.get_back(name, dep_or_wit, amount);
		      line_from2.pop_back();
		      pos_empty_line.push(name, dep_or_wit, amount);
		    }
		}
	    }
	}
    }
}
void TS()
{
  /*
  q1.get_back(name, dep_or_wit, amount);
  
  cout << "TS: q1 back info - name - " << name << ", Withdrawl - " << dep_or_wit << ", amount - " << amount << '\n';
  */
}

int main()
{
  BQueue q1, q2, q3;
  Counter counter1;
  counter1.q1_counter = 0;
  counter1.q2_counter = 0;
  counter1.q2_counter = 0;
  string stemp = "";

  //Test init
  test_init(q1, q2, q3);
  q1.print_node(1);
  //

  cout << "Welcome to the ABC Bank Queue System. Please enter your name: ";
  string sname;
  cin >> sname;
  
  while(sname != "exit")
    {
      if(sname != "NA" && sname != "na")
	{
	  cout << "Would you like to make a Withdrawal or Deposit?: ";
	  string saction;
	  cin >> saction;
	  cout << "For what amount please?: ";
	  double damount;
	  cin >> damount;

	  BQueue shortest = shortest_line(q1, q2, q3);
	  input(sname, saction, damount, shortest);
	}
      
      cout << "Here are the current processing lines:\n";
      print_all(q1, q2, q3);

      while( process_transactions(q1, q2, q3, counter1) );
      cout << "Customer processed. Line now looks like this:\n"; 
      print_all(q1, q2, q3);

      check_empty_line(q1, q2, q3);
      check_empty_line(q2, q3, q1);
      check_empty_line(q3, q1, q2);

      cout << "Welcome to the ABC Bank Queue System. Please enter your name, " 
	   << "or type \'NA\' to resume back processing: ";
      cin >> sname;
    }
}
