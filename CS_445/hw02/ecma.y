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
%token AS BREAK CASE CATCH CLASS CONST CONTINUE DEFAULT DELETE DO ELSE EXTENDS FALSE FINALLY FOR FUNCTION IF IMPLEMENTS IMPORT IN INSTANCEOF INTERFACE INTERNAL IS NEW NULL_VAL PACKAGE PRIVATE PROTECTED PUBLIC RETURN SUPER SWITCH THIS THROW TO TRUE TRY TYPEOF USE VAR VOID WHILE WITH
%token EACH GET SET NAMESPACE INCLUDE DYNAMIC FINAL NATIVE OVERRIDE PROTOTYPE STATIC 
%token ARRAY INT UINT NUM STRING BOOL OOBJECT
%token OB CB OP CP SQ DQ 
%token SUM DIV DIF MUL POW ASSIGN 
%token EQUIV NOTEQ LT GT LTEQ GTEQ 
%token COLON SEMICOLON COMMA
%token IDENT INTEGER

%token ID NAME PUNC MIX

%type <sval> ID NAME NUM PUNC MIX

%%

proc:	EXIT;

%%
