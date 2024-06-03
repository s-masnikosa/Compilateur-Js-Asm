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
