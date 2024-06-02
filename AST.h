/* unary-and-binary tree structure */
struct _expr_tree {
  char rule;                       /* "name" of the rule/operation operation */
  double number;                   /* int  for value */
  char* var;                       /* variable name */
	struct _expr_tree* left;         /* NULL if unary node or leaf*/
  struct _expr_tree* right;        /* used for unary node but NULL if leaf */
};

typedef struct _expr_tree* AST_expr;

struct _command_tree {
  char rule;                    /* "name" of the rule/operation operation */
  AST_expr expr1;     	        /* used for command with at least one sub-expression */
};

typedef struct _command_tree* AST_comm;

struct _program_list{
	AST_comm command;
	struct _program_list* next;
};

typedef struct _program_list* LIST_prog;

/* create an AST from a root value and two AST sons */
AST_expr new_binary_expr(char rule, AST_expr left, AST_expr right);

/* create an AST from a root value and one AST son */
AST_expr new_unary_expr(char rule, AST_expr son);

/* create an AST leaf from a value */
AST_expr new_number_expr(char rule, double number);

/* create a new variable from a string */
AST_expr new_variable_expr(char rule, char* vname);

/* create an AST root from an expression */
AST_comm new_command(AST_expr expression);

/* create a Link from a command */
LIST_prog new_program(LIST_prog p, AST_comm command);

/* delete an AST */
void free_expr(AST_expr t);
void free_comm(AST_comm t);
void free_prog(LIST_prog l);

/* print an AST*/
void print_expr(AST_expr t);
void print_comm(AST_comm t);
void print_prog(LIST_prog l);
