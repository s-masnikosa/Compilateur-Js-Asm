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
	
	FILE* output = NULL;

	if(argc > 1){
		yyin = fopen(argv[1], "r");
		if(yyin == NULL){
			fprintf(stderr, "Le fichier n'a pas été trouvé\n");
			exit(EXIT_FAILURE);
		}

		if(argc > 2){
			output = fopen(argv[2], "w");
		}
	}

	if(output == NULL){
		fprintf(stderr, "Pas de fichier de sortie, redirection stdout\n");
		output = stdout;
	}

	AST_comm rez;

	if(!yyparse(&rez)){ 							// call to the parsing (and lexing) function
		printf("\nParsing:: C'est bien une expression arithmétique\n");
		rez->rule = 'C';
		print_code(rez, output);
	}
	exit(EXIT_SUCCESS);
}
