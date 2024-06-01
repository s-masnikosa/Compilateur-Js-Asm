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
AST_expr new_number_expr(int number)
{
  AST_expr t=(struct _expr_tree*) malloc(sizeof(struct _expr_tree));
  if (t!=NULL){	/* malloc ok */
    t->rule='N';
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

/* infix print an AST*/
void print_expr(AST_expr t){
  if (t!=NULL) {
    printf("[ ");
    print_expr(t->left);
    if (t->left==NULL && t->rule == 'N') 
			printf(":%d: ",t->number); 
		else 
			printf(":%c: ",t->rule);
    print_expr(t->right);
    printf("] ");
  }
}
void print_comm(AST_comm t){
  if (t!=NULL) {
    printf(" ");
    printf("%c: ",t->rule);
    print_expr(t->expr1);
    printf(" \n");
  }

}

void print_code(AST_comm t){
	printf("\nLe code \"%c\" compilé \n\n", t->rule);
	print_code_rec(t->expr1);
	printf("Halt\n");
}

void print_code_rec(AST_expr t){
	if(t!=NULL){
		print_code_rec(t->left);
		print_code_rec(t->right);
		switch(t->rule){
			case 'N':
				printf("CstNb %d\n", t->number);
				break;
			case '+':
				printf("AddiNb\n");
				break;
			case '*':
				printf("MultNb\n");
				break;
			case '-':
				printf("SubiNb\n");
				break;
			case 'M':
				printf("NegaNb\n");
				break;
			default:
				perror("Erreur\n");
				exit(-1);
				break;
		}
	}
}
