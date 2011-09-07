/*
Implementation time: 
02/09/08
19:00 - 19:30
02/11/08
13:20 - 14:20
02/12/08
10:30 - 10:50
02/18/08
20:00 - 23:00
  Notes: A lot of time here, most of the implemenation so far. This was
         the bulk of getting user input cleared up and set up nice for
         inserting into nodes later. New tricks worth mentioning:
	 -isdigit
	 -streamstring's for string to number convertion (check out
	  the s_to_i() function
02/19/08
20:00 - 23:50
  Notes: Most of this time was spent remaking the stack class, and getting it 
         to work correctly with my app. It was a bit of reinventing the wheel,
	 but it was time well spent reinforcing my small scope of the 
	 language.
*/

#include "pcalc.h"
#include "pcalcim.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

int main()
{
  P_Calc calc1;

  cout << "Welcome to the Polish Calculator. Please enter a command\n"
       << "(type \'help\' for a list of commands):\n"
      ;
  
  while(!calc1.quit())
    {
      cout << "RPN ";
      if(calc1.is_empty()) cout << "(empty) ";
      else cout << calc1.peek();
      cout << " > ";
      while(!calc1.perform_op() ) 
	calc1.input();
    }
  cout << "Stack contents: ";
  calc1.print_stack();
  cout << '\n';
}
