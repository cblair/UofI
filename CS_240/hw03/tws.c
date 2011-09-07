#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tws.h"

char *PATH;

int main()
{
  //internal variables
  char *input = (char *)malloc(MAX_BUFFER_SIZE);
  char *filename;
  char *argv[MAX_BUFFER_SIZE];

  //allocates memory for the history and alias arrays,
  //and initializes any other global variables.
  init_shell();

  //reads in input from .twsrc and sets environmental
  //variables listed there
  set_env();

  //if 'exit' is inputed, status is set to -1, and tws
  //terminates
  int status = 0;
  while(status != -1)
    {
      //get input from user
      input = input_cmd();

      //seperate input string into the filename only
      //in the input string, and the rest of the option
      //in argv, space delimited
      filename = parse_cmd(input, filename, argv);

      if(exit_inputed(filename))
	{status = -1;}      
      //else if(internal_cmd_exe(filename, argv))
      //{ /*internal command is executed in func above*/ }
      
      if(!exit_inputed(filename))
	{
	  if(process_cmd(filename, argv)) {break;}
	}

      //resets argv memory space for another go around
      clear_argv(argv);
    }
return(0);
}
