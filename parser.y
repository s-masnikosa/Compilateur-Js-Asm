/*	file parser.y
 *	compilation: bison -d parser.y
 *	result: parser.tab.c = C code for su=yntaxic analyser
 *	result: parser.tab.h = def. of lexical units aka lexems
 */

%code requires { #include "AST.h" }

%{	// the code between %{ and %} is copied at the start of the generated .c
 #include <stdio.h>
 int yylex(void);											// declared to avoid implicit call
 int yyerror(void* rez, const char*);	// on generated functions 
%}

%parse-param {LIST_prog* rez}
%union {AST_comm comm; AST_expr expr; double number; char* vname;}

%token <number> NUMBER				// kinds of non-trivial tokens expected from the lexer
%token <number> SNUMBER
%token <number> NaN
%token <number> BOOLEAN
%token IMPORT <vname>IDENT
%type <expr> expression
%type <comm> command
%start program			// main non-terminal

%left '<' '=' '&'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMOINS NOT

%%	// denotes the begining of the grammar with bison-specific syntax

program:
  /* epsilon */
| command program
		{ *rez = new_program(*rez, $1); }
;

command:					// a command is
	expression ';'	// an expression followed by a semicolon
		{ $$ = new_command($1); }
| IMPORT IDENT ';'
		{ $$ = new_command(new_variable_expr('I', $2)); }
;

expression:										// an expression is
  expression '+' expression		// either a sum of an expression and an expression
		{ $$=new_binary_expr('+', $1, $3); }
|	expression '-' expression		// or an expression minus an expression
		{ $$=new_binary_expr('-', $1, $3); }
| expression '*' expression		// or an expression times an expression
		{ $$=new_binary_expr('*', $1, $3); }
| expression '/' expression		// or an expression divided by an expression
		{ $$=new_binary_expr('/', $1, $3); }
| expression '%' expression		// or an expression modulo an expression
		{ $$=new_binary_expr('%', $1, $3); }
| '(' expression ')'					// or an expression surounded by parentheses
		{ $$=$2; }
| '-' expression %prec UMOINS	// or the negation of an expression
		{ $$=new_unary_expr('M', $2); }
| NUMBER											// or a NUMBER
		{ $$=new_number_expr('N', $1); }
| expression '<' '=' expression
		{ $$=new_binary_expr('L', $1, $4); }
| expression '=' '=' expression 
		{ $$=new_binary_expr('E', $1, $4); }
| expression '<' expression
		{ $$=new_binary_expr('<', $1, $3); }
| IDENT '=' expression
		{ $$=new_equals_expr($1, $3); }
| '!' expression %prec NOT
		{ $$=new_unary_expr('!', $2); }
| expression '&' '&' expression
		{ $$=new_binary_expr('&', $1, $4); }
| BOOLEAN											// or a BOOLEAN
		{ $$=new_number_expr('B', $1); }
| NaN
		{ $$=new_number_expr('n', $1); }
| SNUMBER
		{ $$=new_number_expr('S', $1); }
| IDENT
		{ $$=new_variable_expr('V', $1); }
;

%%	// denotes the end of the grammar
		// everything after %% is copied at the end of the generated .c
int yyerror(void* rez, const char* msg){	// called by the parser if the parsing fails
	printf("Parsing:: syntax error\n");
	return 1;										// to distinguisg with the 0 returned by the success
}
