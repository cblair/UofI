/* timer.c - simple example of timing a piece of code */

/* c89 timer.c -o timer */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

main()
{ int i;
  time_t t1,t2;

  (void) time(&t1);
  
  for (i=1;i<=300;++i) printf("%d %d %d\n",i, i*i, i*i*i);
   
  //sleep(2);
  (void) time(&t2);
   
  printf("\nTime to do 300 squares and cubes= %f seconds, %f:%f\n", t2 - t1, t1, t2);
}
