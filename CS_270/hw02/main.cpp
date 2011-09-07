#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAXSIZE 5000

const char *argarr[MAXSIZE];

using namespace std;

int makearg(char *s, char ***args);
int char_pos(char search_char, string orig_str);
string get_str_before(int i, string str);
string get_str_from_to(int i, char to, string str);
int str_len(string str);
string get_str_after(int i, string str);

int main()
{
  char **argv, str[] = "ls -l file";
  int argc;
  argc = makearg(str, &argv);

  int x;
  for(x = 0; argv[x] != NULL; x++)
    cout << "Data structure argv[" << x << "]: " << argv[x] << "\n";
  cout << "Found " << argc << " tokens\n";

  return (1);
}

int makearg(char *s, char ***args)
{
  string str = s;
  str += '\0';

  char *str_arr = NULL;
  str_arr = (char*)realloc(str_arr, sizeof(str_arr) * 2);

  int i = 0;
  string ret_str[MAXSIZE];
  int token_num = 0;

  do
    {
      i = char_pos(' ', str);
      ret_str[token_num] = get_str_from_to(0, ' ', str);
      str = get_str_after(i + 1, str);
      //cout << "[" << ret_str[token_num] << "] = token number " << token_num  << "\n";
      argarr[token_num] = ret_str[token_num].c_str();
      cout << argarr[token_num] << "\n";

      if(i != -1)
        {
          token_num++;
          //str_arr = (char*)realloc(str_arr, sizeof(str_arr) * 2);
        }
    }
  while(i != -1);

  //terminate the string array
  argarr[token_num + 1] = NULL;

  *args = (char**)argarr;

  return(token_num + 1);
}

string get_str_after(int i, string str)
{
  string ret_str = "";
  while(str[i] != '\0')
    {
      ret_str += str[i];
      i++;
    }
  return(ret_str);
}

string get_str_before(int i, string str)
{
  string ret_str = "";
  int x = 0;
  while(x < i)
    {
      ret_str += str[x];
      x++;
    }
}

//returns the first instance of the char
int char_pos(char search_char, string orig_str)
{
  int i = 0;
  while(orig_str[i] != search_char
        && orig_str[i] != '\0')
    {++i;}

  if(orig_str[i] == '\0')
    return(-1);
  return(i);
}

string get_str_from_to(int i, char to, string str)
{
  string ret_str = "";
  while(str[i] != to && str[i] != '\0')
    {
      ret_str += str[i];
      i++;
    }
  return(ret_str);
}

int str_len(string str)
{
  int x = 0;
  while(str[x] != NULL)
    {
      x++;
    }
  return(x);
}
