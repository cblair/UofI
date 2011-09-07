/*****************************************************
CS 270
Colby Blair
HW 7
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "cbstrings.h"
#include <time.h>

#define MAX_BUFFER_SIZE 512

//tells us the index that we will use for the argument... arguments
int find_arg_element(char **argv, int argc)
{
  int i;
  for(i = 0; i < argc; i++)
    {
      if(str_starts_with(argv[i], "-"))
	return(i);
    }
  return(-1);
}

//return the index of the first argv that is not led by a dash
int find_dir_element(char **argv, int argc)
{
  int i;
  for(i = 1; i < argc; i++)
    {
      if((str_starts_with(argv[i], "-")) == 0)
	return(i);
    }
  return(-1);
}

char *get_dir_name(char **argv, int argc)
{
  //find directory to be ls'ed
  int dir_arg = find_dir_element(argv, argc);

  if(dir_arg == -1)
    return(".");
  return(argv[dir_arg]);
}

int is_elf(char *line)
{
  if(line[1] == 'E' &&
     line[2] == 'L' &&
     line[3] == 'F')
    return(1);
  return(0);
}

int is_dos(FILE *fp)
{
  char line [MAX_BUFFER_SIZE];

  while(fgets(line, sizeof(line), fp ) != NULL)
    {
      int x;
      for(x = 0; x < strlen(line); x++)
	{
	  if((int)line[x] == '\n' && (int)line[x + 1] != 13 )
	    return(0);
	}
    }
  return(1);
}

int is_ascii(FILE *fp)
{
  char line [MAX_BUFFER_SIZE];

  while(fgets(line, sizeof(line), fp ) != NULL)
    {
      int x;
      for(x = 0; x < strlen(line); x++)
	{
	  if((int)line[x] > 128)
	    return(0);
	}
    }
  return(1);
}

int is_o(char *filename)
{
  if(str_index(filename, ".o") != -1)
    return(1);
  return(0);
}

int print_output(char **output, int i, char **argv, int argc)
{
  //stat files
  struct stat buf;
  int exists;

  int x;
  for(x = 0; x < i; x++)
    {
      exists = stat(output[x], &buf);
      if (exists < 0) 
	{
	  fprintf(stderr, "%s not found\n", output[x]);
	} 
      else 
	{
	  ////////////get the argument
	  int arge = find_arg_element(argv, argc);
	  char* ls_arg;
	  ls_arg = (char*)malloc(MAX_BUFFER_SIZE);
	  if(arge != -1)
	    ls_arg = strdup(argv[arge]);
	  else
	    ls_arg = "";

	  /////////////filter arguments

	  // l option
	  char *l_opt;
	  l_opt = (char*)malloc(MAX_BUFFER_SIZE);
	  if(str_index(ls_arg, "l") != -1)
	    {
	      //convert stat's date
	      time_t rawtime = buf.st_mtime;
	      char *date = ctime(&rawtime);
	      //get rid of end newline
	      date[strlen(date) - 1] = '\0';

	      int read, write, execute;
	      read = buf.st_mode & S_IEXEC;
	      read = (int)read;
	      //read = sqrt(read) - 1;

	      sprintf(l_opt, "%4d %d %d %4d %5d %s", 
		      buf.st_mode, buf.st_nlink, buf.st_uid, 
		      buf.st_gid, buf.st_size, date);
	    }
	  else
	    {l_opt = "";}

	  // t option
	  char *t_opt;
	  t_opt = (char*)malloc(MAX_BUFFER_SIZE);
	  if(str_index(ls_arg, "t") != -1)
	    {
	      //convert stat's date
	      time_t rawtime = buf.st_mtime;
	      char *date = ctime(&rawtime);
	      //get rid of end newline
	      date[strlen(date) - 1] = '\0';

	      sprintf(t_opt, "%s", date);
	    }
	  else
	    {t_opt = "";}

	  // f option
	  char *f_opt;
	  f_opt = (char*)malloc(MAX_BUFFER_SIZE);
	  if(str_index(ls_arg, "f") != -1)
	    {
	      //open file and see what its first bits look like
	      FILE *fp;
	      fp = fopen(output[x], "r");
	      char line [MAX_BUFFER_SIZE];
	      	      
	      if(fp != NULL)
		{
		  if(fgets(line, sizeof(line), fp ) != NULL)
		    {
		      //fputs ( line, stdout );
		      //print_ts_str(line);
		      if(is_o(output[x]) == 1)
			f_opt = " - Relocatable .o file";
		      else if(is_elf(line) == 1)
			f_opt = " - ELF File";
		      else if(is_dos(fp) == 1)
			f_opt = " - ASCII File";
		      else if(is_ascii(fp) == 1)
			f_opt = " - ASCII File";
		      else
			f_opt = " - file unknow";
		    }
		  else
		    f_opt = " - directory";
		  
		}
	      
	      fclose(fp);

 	    }
	  else
	    {f_opt = "";}


	  //print formatted text
	  printf("%s %s %10s %s\n", t_opt, l_opt, output[x], f_opt);
	}
    }
  return(0);
}
 
int main(int argc, char *argv[])
{
  DIR *dip;
  struct dirent *dit;
  
  int i = 0;

  if (argc < 2)
    {
      //prevents argv seg faults
      argv[1] = (char*)malloc(sizeof(char*));
      argv[1] = "";
    }

  char *dirname;
  dirname = get_dir_name(argv, argc);

  //open dir
  if ((dip = opendir(dirname)) == NULL)
    {
      perror("opendir");
      return 0;
    }
 
  char **output;
  output = (char**)malloc(sizeof(char**));

  //read dir
  while ((dit = readdir(dip)) != NULL)
    {
      output[i] = (char*)malloc(sizeof(dit->d_name));
      output[i] = strdup(dit->d_name);
      i++;
    }
 
  print_output(output, i, argv, argc);
 
  //close dirpwd
  //produces seg faults!!!!
  /*
    if (closedir(dip) == -1)
    {
      perror("closedir");
      return 0;
    }
  */
 
  return 1;
}

