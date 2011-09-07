#include "newick.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern char *yytext;
extern struct token *yytokens;
extern int yyntokens;
extern int yynallocated;

int main(int argc, char *argv[])
{
  int t;
  if(argc >= 2) {
    yyin = fopen(argv[1],"r");
    if (yyin == NULL) { printf("can't open/read '%s'\n", argv[1]); exit(-1); }
  }
  yywrap();
  //not for use until hw 2
  while ((t=yylex()) != -1) {
    printf("token %d code %d line %d text %s\n", yyntokens, t,
	   yytokens[yyntokens-1].linenumber, yytokens[yyntokens-1].text);
  }
  return 0;
}
