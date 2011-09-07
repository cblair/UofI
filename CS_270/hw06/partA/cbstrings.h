#include <string.h>
#include <stdlib.h>

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
  for(x = 0; x < i; x++)
    {
      strncat(*ret_str, str, 1);
      //printf("TS100: *ret_str = %s, str = %s, i = %d, x = %d\n", *ret_str, st\
	r, i, x);
  str++;
}
//printf("TS102getstrbefore: ret_str = %s\n", *ret_str);

}

void get_str_after(char *str, char *substr, char **ret_str)
{
  //int x = str_index(str, substr) + 1;
  int x = str_index(str, substr) + strlen(substr);
  str += x;

  //printf("TS116: str = %s\n", str);

  *ret_str = (char *)malloc((strlen(str) + strlen(substr) + 1) *sizeof(char));
  for(; *str != '\0'; x++)
    {
      //printf("TS101: str = %s\n", str);
      strncat(*ret_str, str, 1);
      str++;
    }

}

char *str_cpy(char *str1, char *str2)
{
  char *ret_str;

  //str3 = (char *)calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
  ret_str = (char *)malloc((strlen(str1) + strlen(str2) + 1) * sizeof(char) *\
                           2);
  //ret_str = (char *)malloc(sizeof(str1) + sizeof(str2));
  strcat(ret_str, str1);
  strcat(ret_str, str2);

  //printf("ret_str = %s\n", ret_str);
  return(ret_str);
}

//function not completed, not used
char **str_split(char *ostr, char *dstr)
{
  char **sarr;
  sarr = (char **)malloc(sizeof(ostr) + sizeof(dstr));

  printf("TS: ostr = %s\n", ostr);

  char *i;
  i = ostr;
  char **temp;

  int x;
  for(x = 0; str_index(ostr, dstr) != -1; x++, i += strlen(*temp))
    {
      get_str_before(i, " ", temp);
      sarr[x] = *temp;
      printf("TS162: sarr[%d] = %s\n", x, sarr[x]);
    }

}

char *str_replace(char *rstr, char *rwstr, char *ostr)
{
  //printf("TS386: rstr = %s, rwstr = %s, ostr = %s\n", rstr, rwstr, ostr);

  char *str1;
  char *str2;
  char *str3;
  get_str_before(ostr, rstr, &str1);
  str1++;
  str2 = str_cpy(rwstr, "");
  get_str_after(ostr, rstr, &str3);


  //printf("TS408: str1 = %s, str2 = %s, str3 = %s\n", str1, str2, str3);

  char *ret_str;
  ret_str = str_cpy(str1, str2);
  ret_str = str_cpy(ret_str, str3);

  return(ret_str);
}
