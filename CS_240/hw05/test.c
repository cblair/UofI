#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int str_size(char *str)
{
  int x;
  for(x = 0; str[x] != '\0'; x++)
    {;}
  return(x);
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
              if(needle[y+1] == '\0')
                return(index);
              if(needle[y] != haystack[x])
                break;
            }
        }
      else x++;
    }
  return(-1);
}

char *str_cpy(char *str1, char *str2)
{
  char *ret_str;

  //str3 = (char *)calloc(strlen(str1) + strlen(str2) + 1, sizeof(char));
  ret_str = (char *)malloc((strlen(str1) + strlen(str2) + 1) * sizeof(char) * 2); 
  //ret_str = (char *)malloc(sizeof(str1) + sizeof(str2)); 
  strcat(ret_str, str1);
  strcat(ret_str, str2);

  printf("ret_str = %s\n", ret_str);
  return(ret_str);
}

int main()
{
  char *str1 = "Hello ";
  char *str2 = "there";
  char *str3;

  str3 = str_cpy(str1, str2);

  printf("LOCAL TEST! str3: %s\n", str3);

  return 0;

}
