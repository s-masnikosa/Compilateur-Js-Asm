/*	file lexer.l
 *	compilation: flex lexer.l
 *	result: lex.yy.c = lexical analyser in C
 */

%{
 #include <stdio.h>				// printf
 #include "parser.tab.h"	// token constants defined un parser.y via #define
%}

%%

(0|[1-9][0-9]*)(\.[0-9]*[1-9])?	{ printf("lex: création token FLOAT %s\n", yytext); 
																	return FLOAT; }
[ \t\r]													{ ; }	// separator
\n															{ printf("lex: fin de lecture\n");
																	return 0; }
.																{ printf("lex: création token %s\n", yytext);
																	return yytext[0]; }

%%
int yywrap(void){ return 1; } 	// function called at the end of the file
