/*	file main.c
 *	compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
 *	result: executable
 */
#include <stdio.h>
#include <stdlib.h>
extern int yyparse(void);			// avoid implicit call
int main(void){
	if(!yyparse()){ 							// call to the parsing (and lexing) function
		printf("\nParsing:: C'est bien une expression arithmétique\n");
	}
	exit(EXIT_SUCCESS);
}
