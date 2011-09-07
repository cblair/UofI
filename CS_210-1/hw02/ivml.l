
%{ 
#include "ivml.h"
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
COMMENT "#".*
IGNORE  {COMMENT}
LETTER  [a-zA-Z]
DIGIT   [0-9]
REALNUM [0-9]*"."[0-9]*
RESERV  "include"
STR     {LETTER}({LETTER}|"."|"/")+
ID      "\t"{LETTER}({LETTER}|{DIGIT}|"_")*
INTEG   [0-9]*

%%

"\n"        { line_num++; }
{IGNORE}
"include"   { set_token(INCLUDE); return INCLUDE; }
"default"   { set_token(DEFAULT); return DEFAULT; } 
"Door"      { set_token(DOOR); return DOOR; }
"Room"      { set_token(ROOM); return ROOM; }
"Opening"   { set_token(OPENING); return OPENING; }
"{"         { set_token(LEFTCURLY); return LEFTCURLY; } 
"}"         { set_token(RIGHTCURLY); return RIGHTCURLY; }
"["         { set_token(LEFTSQUARE); return LEFTSQUARE; }
"]"         { set_token(RIGHTSQUARE); return RIGHTSQUARE; }
","         { set_token(COMMA); return COMMA; }
{ID}        { set_token(IDENT); return IDENT; }
{STR}       { set_token(STRING); return STRING; } 
{REALNUM}   { set_token(REAL); return REAL; }
{INTEG}     { set_token(INT); return INT; } 
.           
<<EOF>>     return -1;

%%

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
{
line_num++;
}

int test(int x)
{
return(x);
}
