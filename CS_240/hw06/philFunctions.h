#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generalFunctions.h"
#include <time.h>

#define MAX_EAT_TIMES 25
#define MBS 256 //max buffer size 

//ratio of time thinking / time eating                     
float te;

//name of datafile, set in main                                              
char datafile[MBS];

//semaphore libs                      
#include <sys/ipc.h>
#include <sys/sem.h>

//semaphore struct                                         
union semun {
  int val;               /* used for SETVAL only */
  struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
  ushort *array;         /* used for GETALL and SETALL */
};


//returns the floating p num of the seconds it though
float think()
{
  float seconds = ((float)cbrandom(1000) / 100) * te; //seconds   
  sleep(seconds);
  return(seconds);
}

float hungry(int id, int semid, union semun semset)
{
  int printonce = 0;
  float start_time = time(NULL);
  while((semctl(semid, 0, GETVAL, semset)) <= 0)
    {
      if(printonce == 0)
	{
	  printf("Philosopher %d IS WAITING FOR FIRST FORK.\n", id);
	  printonce++;
	}
    } 
  printonce = 0;

  //consume first fork
  semset.val = semctl(semid, 0, GETVAL, semset) - 1; 
  semctl(semid, 0, SETVAL, semset);
  printf("fork consumed, forks left = %d\n", semctl(semid, 0, GETVAL, semset));

  while((semctl(semid, 0, GETVAL, semset)) <= 0)
    {
      if(printonce == 0)
	{
	  printf("Philosopher %d IS WAITING FOR SECOND FORK.\n", id);
	  printonce++;
	}
    } 
  //consume second fork
  semset.val = semctl(semid, 0, GETVAL, semset) - 1; 
  semctl(semid, 0, SETVAL, semset);
  printf("fork consumed, forks left = %d\n", semctl(semid, 0, GETVAL, semset));

  float end_time = time(NULL);

  float wait_time = end_time - start_time;

  //write our wait time
  char output[MBS];
  sprintf(output, "%d:\t%f\n", id, wait_time);
  write_datafile(datafile, "a", output);

  return(wait_time);
}

float eat(int semid, union semun semset)
{
  //eat a random amout of time.
  float seconds = ((float)cbrandom(1000) / 100); //seconds   
  sleep(seconds);

  //relinquish the forks!
  semset.val = semctl(semid, 0, GETVAL, semset) + 2; 
  semctl(semid, 0, SETVAL, semset);
  printf("fork returned, forks left = %d\n", semctl(semid, 0, GETVAL, semset));
  return(seconds);
}

void philosopher(int id, int semid, union semun semset)
{
  printf("Philosopher %d is starting.\n", id);
  printf("Philosopher %d was thinking for %f seconds.\n", id, think());
  printf("Philosopher %d is hungering.\n", id);
  float wait_time = hungry(id, semid, semset); //one must be hungry before they eat...
  printf("Philosopher %d was waiting for a fork for %f seconds.\n", id, wait_time);
  printf("Philosopher %d was eating for %f seconds.\n", id, eat(semid, semset));

  printf("Philosopher %d is done with one process.\n", id);
  //wait a moment before starting next round
  sleep(1);
}

void philosophers(int nphils, int nforks)
{

  //initialize semaphores       
  key_t key;
  int semid;

  char semfile[MBS];
  sprintf(semfile, "%s.sem", datafile);
  printf("Writing to sem file %s\n", semfile);
  
  key = ftok(semfile, 'E');
  //semid is the id of the set of semaphores we have    
  semid = semget(key, 1, 0666 | IPC_CREAT);

  union semun semset;
  semset.val = nforks;
  //int x; for(x = 0; x < nforks; x++) 
  semctl(semid, 0, SETVAL, semset);
  
  //done intitialize semaphores         

  printf("Test starts with %d\n", semctl(semid, 0, GETVAL, semset));

  int x;
  for(x = 1; x <= nphils; x++)
    {
      int pid = fork();
      if(pid == 0)
	{
	  //child
	  int y;
	  for(y = 0; y < MAX_EAT_TIMES; y++)
	    {
	      philosopher(x, semid, semset);
	    }
	  break;
	}
      else
	{
	  //parent                                                        
	  
	  //wait a moment before spawning another child                   
	  sleep(1);
	}
    }
  
}
