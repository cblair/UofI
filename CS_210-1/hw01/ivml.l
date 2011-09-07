
%{ 
#include "ivml.h" 
%}
COMMENT "#".*
IGNORE  {COMMENT}|"\n"
LETTER  [a-zA-Z]
DIGIT   [0-9]
REAL    [0-9]*"."[0-9]*
RESERV  "include"
STR     {LETTER}({LETTER}|"."|"/")+
IDENT   "\t"{LETTER}({LETTER}|{DIGIT}|"_")*
INT     [0-9]*

%%

{IGNORE}
"include"   return INCLUDE;
"default"   return DEFAULT;
"Door"      return DOOR;
"Room"      return ROOM;
"Opening"   return OPENING;
"{"         return LEFTCURLY;
"}"         return RIGHTCURLY;
"["         return LEFTSQUARE;
"]"         return RIGHTSQUARE;
","         return COMMA;
{IDENT}     return IDENT;
{STR}       return STRING;
{REAL}      return REAL;
{INT}       return INT;
.           
<<EOF>>     return -1;

%%

