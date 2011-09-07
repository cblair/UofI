#include <time.h>

long int cbrandom(int mod)
{
  //get raw time in seconds
  //time_t result;
  srand(time(NULL));
  return(rand() % mod);
}

void write_datafile(char *name, char *mode, char *data)
{
  FILE *fp;
  fp = fopen(name, mode);
  fprintf(fp, data, mode);
  fclose(fp);
}

/*
***Not used anymore. Might be useful later***

char *get_sleep_cmd(float time)
{
  char *cseconds;
  cseconds = (char*)malloc(MAX_BUFFER_SIZE);
  sprintf(cseconds, "%f", time);

  char *sleep = "sleep ";
  char *cmd;
  cmd = (char*)malloc( (strlen(sleep) + strlen(cseconds)) * sizeof(char*));
  strcat(cmd, sleep);
  strcat(cmd, cseconds);

  return(cmd);
}

void sleep(float time)
{
  char *cmd;
  cmd = get_sleep_cmd(time);
  system(cmd);
}
*/

