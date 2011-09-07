/* lab02.cpp
 *
 * CS 121.Bolden.........GNU Version 2...........Colby Blair
 * 01/22/08 .....Dell Inspiron 1521, AMD Turion 64....cblair@vandals.uidaho.edu
 *
 * This program computes basic math functions through the command line. The 
 * arithmetic functions are invoked by using the 'calc' argument, followed
 * by the arithmatic ( 2 + 2 )
 * Notes:
 * 01/19/08 15:00 Begining Implementation:
 *  The biggest time consumer here was manipulating argv, passing to functions,
 *  etc. without seg faults. The best way was to add in a minimum argument
 *  function that would not allow the 'lab02' command without arguments.
 *  The other, almost more time consuming issue was determining that for char
 *  purposes, argv can actually be treated like a 2 dimensional array.
 *  This is essential when using argv directly in if statements.
 * 01/19/08 18:00 Ending Implementation
 * 01/22/08 08:30 Beginging Implementation:
 *  Mostly double checking the error handlers and trying to break the program
 *  with the input. Seems solid! I will implement this as a class if I get 
 *  a chance, there would be no reason not to.
 * 01/22/08 09:30 Ending Implementation                
 *---------------------------------------------------------------------
 */

#include <iostream>

void min_arg_check(int &argc);
void calc(int &argc, char *argv[]);
void error_check(int &argc, char *argv[]);
bool is_calc(int &argc, char *argv[]);
void troubleshooter(int &argc, char *argv[]);
void help();

using namespace std;

int main(int argc, char *argv[])
{
  //This has to be checked first to avoid dereferencing argv[] as a null;
  // even before the error_check() call.
  min_arg_check(argc);

  //Gives the usage cout if there is no acceptable arguments
  if(!is_calc(argc, argv)) help();

  // error_check() here is not necessary; all other functions I've written will 
  // do this if they are dealing with argc and argv[], but I put it here for
  //completeness in case other functions / error checking needs are inserted.    
  error_check(argc, argv);
  if(is_calc(argc, argv)) calc(argc, argv);
}

//If argc is less than 2, we'll get a seg fault on argv if we try to pass it
//to any functions. This has to come first.
void min_arg_check(int &argc)
{
   if(argc < 2)
     {
       cerr << "ERROR::min_arg_check(): The minumum amount of arguments were "
	    << "not entered.\nExiting...\n"
	 ;
       exit(1);
     }

}

//Checks for all input errors except for the minimum amount of arguments 
void error_check(int &argc, char *argv[])
{
 //This case is for if a 'calc' argument is entered, but an entire 
 //artithmatic operation has not been entered. This would otherwise
 //result in a runtime error.
 if(is_calc(argc, argv) && argc < 5)
 {
  cerr << "ERROR: The argument \'calc\' was entered, but not enough "
       << "arithmetic information was entered to process an execution. "
       << "Exiting...\n"
       ;
  exit(1);              
 }
 
 //Division by zero checker; we have to use the temp int var, or for some reason,
 //sometimes in runtime, we'll get seg faults.
 //This could still be a place of suspecion for seg faults.
 int itemp = atoi(argv[4]);
 if(itemp == 0)
   {
     cerr << "ERROR::error_check(): Division by zero detected! Exiting...\n";
     exit(1);
   }
}

//If the calc argument is passed, then we enter the math phase of the program
void calc(int &argc, char *argv[])
{
  error_check(argc, argv); 
 
 //if the argument from the command line starts with 'calc', we then perform 
 //some arithmatic.    
  
  // This gives us a few int variable from the arguments to do manipulations
  //on            
  int x = atoi(argv[2]);
  int y = atoi(argv[4]);
  switch(argv[3][0])
  {
                    case '*':
                         cout << x << ' ' << argv[3] << ' ' << y << " --> "
                              << x * y << '\n'
                             ;
                         break;
                    case '/':
                         cout << x << ' ' << argv[3] << ' ' << y << " --> "
                              << x / y << '\n'
                             ;
                         break;
                    case '+':
                         cout << x << ' ' << argv[3] << ' ' << y << " --> "
                              << x + y << '\n'
                             ;
                         break;
                    case '-':
                         cout << x << ' ' << argv[3] << ' ' << y << " --> "
                              << x - y << '\n'
                             ;
                         break;
  default:
    help();
    break;
  } 
}

//Returns true if a 'calc' argument can be found in argv[1]
bool is_calc(int &argc, char *argv[])
{
  //error_check(argc, argv);     
 if(argv[1][0] == 'c' 
               && argv[1][1] == 'a' 
               && argv[1][2] == 'l'
               && argv[1][3] == 'c' )
                  return(true);
 return(false);
}

//I've left this in, because it has been a great way to find out what the input
//looks like in the program. This has been a fast way to find out what is working
//and why things are breaking.
void troubleshooter(int &argc, char *argv[])
{
 cout << "is_calc() = " << is_calc(argc, argv) << '\n';
 cout << "TS: argc = " << argc << '\n';
 for(int i = 0; i < argc; ++i) cout << "argv[" << i << "] = " 
                                    << argv[i] << '\n'
                                    ;
}

void help()
{
  cout << "Usage: lab02 <argument>\n"
       << "calc <integer> <operator> <nonzero integer>\n"
       << "                   '*' - The multiplication operator needs '' "
       << "so the shell does not use it as a wildcard symbol \n"
       << "                   / \n"
       << "                   + \n"
       << "                   - \n"
    ;
  exit(1);
}
