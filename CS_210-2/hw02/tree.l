%{
#include <stdlib.h>
#include "tree.tab.h"
#include <string.h>

int nlin;

%}

%%
"exit"       { return(EXIT); }
"quit"       { return(EXIT); }
"bye"        { return(EXIT); }
[/][/][a-zA-Z0-9 ]* { /*comment ignore*/; }
"\n"         { ++nlin; return(yytext[0]); }
"debug"      { return(DEBUG); }
"translate"  { return(TRANS); }
"rotate"     { return(ROT); }
"scale"      { return(SCALE); }
"defaultlen" { return(DEFLEN); }
"spacing"    { return(SPACE); }
"draw"       { return(DRAW); }
"show"       { return(SHOW); }
#([a-z])     { yylval.varindex = yytext[1] - 'a';
               return(VAR);
             }
"="          { return(yytext[0]); }
\"[^\"\r\n]*\"+ { yylval.sval = strdup(yytext); 
                  return(STRING); 
             } 
[a-zA-Z]+   { yylval.sval = strdup(yytext); 
              return(STRING); 
            }
[0-9]+|[0-9]+[.][0-9]*|[0-9]*[.][0-9]+|[0-9][.][0-9]+ { yylval.sval = strdup(yytext);
                                                        return NUM;
                                                      }
" "
"("          { return(yytext[0]); }
")"          { return(yytext[0]); }
":"          { return(yytext[0]); }
","          { return(yytext[0]); }
";"          { return(yytext[0]); }
.            { }
<<EOF>>      { return(EXIT); }

%%

