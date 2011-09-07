#include <stdio.h>
#include "token.h"

FILE *yyin; char *yytext;
struct token *yytokens;
int yyntokens;
int yynallocated;

struct token *arr;

void init_token()
{
  yyntokens = 0;
  yynallocated = 0;
  return;
}

void set_token(int cat)
{
  yynallocated += 4;
  // = malloc(sizeof *yytokens);
  //yytokens[yyntokens].category = cat;
  yyntokens++;
}

int main()
{
  int y = 1;

  /*
  init_token();
  while(y <= 10)
    {
      set_token(y++);
      printf("category = %d\n, size of yytokens = %d\n", yytokens->category, sizeof(yytokens));
    }
  */

  arr = malloc(sizeof(y));
  arr[0].linenumber = y;

  printf("TS: arr[0].linenumber = %d\n", arr[0].linenumber);
  return(0);
}
