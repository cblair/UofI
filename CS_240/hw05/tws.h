#define MAX_BUFFER_SIZE 5000
#include <string.h>
#include <dirent.h>
#include "cbstrings.h"

//history vars
char **history;
int h_n;

//alias vars
char **alias;
int a_n;

//tws directory and executables vars
char **tws;
int t_n;
int execute_tws;

void init_shell()
{
  history = (char **)malloc(sizeof(char **));
  alias = (char **)malloc(sizeof(char **));
  tws = (char **)malloc(sizeof(char **));
}

int argv_len(char **argv)
{
  int x;
  for(x = 0; argv[x] != NULL; x++)
    {;}
  return(x);
}

//replace any word in filename and argv starting with '$' with
//the equivalent env var val
void str_replace_var_ref(char *filename, char **argv)
{
  char *var_exists;
  if(str_starts_with(filename, "$"))
    {
      var_exists = getenv(filename + 1);
      if(var_exists)
        filename = strcpy(filename, getenv(filename + 1));
      else
        filename[0] = '\0';
    }
  
  int x;
  for(x = 0; x < argv_len(argv); x++)
    {
      int i;
      if( (i = str_index(argv[x], "$")) != -1)
        {
          char *substr;
          char *newstr;
          get_str_from_to(argv[x], "$", ":", &substr);
          if(strcmp(substr, "") == 0)
            {
              get_str_from_to_end(argv[x], "$", &substr);
              var_exists = getenv(substr);
              if(var_exists)
                {
                  newstr = (char*)malloc(sizeof(substr) + sizeof("$") + sizeof(var_exists) + sizeof(argv[x]));
                  newstr = str_cpy("$", substr);
                  //printf("TS283: newstr = %s, argv[%d] = %s, var_exists = %s\n", newstr, x, argv[x], var_exists);
		  newstr = str_replace(newstr, var_exists, argv[x]);
		  //printf("TS287: argv[x] = %s, newstr = %s\n", argv[x], newstr);
		  //argv[x] = str_cpy(newstr, "");
		  strcpy(argv[x], newstr);
		  //printf("TS289: argv[x] = %s, newstr = %s\n", argv[x], newstr);
		}
	      else
		argv[x] = "";
	      
	      //printf("TS284: %s, %s\n", argv[x], newstr);
	    }
	  else
	    {
	      char *rstr;
	      rstr = strdup("");
	      rstr = str_cpy("$", substr);
	      //rstr = str_cpy(rstr, ":");
	      var_exists = getenv(substr);
	      if(var_exists)
		{
		  //printf("TS305: rstr = %s, var_exists = %s, argv[x] = %s\n", rstr, var_exists, argv[x]);
		  newstr = (char*)malloc(sizeof(rstr) + sizeof(var_exists) + sizeof(argv[x]));
		  newstr = str_replace2(argv[x], rstr, var_exists);
		  //str_replace(rstr, var_exists, argv[x]);
		  //printf("TS314: newstr = %s\n", newstr);
		  //argv[x] = (char*)malloc(sizeof(newstr));
		  strcpy(argv[x], newstr);
		  //printf("TS284: argv[x] = %s, newstr = %s\n", argv[x], newstr);
		}
	      else
		argv[x] = "";
	    }     
	}
    }
}

int tws_exe_exists(char *filename)
{
  int ret_val = 0;

  DIR *dir;
  dir = (DIR*)malloc(sizeof(DIR*));

  struct dirent *dp;

  //tws dir, has to be a env var
  const char *dir_path = getenv("TWSDIR");

  if(dir_path == NULL)
    return(0);

  //printf("TS108:\n");
  
  if((dir = opendir(dir_path)) == NULL)
    {
      //printf("ERROR: could not open tws directory.\n");
      return(0);
    }

  while ((dp = readdir(dir)) != NULL) {
    if(strcmp(dp->d_name, filename) == 0)
      {
	printf("TS126:\n");
	ret_val = 1;
      }
  }

  /*
  if(closedir(dir) == -1)
    {
      perror("closedir");
    }
  */
  
  //printf("TS131:\n");

  return(ret_val);
}

void process_env(char *cmd)
{
  char *varname;
  char *arg;

  //set env var
  get_str_before(cmd, "=", &varname);
  get_str_after(cmd, "=", &arg);
  //printf("TS92: varname = %s, arg = %s\n", varname,  arg);

  char *test = getenv(varname);
  int replace;
  if(test == NULL)
    replace = 0;
  else replace = 1;
  setenv(varname, arg, replace);

}

int alias_exists(char *astr)
{
  int x;
  char *apotname;
  char *aexname;
  
  //if astr has an equal sign, copy string. Otherwise, assign
  //string. Change the first newline to a term char
  if(str_index(astr, "=") != -1)
    get_str_before(astr, "=", &apotname);
  else
    {
      int i;
      if( (i = str_index(astr, "\n")) != -1 )
	  astr[i] = '\0';
      apotname = astr;
    }

  for(x = 0; x < a_n; x++)
    {
      get_str_before(alias[x], "=", &aexname);
      if(strcmp(apotname, aexname) == 0)
	return(x);
    }
  return(-1);
}

/*
char *alias_element(char *test)
{}
*/

void print_aliases(char **argv)
{
  int x;

  for(x = 0; x < a_n; x++)
    {
      printf("alias: %s\n", alias[x]);
    }
}

char *process_alias_cmd(char *input)
{
  int i;
  if((i = alias_exists(input)) != -1)
    {
      //printf("TS396: alias exists. i = %d, input = %s, alias[%d] = %s\n", i, input, i, alias[i]);
      char *cmd;
      cmd = str_cpy(alias[i], "");

      //do single or double quote mode
      if(str_index(cmd, "'") != -1)
	{
	  //printf("TS196: single quote mode\n");
	  get_str_after(cmd, "'", &cmd);
	  get_str_before(cmd, "'", &cmd);
	  cmd[strlen(cmd) - 1] = '\0';
	  //printf("TS404: cmd = %s\n", cmd);
	  //print_ts_str(cmd);
	  //return(str_cpy(cmd, "\n"));
	  return(cmd);
	}
      else
	{
	  //printf("TS196: double quote mode\n");
	  get_str_after(cmd, "\"", &cmd);
	  get_str_before(cmd, "\"", &cmd);
	  //printf("TS404: cmd = %s\n", cmd);
	  //print_ts_str(cmd);
	  cmd[strlen(cmd) - 1] = '\0';
	  //return(str_cpy(cmd, "\n"));
	  return(cmd);
	}
    }
 return(input);
}

//changes cmd to the element in history[i], unless it doesn't exist
char *process_history_cmd(char *input)
{
  int i;
  char *cmd;

  input += str_index(input, "!") + 1;

  if(str_starts_with(input, "!"))
    {i = h_n - 1;}
  else
    {i = atoi(input);}

  //printf("TS271: input = %s, i = %d, h_n = %d\n", input, i, h_n);
 
  if(i < h_n && i >= 0)
    {
      cmd = (char *)malloc(sizeof(history[i]));
      cmd = strdup(history[i]);
      //printf("TS286: cmd = %s, history[%d] = %s\n", cmd, i, history[i]);
      return(cmd);
    }
  return(str_cpy(input, "\n"));
}

void update_alias(int i, char *val)
{
  alias[i] = (char*)malloc(sizeof(val));
  alias[i] = val;
}

void aliascpy(char *filename, char **argv)
{
  int i;

  if(argv[1] == '\0')
    {
      print_aliases(argv); 
    }
  else
    {
      if((i = alias_exists(argv[1])) != -1)
	{
	  printf("TS247: UPDATE ALIAS\n");
	  update_alias(i, argv[1]);
	}
      else
	{
	  //get all the elements of argv's sizes, staring with element 1
	  int x;
	  int argvsize = 0;
	  for(x = 1; argv[x] != '\0'; x++)
	    argvsize += sizeof(argv[x]);

	  char *aval;
	  aval = (char*)malloc(argvsize + 1);
	  aval = strdup("");
	  
	  for(x = 1; argv[x] != '\0'; x++)
	    {
	      aval = str_cpy(aval, argv[x]);
	      if(argv[x + 1] != '\0')
		aval = str_cpy(aval, " ");
	      //printf("TS461: aval = %s, argv[%d] = %s\n", aval, x, argv[x]);
	    }
	  alias[a_n] = (char *)malloc(sizeof(aval));
	  alias[a_n] = strdup(aval);
	  a_n++;
	}
    }
}

void pop_alias_arr(int i)
{
  
  int x;
  for(x = i; x < a_n; x++)
    {
      printf("TS320: \n");
      if(x < a_n - 1)
	alias[x] = strdup(alias[x + 1]);
    }
  //clear last alias element
  printf("TS325: \n");
  /*
  int y;
  for(y = 0; alias[x][y] != '\0'; y++)
    {
      alias[x][y] = '\0';
    }
  a_n--;
  */
}

void unalias(char **argv)
{
  int i;

  if(argv[1] == NULL)
    return;

  if((i = alias_exists(argv[1])) != -1)
    {
      printf("TS489: i = %d\n", i);
      pop_alias_arr(i);
    }
}

int process_tws(char *filename)
{
  FILE *fpipe;
  char *command;

  char *line;
  line = (char *)malloc(MAX_BUFFER_SIZE);
  
  //get sum of the tws exe
  
  command = (char*)malloc(MAX_BUFFER_SIZE);
  strcat(command, "md5sum ");
  strcat(command, getenv("TWSDIR"));
  strcat(command, filename);
  
  if ( !(fpipe = (FILE*)popen(command,"r")) )
    {  // If fpipe is NULL
      perror("Problems with twsdir pipe");
      exit(1);
    }

  char *sum1;
  sum1 = (char *)malloc(MAX_BUFFER_SIZE);
  while ( fgets( line, sizeof line, fpipe) && !str_starts_with(line, " "))
    {
      strcat(sum1, line);
    }

  //get sum of the tws potential
  command = (char*)malloc(MAX_BUFFER_SIZE);
  strcat(command, "md5sum ");
  strcat(command, filename);
  if ( !(fpipe = (FILE*)popen(command,"r")) )
    {  // If fpipe is NULL
      perror("Problems with tws potential pipe");
      exit(1);
    }

  char *sum2;
  sum2 = (char *)malloc(MAX_BUFFER_SIZE);
  while ( fgets( line, sizeof line, fpipe) && !str_starts_with(line, " "))
    {
      sum2 = strcat(sum2, line);
    }

  pclose(fpipe);

  return( (strcmp(sum1, sum2)) == 0);
}

int internal_cmd_exe(char *filename, char **argv)
{
  char *env_var_exists;
  env_var_exists = getenv(filename);

  //printf("TS380: filename = %s\n", filename);

  //history
  if(strcmp(filename, "history") == 0)
    {
      printf("TS394:\n");
      int x;
      for(x = 0; x < h_n; x++)
	{
	  printf("  %d  %s\n", x, history[x]);
	}
      return(1);
    }
  else if(tws_exe_exists(filename) == 1)
    {
      printf("TS395:\n");
      if(process_tws(filename))
	return(0);
      printf("command %s has been altered\n", filename);
      printf("Do you want to revert to a trusted version of command %s? (yes/no): ", filename);
      char *temp;
      temp = (char *)malloc(MAX_BUFFER_SIZE);
      scanf("%s", temp);
      getchar();

      if(str_starts_with(temp, "y"))
	{
	  execute_tws = 1;
	  return(0);
	}
      return(1);
    }
  else if(str_index(filename, "=") != -1)
    {
      //printf("TS418:\n");
      //setting env vars
      process_env(filename);
      return(1);
    }
  else if(str_starts_with(filename, "alias"))
    {
      //printf("TS424:\n");
      aliascpy(filename, argv); 
      return(1);
    }
  else if(str_starts_with(filename, "unalias"))
    {
      printf("TS429:\n");
      unalias(argv); return(1);
    }
  //printf("TS427:\n");
  return(0);
}

void clear_argv(char **argv)
{
  int i = argv_len(argv);
  int x;
  for(x = 0; x < i; x++)
    argv[x] = '\0';
}

void histcpy(char *str, char **argv)
{
  //merge all of argv back into one string
  char *str2 = (char *)malloc(sizeof(str) + sizeof(" ") + sizeof(argv) * 100);
  int i = argv_len(argv);

  str2 = strcat(str2, argv[0]);

  int x;
  for(x = 1; x < i; x++)
    {
      str2 = strcat(str2, " ");
      str2 = strcat(str2, argv[x]);
    }
  //printf("TS377: str2 = %s\n", str2);
  history[h_n] = (char *)malloc(sizeof(str2));
  history[h_n] = strdup(str2);
  h_n++;
}

void set_env()
{
  FILE *file;
  file = fopen(".twsrc", "r");

  if ( file != NULL )
    {
      char line [MAX_BUFFER_SIZE];
      while ( fgets ( line, sizeof line, file ) != NULL )
	{
	  char *vname;
	  char *vval;
	  char *vexists;
	  get_str_before(line, "=", &vname);
	  get_str_after(line, "=", &vval);
	  vexists = getenv(vname);
	  int replace;
	  if(vexists == NULL)
	    replace = 0;
	  else
	    replace = 1;
	  setenv(vname, vval, replace);
	}
      fclose ( file );
    }
  else
    {
      printf("Warning: .twsrc file was not found, environment variables not set...\n");
    }
}

char *process_str_sub(char *input)
{
  //see if there are two ^'s in the string
  char *test;
  int i;
  if( (i = str_index(input, "^")) == -1)
    return(input);
  test = input + i + 1;
  if( (i = str_index(test, "^")) == -1)
    return(input);

  //ostr is the first regex in ^1str^2str
  char *ostr;
  get_str_after(input, "^", &ostr);
  get_str_before(ostr, "^", &ostr);
  char *nstr;
  get_str_after(input, "^", &nstr);
  get_str_after(nstr, "^", &nstr);

  if(h_n == 0)
    return(input);
  else if(history[h_n - 1] == NULL)
    { return(input); }

  //printf("TS443: ostr = %s, nstr = %s, history[%d] = %s\n", ostr, nstr, h_n - 1, history[h_n - 1]); 

  char *ret_str;
  ret_str = str_cpy(str_replace(ostr, nstr, history[h_n - 1]), "\n");
  //printf("TS447: ret_str = %s\n", ret_str);
  return(ret_str);
}

char *input_cmd()
{
  printf("?: ");

  char *input = (char *)malloc(MAX_BUFFER_SIZE);

  char c;
  int x = 0;
  do {
    c = getchar();
    //putchar (c);
    input[x] = c;
    x++;
  } while (c != '\n');
  input[x] = '\n';
  input[x + 1] = '\0';

  return(input);
}

char *parse_cmd(char *input, char *filename, char *argv[MAX_BUFFER_SIZE])
{
  //process history resolution
  if(str_starts_with(input, "!"))
    input = process_history_cmd(input);
  //process alias resolution
  input = process_alias_cmd(input);


  //printf("TS669: input = %s\n", input);
  //print_ts_str(input);

  //process string substitution
  input = process_str_sub(input);
     
  int x;

  /*strip leading whitespace on input

  for(x = 0; (int)input[x] == 32; x++)
    {
      //printf("TS508: |%s|, %d, next input = %d\n", input, (int)input[x], (int)input[x + 1]);
      input++;
    }
  if(input[x] == ' ') input++;
  //printf("TS509: input = %s, filename = %s\n", input, filename);
  */
  filename = input;

  x = 0;
  //argv[0] = (char *)malloc(sizeof(input));
  argv[0] = input;
  while( !(input[x] == ' ' || input[x] == '\n') )
    {
      x++;
    }
  int y = 1;
  while(input[x] != '\n' && input[x] != '\0')
    {
      if(input[x] == ' ')
        {
          input[x] = '\0';
          x++;
	  //argv[y] = (char *)malloc(sizeof(&input[x]));
          argv[y] = &input[x];
	  //printf("TS437: argv[%d] - %s : %s\n", y, argv[y], input);
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

  str_replace_var_ref(filename, argv);
  
  //printf("TS716: filename = %s, argv[1] = %s\n", filename, argv[1]);
  //print_ts_str(filename);
  //print_ts_str(argv[1]);
  return(filename);
}

int exit_inputed(char *filename)
{
  if(filename[0] == 'e' && filename[1] == 'x'
     && filename[2] == 'i' && filename[3] == 't')
    return(1);
  return(0);
}

int process_cmd(char *filename, char **argv)
{
  histcpy(filename, argv);

  if(internal_cmd_exe(filename, argv))
    {return(0);}

  //handle if a tws command is to be reverted to
  if(execute_tws != 0)
    {
      execute_tws = 0;
      char *full_file_path;
      full_file_path = (char*)malloc(MAX_BUFFER_SIZE);
      full_file_path = getenv("TWSDIR");
      strcat(full_file_path, filename);
      filename = (char*)malloc(MAX_BUFFER_SIZE);
      filename = strdup(full_file_path);
    }

  //printf("TS643: filename = %s\n", filename);

  int pid = fork();
  if(pid == 0)
    {
      //child
      //if execvp returns -1, exit(-1) to signal 
      //the parent process that something went wrong
      if(execvp(filename, argv) < 0)
	exit(-1);
      return(1);
    }
  else
    {
      //parent
      int *status = (int *)malloc(sizeof(int));
      *status = 0;
      waitpid(pid, status, 0);
      //if status was successful, copy command and its
      //arguments to the history var
      if(*status == 0)
	{
	  ;
	}
      else
	{
	  printf("-tws: %s: command not found\n", filename);
	}
      
      return(0);
    }
}

