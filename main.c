/*	file main.c
 *	compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
 *	result: executable
 */
#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
extern int yyparse(AST_comm* rez);			// avoid implicit call
int main(void){
	AST_comm rez;
	if(!yyparse(&rez)){ 							// call to the parsing (and lexing) function
		printf("\nParsing:: C'est bien une expression arithmétique\n");
		rez->rule = 'C';
		print_code(rez);
	}
	exit(EXIT_SUCCESS);
}
