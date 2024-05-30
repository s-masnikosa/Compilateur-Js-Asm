/*	file parser.y
 *	compilation: bison -d parser.y
 *	result: parser.tab.c = C code for su=yntaxic analyser
 *	result: parser.tab.h = def. of lexical units aka lexems
 */

%{	// the code between %{ and %} is copied at the start of the generated .c
 #include <stdio.h>
 int yylex(void);						// declared to avoid implicit call
 int yyerror(const char*);	// on generated functions 
%}

%token NUMBER				// kinds of non-trivial tokens expected from the lexer
%start expression		// main non-terminal

%left '+' '-'
%left '*'
%nonassoc UMOINS

%%	// denotes the begining of the grammar with bison-specific syntax

expression:										// an expression is
	expression '+' expression		// either a sum of an expression and an expression
|	expression '-' expression		// or an expression minus an expression
| expression '*' expression		// or an expression times an expression
| '(' expression ')'					// or an expression surounded by parentheses
| '-' expression %prec UMOINS	// or the negation of an expression
| NUMBER											// or a NUMBER
;

%%	// denotes the end of the grammar
		// everything after %% is copied at the end of the generated .c
int yyerror(const char* msg){	// called by the parser if the parsing fails
	printf("Parsing:: syntax error\n");
	return 1;										// to distinguisg with the 0 returned by the success
}
