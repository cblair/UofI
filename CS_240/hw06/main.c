#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "philFunctions.h"


int main(int argc, char **argv)
{
  if(argc != 4)
    {
      printf("Usage: main <# of forks> <# of philosophers> <# t/e ration (thinking time to eating time)>\n");
      exit(0);
    }

  //initalize program parameters
  int nforks = atoi(argv[1]);
  int nphils = atoi(argv[2]);
  te = (float)atoi(argv[3]);

  sprintf(datafile, "data/wait_time-n%d-te%f.dat", nphils, te);
  write_datafile(datafile, "w", "");

  printf("Test started with %d forks, %d philosophers, and a t/e ratio of %f to datafile '%s'.\n", nforks, nphils, te, datafile);

  if(nforks < 2 * nphils)
    {
      printf("WARNING: The resources and consumers you defined may cause deadlock. Continue? (y/n): ");

      char buffer[MBS];
      scanf("%s\n", buffer);

      //flush input buffer

      while(buffer[0] != 'y' && buffer[0] != 'n')
	printf("Sorry, that input was not recongnized (y/n): ");
      if(buffer[0] == 'n')
	exit(0);
    }

  philosophers(nphils, nforks);

  return(0);
}
