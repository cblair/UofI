/*
**************************String manipulations***************************
This file provides string manipulation functions for string projects in C
Author: Colby Blair
 */

#include <string.h>
#include <unistd.h>

int strcmp2(char *str1, char *str2)
{
  int x = 0;
  int y = 0;
  while(str1[x] != '\0' && str2[y] != '\0')
    {
      if(str1[x] != str2[y])
	return(0);
      x++;
      y++;  
    }
  if(str1[x] != str2[y])
    return(0);
  return(1);
}

int str_size(char *str)
{
  int x;
  for(x = 0; str[x] != '\0'; x++)
    {;}
  return(x);
}

void print_ts_str(char *str)
{
  int x;
  //for(x = 0; str[x] != '\n' && str[x] != '\0'; x++)

  for(x = 0; str[x] != '\0'; x++)
    printf("TS: str[%d] = %c : %d\n", x, str[x], (int)str[x]);
}

int str_starts_with(char *haystack, char *needle)
{
  int nsize = str_size(needle);
  int x;
  for(x = 0; x < nsize; x++)
    {
      if(haystack[x] != needle[x])
	return(0);
    }
  return(1);
}

int str_index(char *haystack, char *needle)
{
  int x;
  int index;
  int hsize = str_size(haystack);
  int nsize = str_size(needle);

  for(x = 0; x < hsize; )
    {
      //printf("search h: %c\n", haystack[x]);
      if(needle[0] == haystack[x])
	{
	  index = x;
	  int y;
	  for(y = 0; y < nsize; y++, x++)
	    {
	      //printf("compare h - n: %c %c\n", haystack[x], needle[y]);
	      //if(needle[y+1] == '\0')
	      if(needle[y+1] == '\0' && needle[y] == haystack[x])
		{return(index);}
	      if(needle[y] != haystack[x])
		break;
	    }
	}
      else x++;
    }
  return(-1);
} 

void get_str_before(char *str, char *substr, char **ret_str)
{
  int i = str_index(str, substr);
  int x;

  //printf("TS82: str = %s, substr = %s, i = %d\n", str, substr, i);
  *ret_str = (char *)malloc((strlen(str) + strlen(substr) + 1) *sizeof(char));
  *ret_str = strdup("");

  if(i == -1)
    {
      //printf("TS102\n");
      //print_ts_str(strdup(str));
      *ret_str = strdup(str);
    }

  for(x = 0; x < i; x++)
    {
      strncat(*ret_str, str, 1);
      //printf("TS100: *ret_str = %s, str = %s, i = %d, x = %d\n", *ret_str, str, i, x);
      str++;
    }
  //printf("TS102getstrbefore: ret_str = %s\n", *ret_str);

}

char *str_cpy(char *str1, char *str2)
{
  char *ret_str;

  //printf("TS114: str1 = %s, str2 = %s\n", str1, str2);

  //str3 = (char *)calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));     
  ret_str = (char *)malloc((strlen(str1) + strlen(str2) + 1) * sizeof(char) * 2);

  //printf("TS117: str1 = %s, str2 = %s\n", str1, str2);

  ret_str = strdup("");
  //ret_str = (char *)malloc(sizeof(str1) + sizeof(str2));                    
  strcat(ret_str, str1);
  //printf("TS122: ret_str = %s\n", ret_str);
  strcat(ret_str, str2);
  //printf("TS124: ret_str = %s\n", ret_str);

  //printf("ret_str = %s\n", ret_str);
  return(ret_str);
}

void get_str_from_to_end(char *str, char *substr, char **ret_str)
{
  int i = str_index(str, substr);
  int x;

  //printf("TS82: str = %s, substr = %s, i = %d\n", str, substr, i);
  *ret_str = (char *)malloc((strlen(str) + strlen(substr) + 1) *sizeof(char));
  *ret_str = strdup("");

  for(x = 0; x < i; x++)
    {
      str++;
      //printf("TS122: str = %s\n", str);
    }
  str++;

  *ret_str = str_cpy(*ret_str, str);
  // printf("TS128: str = %s\n", str);
}

void get_str_after(char *str, char *substr, char **ret_str)
{
  
  int x = str_index(str, substr) + strlen(substr);
  str += x;

  //printf("TS116: str = %s\n", str);

  *ret_str = (char *)malloc((strlen(str) + strlen(substr) + 1) *sizeof(char));
  *ret_str = strdup("");
  for(; *str != '\0'; x++)
    {
      //printf("TS101: str = %s\n", str);
      strncat(*ret_str, str, 1);
      //printf("TS101: *ret_str = %s\n", *ret_str);
      str++;
    }
}

//function not completed, not used
/*
char **str_split(char *ostr, char *dstr)
{
  char **sarr;
  sarr = (char **)malloc(sizeof(ostr) + sizeof(dstr));
  
  //printf("TS: ostr = %s\n", ostr);

  char *i;
  i = ostr;
  char **temp;

  int x;
  for(x = 0; str_index(ostr, dstr) != -1; x++, i += strlen(*temp))
    {
      get_str_before(i, " ", temp);
      sarr[x] = *temp;
      //printf("TS162: sarr[%d] = %s\n", x, sarr[x]);
    }
  return("Not finished");
}
*/

//gets a string after from, and before to
void get_str_from_to(char *ostr, char *from, char *to, char **ret_str)
{
  //*ret_str = (char *)malloc(sizeof(ostr));
  //*ret_str = strdup("");

  get_str_after(ostr, from, ret_str);
  get_str_before(*ret_str, to, ret_str);
}

char *str_replace(char *rstr, char *rwstr, char *ostr)
{
  //printf("TS386: rstr = %s, rwstr = %s, ostr = %s\n", rstr, rwstr, ostr);

  char *str1;
  char *str2;
  char *str3;

  str1 = strdup("");
  str2 = strdup("");
  str3 = strdup("");

  get_str_before(ostr, rstr, &str1);
  str2 = strdup(rwstr);
  get_str_after(ostr, rstr, &str3);

  //printf("TS408: str1 = %s, str2 = %s, str3 = %s\n", str1, str2, str3);

  char *ret_str;
  ret_str = (char*)malloc( (sizeof(str1) + sizeof(str2) + sizeof(str3)));
  ret_str = strdup("");
  ret_str = str_cpy(str1, str2);
  if(strcmp(str3, "") != 0)
    ret_str = str_cpy(ret_str, str3);

  //printf("TS250: ret_str = %s\n", ret_str);
  return(ret_str);
}

char *str_replace2(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  //printf("TS265: str = %s, orig = %s, rep = %s\n", str, orig, rep);

  if(!(p = strstr(str, orig)))
    {return str;}

  strncpy(buffer, str, p-str); 
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
  
  return buffer;
}


