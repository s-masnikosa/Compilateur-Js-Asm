/*	file main.c
 *	compilation: gcc -o compiler main.c parser.tab.c lexer.tab.c
 *	result: executable
 */
#include <stdio.h>
#include <stdlib.h>
extern int yyparse(void);			// avoid implicit call
extern FILE* yyin;
int main(int argc, char** argv){
	if(argc > 1){
		yyin = fopen(argv[1], "r");
		if(yyin == NULL){
			fprintf(stderr, "Le fichier n'a pas été trouvé\n");
			exit(EXIT_FAILURE);
		}
	}

	if(!yyparse()){ 							// call to the parsing (and lexing) function
		printf("\nParsing:: C'est bien une expression arithmétique\n");
	}
	exit(EXIT_SUCCESS);
}
