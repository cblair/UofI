

%%
"#".*
"\n"
" "
"default"   return DEFAULT;
"include"   return INCLUDE;
"Door"      return DOOR;
"Room"      return ROOM;
"Opening"   return OPENING;
"{"         return LEFTCULY;
"}"         return RIGHTCURL;
"["         return LEFTSQUARE;
"]"         return RIGHTSQUARE;
","         return COMMA;
[a-z]*|[A-Z]*|"\\"   return STRING;
[A-Z]*|[a-z]*|"\\"   return STRING;
"\t"[a-z]*|[A-Z]*    return IDENT;
[0-9]*"."[0-9]* return REAL;
[0-9]*       return INT;
.           
<<EOF>>     return -1;
%%

