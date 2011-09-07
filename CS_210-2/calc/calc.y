%{ 
#include <stdio.h>
#include <stdlib.h>

extern int yylex();

// get better error reporting
#define YYERROR_VERBOSE

// bison requires that you supply this function
void yyerror(const char *msg)
{
      printf("ERROR(PARSER): %s\n", msg);
}

// variable storage
double vars[26];
%}

%union {
	double dvalue;
	int varindex;
}

%token EXIT
%token <dvalue> NUMBER
%token <varindex> NAME

%type <dvalue> expression
%type <dvalue> term
%type <dvalue> varornum
%%
statementlist : statement '\n' 
              | statement '\n' statementlist
              ;

statement : NAME '=' expression     { vars[$1] = $3; }
	  | expression              { printf("ANS: %f\n", $1);  }
          | EXIT                    { return 0; }
          ;

expression: expression '+' term     { $$ = $1 + $3; }
          | expression '-' term     { $$ = $1 - $3; }
          | term                    { $$ = $1; }
          ;

term : term '*' varornum            { $$ = $1 * $3; }
     | term '/' varornum            { if ($3==0) {
	                                  printf("ERROR: Divide %f by zero\n", $1);
	                              }   
                                      else {
	                                  $$ = $1 / $3; 
	                              }
	                            }
     | varornum                     { $$ = $1; }
     ;

varornum : NUMBER                   { $$ = $1; }
	 | NAME                     { $$ = vars[$1]; }
         | '(' expression ')'       { $$ = $2; }
         ;

%%

main()
{
        int i;

        for (i=0; i<26; i++) vars[i] = 0;
        yyparse();
}
