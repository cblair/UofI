#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 1024

void input_cmd(char input[MAX_BUFFER_SIZE])
{
  char c;
  int x = 0;
  do {
    c = getchar();
    //putchar (c);
    input[x] = c;
    x++;
  } while (c != '\n');
  input[x] = '\n';  
}

int main()
{

  char input[MAX_BUFFER_SIZE];
  char *filename;
  char *argv[MAX_BUFFER_SIZE];

  int status = 0;
  while(status != -1)
    {
      printf("?: ");
      
      input_cmd(input);
      
      //parse_cmd(input, filename, argv);
      int x = 0;
      filename = input;
      argv[0] = input;
      while( !(input[x] == ' ' || input[x] == '\n') )
	{
	  x++;
	}
      int y = 1;
      while(input[x] != '\n')
	{
	  if(input[x] == ' ')
	    {
	      input[x] = '\0';
	      x++;
	      argv[y] = &input[x];
	      y++;
	    }
	  else
	    x++;
	}
      input[x] = '\0';
      int argvsize = y + 1;
      y = 0;
      while(y < argvsize)
	{
	  y++;
	}
     
  if(filename[0] == 'e' && filename[1] == 'x' 
	 && filename[2] == 'i' && filename[3] == 't') 
	{status = -1;}
      else
	{
	  int pid = fork();
	  if(pid == 0)
	    {
	      execvp(filename, argv);
	    }
	  else
	    {
	      //parent
	      waitpid(pid, NULL, 0);
	    }
	}
    }
return(0);
}
