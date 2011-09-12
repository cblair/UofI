#ifndef _TOKENLIST_H
#define _TOKENLIST_H

struct token {
	int category;   /* the integer code returned by yylex */
	char *text;     /* the actual string (lexeme) matched */
	int lineno;     /* the line number on which the token occurs */
	char *filename; /* the source file in which the token occurs */
	int ival;       /* integer constant, store its value here */
	char *sval;      /* string constant, malloc space and store */
};

struct tokenlist {
	struct token *t;
	struct tokenlist *next;
};

int tokenlist_init();
int tokenlist_del();
char *tokenlist_print();
int tokenlist_append(int, char *, int, char *);
char *tokenlist_strip_quotes(char*);

#endif
