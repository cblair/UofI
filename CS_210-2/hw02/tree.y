%{ 
#include <stdio.h>
#include <stdlib.h>
#include "treeClass.cpp"

extern int yylex();
 extern int nlin;

// get better error reporting
#define YYERROR_VERBOSE

// bison requires that you supply this function
void yyerror(const char *msg)
{
  printf("ERROR(PARSER): %s at line %d. \n", msg, nlin + 1);
}

// variable storage
Node vars[26];

//head to the tree
Node *head;

//Node index pointer for operations
Node *pI;

%}

%union {
	double dval;
	int varindex;
        int ival;
        char *sval;
        struct Node *ptr;
}

%token EXIT
%token DEBUG TRANS ROT SCALE DEFLEN SPACE DRAW SHOW VAR STRING NUM

%type <varindex> VAR 
%type <sval> NUM
%type <sval> STRING
%type <ptr> exprlist
%type <ptr> assignment
%type <ptr> name

%%
statementlist : statement                    
              | statement statementlist         {/*shift reduce warning*/}
              | EXIT                            { return(0); }
              ;

statement     : assignment                   		                             
              | directive 
	      | term 
              ;

term          : '\n' | ';' 
              ;

assignment    : VAR '=' NUM                    	{ $$ = new Node; 
                                                  $$->setName($3);
                                                  vars[$1] = $$;
                                               	}
              | VAR '=' exprlist               	{ $$ = $3;
                                               	}
              ;

exprlist      : name                           	{ $$ = $1;
                                               	}
  	      | '(' exprlist ')'	        { $$ = new Node;
						  $$->setChild($2);
						}
              | exprlist ':' NUM                { $$ = $1; /*shift reduce warning*/ 
						  $$->setLen(atoi($3));
                                                }                         
   	      |  exprlist ',' exprlist    	{ $$ = $1; /*shift reduce warning*/
						  $1->addSibling($3);
						}
    	      |  exprlist exprlist       	{ $$ = $1; /*a lot of shift reduce warning*/
						  $1->addSibling($2);
						}
              | VAR                            	{ $$ = &vars[$1];
                                               	}
              ;

directive     : DEBUG                          
              | TRANS NUM NUM                  	{ head->setTranslate(atoi($2), atoi($3)); }
              | ROT NUM                        	{ head->setRotate(atoi($2)); }
              | SCALE NUM NUM                  	{ head->setScale(atoi($2), atoi($3)); }
              | DEFLEN NUM                     	{ head->setLen(atoi($2)); }
              | SPACE NUM                      	{ head->setYDelta(atoi($2)); }
              | DRAW                           	{ head->draw(); }
              | DRAW exprlist                  	{ $2->draw(); }
              | SHOW exprlist                  	{ $2->display(); }
              ;

name          : STRING                          { Node *node = new Node; 
                                                  node->setName($1);
						  $$ = node;
                                                }                         
	      ; 

%%

main()
{
        int i;

	head = new Node;
	pI = new Node;
	pI = head;

        for (i=0; i<26; i++) vars[i] = 0;
        yyparse();
}
