#include <stdio.h>
#include <stdlib.h>
#include "AST.h"

/* create an AST from a root value and two AST sons */
AST_expr new_binary_expr(char rule, AST_expr left, AST_expr right) {
	  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule=rule;
    t->left=left;
    t->right=right;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST from a root value and one AST son */
AST_expr new_unary_expr(char rule, AST_expr son)
{
  return new_binary_expr(rule, NULL,son);
}

/* create AST equals */
AST_expr new_equals_expr(char* vname, AST_expr son){
	AST_expr t = new_unary_expr('=', son);
	if(t != NULL) t->var=vname;
	return t;
}


/* create an AST leaf from a value */
AST_expr new_number_expr(char rule, double number)
{
	AST_expr t=new_binary_expr(rule, NULL, NULL);
	if(t!=NULL) t->number=number;
	return t;
}

AST_expr new_variable_expr(char rule, char* vname){
	AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule=rule;
    t->left=NULL;
    t->right=NULL;
		t->var = vname;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST leaf from a value */
AST_comm new_command(AST_expr expression){
  AST_comm t = (struct _command_tree*)malloc(sizeof(struct _command_tree));
  if (t!=NULL){	/* malloc ok */
    t->expr1 = expression;
  } else printf("ERR : MALLOC ");
  return t;

}

/* create a Link from a command */
LIST_prog new_program(LIST_prog p, AST_comm command){
	LIST_prog l=(struct _program_list*)malloc(sizeof(struct _program_list));
	if(l!=NULL){
		l->command = command;
		l->next = p;
	}else
		fprintf(stderr, "ERR : MALLOC \n");
	return l;
}

/* delete an AST */
void free_expr(AST_expr t)
{
  if (t!=NULL) {
    free_expr(t->left);
    free_expr(t->right);
		free(t->var);
		free(t);
  }
}
void free_comm(AST_comm t)
{
  if (t!=NULL) {
    free_expr(t->expr1);
    free(t);
  }
}
void free_prog(LIST_prog l){
	if(l!=NULL){
		free_prog(l->next);
		free_comm(l->command);
		free(l);
	}
}

void print_expr(AST_expr t){
  if (t!=NULL) {
    printf("[ ");
    switch(t->rule){
		  case 'N':
		  	printf(":%g: ",t->number);
		  	break;
		  case 'B':
		  	printf(":%s: ", (t->number == 0)?"True":"False");
				break;
			case 'n':
				printf(":%lf: ", t->number);
				break;
			case 'S':
				printf(":%g: ", t->number);
				break;
			case 'V':
				printf(":%s: ", t->var);
				break;
			case 'I':
				printf(":I: %s.jsm ", t->var);
				break;
			case '=':
				printf("[ :%s: ] ", t->var); // pas de break pour continuer sur le reste
			default:
				if(t->left != NULL)
					print_expr(t->left);
				printf(":%c: ", t->rule);
				print_expr(t->right);
				break;
    }
    printf("] ");
  }
}


/* infix print an AST*/

void print_comm(AST_comm t){
  if (t!=NULL) {
    printf(" ");
    printf("%c: ",t->rule);
    print_expr(t->expr1);
    printf(" \n");
  }
}

void print_prog(LIST_prog l){
	if(l!=NULL){
		print_comm(l->command);
		print_prog(l->next);
	}
}

void print_code(AST_comm t, FILE* output){
	sizeof_expr(t->expr1);
	print_code_rec(t->expr1, output);
	fprintf(output, "Drop\n");
}

void print_code_rec(AST_expr t, FILE* output){
	if(t!=NULL){
		print_code_rec(t->left, output);
		if(t->rule == '&')
			fprintf(output, "ConJmp %d\n", t->right->depth + 1);
		print_code_rec(t->right, output);
		switch(t->rule){
			case 'N':
				fprintf(output, "CstNb %g\n", t->number);
				break;
			case 'n':
				fprintf(output, "CstNb %lf\n", t->number);
				break;
			case 'S':
				fprintf(output, "CstNb %g\n", t->number);
				break;
			case 'B':
				fprintf(output, "CsteBo %s\n", (t->number == 0)?"False":"True");
				break;
			case 'V':
				fprintf(output, "GetVar %s\n", t->var);
				break;
			case '=':
				fprintf(output, "SetVar %s\n", t->left->var);
				break;
			case '+':
				fprintf(output, "AddiNb\n");
				break;
			case '*':
				fprintf(output, "MultNb\n");
				break;
			case '-':
				fprintf(output, "SubiNb\n");
				break;
			case 'M':
				fprintf(output, "NegaNb\n");
				break;
			case '/':
				fprintf(output, "DivNb\n");
				break;
			case '%':
				fprintf(output, "ModuNb\n");
				break;
			case 'L': // <=
				fprintf(output, "LoEqNb\n");
				break;
			case 'E': // ==
				fprintf(output, "Equals\n");
				break;
			case '<':
				fprintf(output, "LoStNb\n");
				break;
			case '!':
				fprintf(output, "Not\n");
				break;
			case 'I':
				FILE* f = fopen(strcat(t->var, ".jsm"), "r");
				if(f == NULL){
					fprintf(stderr, "Le fichier \"%s\" n'a pas été trouvé\n", t->var);
					exit(EXIT_FAILURE);
				}
				char c;
				while( (c = fgetc(f)) != EOF )
					fputc(c, output);
				break;
			case '&':
				fprintf(output, "Jump 1\nCsteBo False\n");
				break;
			default:
				perror("Erreur\n");
				exit(-1);
				break;
		}
	}
}

void print_prog_code(LIST_prog l, FILE* output){
	printf("\nCompilation..\n");
	print_prog_code_rec(l, output);
	fprintf(output, "Halt");
	printf("\nLe code a été compilé avec succès\n");
}

void print_prog_code_rec(LIST_prog l, FILE* output){
	if(l != NULL){
		print_code(l->command, output);
		print_prog_code_rec(l->next, output);
	}
}

int sizeof_expr(AST_expr expr){
	if(expr == NULL)
		return 0;
	expr->depth = sizeof_expr(expr->left) + sizeof_expr(expr->right) + 1;
	return expr->depth;
}




















