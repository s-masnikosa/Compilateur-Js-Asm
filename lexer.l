/*	file lexer.l
 *	compilation: flex lexer.l
 *	result: lex.yy.c = lexical analyser in C
 */

%{
 #include <stdio.h>       // printf
 #include <math.h>        // float Scientifique et NaN
 #include "parser.tab.h"  // token constants defined un parser.y via #define
%}

%%

[0-9]+(\.[0-9]*)?                               { yylval.number=atof(yytext);
                                                  return NUMBER; }
([0-9]*)(\.[0-9]*)?(\e[-]?[1-9][0-9]*)?         { yylval.number=atof(yytext);
                                                  return SNUMBER; }
NaN                                             { yylval.number=NAN;
                                                  return NaN; }
True|False 									                    { yylval.number=(yytext[0] == 'F')?0:1;
                                                  return BOOLEAN; }
import                                          { return IMPORT; }
[a-z]([[:alnum:]]|[_])*                         { yylval.vname=yytext;
																									return IDENT; }
[ \t\r\n]										                    { ; }	// separator
<<EOF>>											                    { return 0; }
.														                    { return yytext[0]; }

%%
int yywrap(void){ return 1; } 	// function called at the end of the file
