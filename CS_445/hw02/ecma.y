%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <typeinfo>

extern int yylex();
extern int lineno;

// better error reporting
#define YYERROR_VERBOSE

// bison requires that you supply this function
int yyerror(const char *msg)
{
        printf("ERROR(PARSER): %s at line %d.\n", msg, lineno);
        return(1);
}

%}

%union {
        double dval;
        int varindex;
        int ival;
        char *sval;
}

%token EXIT
%token AS ARRAY BOOLEAN BREAK CASE CATCH CLASS CONST CONTINUE DEFAULT DELETE DO ELSE EXTENDS FALSE FINALLY FOR FUNCTION IF IMPLEMENTS IMPORT IN INSTANCEOF INT INTERFACE INTERNAL IS NEW NULL_TYPE NUMBER OBJECT PACKAGE PRIVATE PROTECTED PUBLIC RETURN STRING SUPER SWITCH THIS THROW TO TRUE TRY TYPEOF UINT USE VAR VOID WHILE WITH
%token EACH GET SET NAMESPACE INCLUDE DYNAMIC FINAL NATIVE OVERRIDE PROTOTYPE STATIC 
%token PERIOD COMMA SEMICOLON COLON LC RC LP RP LB RB ASSIGN LT GT LTE GTE DLT DGT SUM DIF DIV MUL POW INCREMENT DECREMENT AMP DAMP PIPE DPIPE AT
%token IDENT INTEGER

%token ID INT_LIT NAME PUNC REAL_LIT STRING_LIT

%type <sval> ID PUNC

%%

proc:	EXIT;

%%
