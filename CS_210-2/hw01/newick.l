
%{ 
#include "newick.h"
#include <strings.h> 
#include "token.h"

struct token *yytokens;
int yyntokens;
int yynallocated;
char *temp;
int line_num;

int set_token(int cat);
void inc_line();
int pow(int var, int exp);

%}


%%
"exit"       { printf("Exit\n"); return(-1); }
"quit"       { printf("Exit\n"); return(-1); }
"bye"        { printf("Exit\n"); return(-1); }
[/][/][a-zA-Z0-9 ]* { /*printf("Comment\n");*/ }
"\n"         { line_num++; printf("EOL\n"); }
"debug"      { printf("Set debugging\n"); }
"translate"  { printf("Set translation\n"); }
"rotate"     { printf("Set rotation\n"); }
"scale"      { printf("Set scaling\n"); }
"defaultlen" { printf("Set default length\n"); }
"spacing"    { printf("Set spacing\n"); }
"draw"       { printf("Draw\n"); }
"show"       { printf("Show\n"); }
#([a-z])     { printf("Variable %s\n", yytext); }
"="          { printf("is assigned\n"); }
\"[^\"\r\n]*\"+ { printf("String %s\n", yytext); }
[a-zA-Z.]+   { printf("String \"%s\"\n", yytext); }
[0-9]+|[0-9]+[.][0-9]*|[0-9]*[.][0-9]+|[0-9][.][0-9]+ { printf("Number\t%8.3f\n",(float)atof(yytext) ); }
" "
"("          { printf("Set internal node\n"); }
")"          { printf("End internal node\n"); }
":"          { printf("Begin edge length\n"); }
","          { printf("and\n"); }
";"          { printf("End of tree\n"); }
.            { printf("ERROR: this was skipped: %s\n", yytext); }           
<<EOF>>     return -1;

%%

//not for use until hw 2
int set_token(int cat)
{
int size = sizeof(cat) + sizeof(yytext) + sizeof(yylex());
//calculate size to allocate
if(size >= yynallocated)
 {
  //printf("Setting yynallocated from %d ", yynallocated);
  yynallocated = pow(size, ((yynallocated / 2) + 1 ));
  //printf(" to %d\n", yynallocated);
 }

yytokens = malloc(yynallocated);
yytokens[yyntokens].category = cat;
yytokens[yyntokens].linenumber = line_num;
temp = yytext;
yytokens[yyntokens].text = temp;

yyntokens++;

return(cat);
}

int pow(int var, int exp)
{
if(exp == 0)
 {
  return(1);
 }

int ret_val = 0;
while(exp > 0)
 {
  ret_val += var;
  exp--;
 }

return(ret_val);
}

void inc_line()
{line_num++;}

int test(int x)
{return(x);}
