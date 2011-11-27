///////////////////////////////////////////////////////////////////////////////
//Class:	CS 445
//Semester:	Fall 2011
//Assignment:	Homework 4
//Author:	Colby Blair
//File name:	main.h
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

//Exit codes
#define ERROR_LEXICAL	1
#define ERROR_SYNTAX	2
#define ERROR_SEMANTIC	3
#define ERROR_INTERNAL	4

#define YY_MAX_BUF 1024


#ifdef YYDEBUG
#define DEBUGMSG printf
#else
#define DEBUGMSG //printf
#endif
