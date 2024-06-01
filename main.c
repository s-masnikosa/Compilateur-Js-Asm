/*	file main.c
 *	compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
 *	result: executable
 */
#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
extern int yyparse(AST_comm* rez);			// avoid implicit call
extern FILE* yyin;
int main(int argc, char** argv){
	if(argc > 1){
		yyin = fopen(argv[1], "r");
		if(yyin == NULL){
			fprintf(stderr, "Le fichier n'a pas été trouvé\n");
			exit(EXIT_FAILURE);
		}
	}
	
	AST_comm rez;

	if(!yyparse(&rez)){ 							// call to the parsing (and lexing) function
		printf("\nParsing:: C'est bien une expression arithmétique\n");
		rez->rule = 'C';
		print_comm(rez);
	}
	exit(EXIT_SUCCESS);
}
