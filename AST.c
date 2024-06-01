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

/* create an AST leaf from a value */
AST_expr new_number_expr(double number, char type)
{
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule=type;
    t->number=number;
    t->left=NULL;
    t->right=NULL;
  } else printf("ERR : MALLOC ");
  return t;
}

/* create an AST leaf from a value */
AST_comm new_command(AST_expr expression){
  AST_comm t =  malloc(sizeof(struct _command_tree));
  if (t!=NULL){	/* malloc ok */
    t->expr1 = expression;
  } else printf("ERR : MALLOC ");
  return t;

}

/* delete an AST */
void free_expr(AST_expr t)
{
  if (t!=NULL) {
    free_expr(t->left);
    free_expr(t->right);
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
				printf(":%e: ", t->number);
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

void print_code(AST_comm t, FILE* output){
	printf("\nLe code \"%c\" compilé \n\n", t->rule);
	print_code_rec(t->expr1, output);
	fprintf(output, "Halt\n");
}

void print_code_rec(AST_expr t, FILE* output){
	if(t!=NULL){
		print_code_rec(t->left, output);
		print_code_rec(t->right, output);
		switch(t->rule){
			case 'N':
				fprintf(output, "CstNb %g\n", t->number);
				break;
			case 'B':
				fprintf(output, "CsteBo %s\n", (t->number == 0)?"False":"True");
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
			default:
				perror("Erreur\n");
				exit(-1);
				break;
		}
	}
}
