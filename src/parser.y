%{
/* parser.y
 * Parsing of lexical tokens into abstract syntax tree.
 * by Ryan Wilson-Perkin
 * for CIS4650W15 Compilers: Cflat Assignment
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "astprint.h"

extern int col_num;
extern int line_num;
extern char *line_buf;
extern FILE *yyin;
extern char *yytext;
extern void add_custom_type(char *);
extern void free_custom_types();
extern int yylex(void);

struct program *parse_file(FILE *in);

static struct program *program;
static int n_errors = 0;
static void yyerror(const char *);
%}

%union {
    char cval;
    float fval;
    int ival;
    char *sval;

    struct type_decl_list *type_decl_list_p;
    struct type_decl *type_decl_p;
    struct var_decl_stmt_list *var_decl_stmt_list_p;
    struct var_decl *var_decl_p;
    struct struct_type *struct_type_p;
    struct array_specifier *array_specifier_p;
    struct function_def_list *function_def_list_p;
    struct function_def *function_def_p;
    struct function_param_list *function_param_list_p;
    struct function_body *function_body_p;
    struct function_call *function_call_p;
    struct function_arg_list *function_arg_list_p;
    struct stmt_list *stmt_list_p;
    struct stmt *stmt_p;
    struct expr_stmt *expr_stmt_p;
    struct compound_stmt *compound_stmt_p;
    struct select_stmt *select_stmt_p;
    struct iter_stmt *iter_stmt_p;
    struct return_stmt *return_stmt_p;
    struct expr *expr_p;
    struct var *var_p;
    struct constant *constant_p;
}
%token CHAR ELSE FLOAT FOR IF INT RETURN STRUCT TYPEDEF VOID WHILE 
%token LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN SEMICOLON COMMA
%token ASSIGN OR AND NOT EQ NE LT LE GT GE PLUS MINUS ASTERISK SLASH PERCENT
%token SIZEOF INCREMENT DECREMENT PERIOD
%token UNRECOGNIZED
%token <cval> CONST_CHAR
%token <fval> CONST_FLOAT
%token <ival> CONST_INT
%token <sval> ID
%token <sval> TYPE_NAME

%type <type_decl_list_p> type_decl_list;
%type <type_decl_p> type_decl;
%type <var_decl_stmt_list_p> var_decl_stmt_list;
%type <var_decl_p> var_decl;
%type <var_decl_p> var_decl_stmt;
%type <struct_type_p> struct_type;
%type <array_specifier_p> array_specifier;
%type <function_def_list_p> function_def_list;
%type <function_def_p> function_def;
%type <function_param_list_p> function_param_list;
%type <function_body_p> function_body;
%type <function_call_p> function_call;
%type <function_arg_list_p> function_arg_list;
%type <stmt_list_p> stmt_list;
%type <stmt_p> stmt;
%type <expr_stmt_p> expr_stmt;
%type <compound_stmt_p> compound_stmt;
%type <select_stmt_p> select_stmt;
%type <iter_stmt_p> iter_stmt;
%type <return_stmt_p> return_stmt;
%type <expr_p> expr;
%type <expr_p> assign_expr;
%type <expr_p> logical_or_expr;
%type <expr_p> logical_and_expr;
%type <expr_p> equality_expr;
%type <expr_p> relational_expr;
%type <expr_p> additive_expr;
%type <expr_p> multiplicative_expr;
%type <expr_p> unary_expr;
%type <expr_p> postfix_expr;
%type <var_p> var;
%type <constant_p> constant;
%type <ival> basic_type

/* Expect a single shift/reduce conflict for dangling else. */
%expect 1

%%

program 
    : type_decl_list var_decl_stmt_list function_def_list { program = create_program(line_num, col_num, $1, $2, $3); }
    ; 

type_decl_list
    : /* empty */ { $$ = NULL; }
    | type_decl_list type_decl { $$ = create_type_decl_list(line_num, col_num, $2, $1); }
    ;

type_decl
    : TYPEDEF var_decl SEMICOLON { $$ = create_type_decl(line_num, col_num, $2); add_custom_type(((struct var_decl *)$2)->id); }
    ;

var_decl_stmt_list
    : /* empty */ { $$ = NULL; }
    | var_decl_stmt_list var_decl_stmt { $$ = create_var_decl_stmt_list(line_num, col_num, $2, $1); }
    ;

var_decl_stmt
    : var_decl SEMICOLON { $$ = $1 }
    ;

var_decl
    : basic_type ID array_specifier { $$ = create_var_decl_basic(line_num, col_num, $1, $2, $3); }
    | struct_type ID array_specifier { $$ = create_var_decl_struct(line_num, col_num, $1, $2, $3); }
    | TYPE_NAME ID array_specifier { $$ = create_var_decl_typedef(line_num, col_num, $1, $2, $3); }
    ;

struct_type
    : STRUCT LBRACE var_decl_stmt_list RBRACE { $$ = create_struct_type(line_num, col_num, NULL, $3); }
    | STRUCT ID LBRACE var_decl_stmt_list RBRACE { $$ = create_struct_type(line_num, col_num, $2, $4); }
    ;

array_specifier
    : /* empty */ { $$ = NULL; }
    | LBRACKET constant RBRACKET { $$ = create_array_specifier(line_num, col_num, $2); }
    ;

function_def_list
    : function_def { $$ = create_function_def_list(line_num, col_num, $1, NULL); }
    | function_def_list function_def { $$ = create_function_def_list(line_num, col_num, $2, $1); }
    ;

function_def
    : basic_type ID LPAREN function_param_list RPAREN function_body { $$ = create_basic_function_def(line_num, col_num, $1, $2, $4, $6); }
    | VOID ID LPAREN function_param_list RPAREN function_body { $$ = create_void_function_def(line_num, col_num, $2, $4, $6); }
    | basic_type ID LPAREN RPAREN function_body { $$ = create_basic_function_def(line_num, col_num, $1, $2, NULL, $5); }
    | VOID ID LPAREN RPAREN function_body { $$ = create_void_function_def(line_num, col_num, $2, NULL, $5); }
    ;

function_param_list
    : var_decl { $$ = create_function_param_list(line_num, col_num, $1, NULL); }
    | function_param_list COMMA var_decl { $$ = create_function_param_list(line_num, col_num, $3, $1); }
    ;

function_body
    : LBRACE var_decl_stmt_list stmt_list return_stmt RBRACE { $$ = create_function_body(line_num, col_num, $2, $3, $4); }
    ;

function_call
    : ID LPAREN function_arg_list RPAREN { $$ = create_function_call(line_num, col_num, $1, $3); }
    | ID LPAREN RPAREN { $$ = create_function_call(line_num, col_num, $1, NULL); }
    ;

function_arg_list
    : expr { $$ = create_function_arg_list(line_num, col_num, $1, NULL); }
    | function_arg_list COMMA expr { $$ = create_function_arg_list(line_num, col_num, $3, $1); }
    ;

stmt_list
    : /* empty */ { $$ = NULL; }
    | stmt_list stmt  { $$ = create_stmt_list(line_num, col_num, $2, $1); }
    ;

stmt
    : expr_stmt { $$ = create_stmt(line_num, col_num, EXPR_STMT, $1); }
    | compound_stmt { $$ = create_stmt(line_num, col_num, COMPOUND_STMT, $1); }
    | select_stmt { $$ = create_stmt(line_num, col_num, SELECT_STMT, $1); }
    | iter_stmt { $$ = create_stmt(line_num, col_num, ITER_STMT, $1); }
    | return_stmt { $$ = create_stmt(line_num, col_num, RETURN_STMT, $1); }
    | SEMICOLON { $$ = NULL; }
    ;

expr_stmt
    : expr SEMICOLON  { $$ = create_expr_stmt(line_num, col_num, $1); }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE { $$ = create_compound_stmt(line_num, col_num, $2); }
    ;

select_stmt
    : IF LPAREN expr RPAREN stmt { $$ = create_select_stmt(line_num, col_num, $3, $5, NULL); }
    | IF LPAREN expr RPAREN stmt ELSE stmt { $$ = create_select_stmt(line_num, col_num, $3, $5, $7); }
    ;

iter_stmt
    : WHILE LPAREN expr RPAREN stmt  { $$ = create_iter_stmt(line_num, col_num, NULL, $3, NULL, $5); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, $3, $5, $7, $9); }
    | FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, NULL, $4, $6, $8); }
    | FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, $3, NULL, $6, $8); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, $3, $5, NULL, $8); }
    | FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, NULL, NULL, $5, $7); }
    | FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, $3, NULL, NULL, $7); }
    | FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt { $$ = create_iter_stmt(line_num, col_num, NULL, $4, NULL,  $7); }
    | FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt  { $$ = create_iter_stmt(line_num, col_num, NULL, NULL, NULL, $6); }
    ;

return_stmt
    : RETURN expr SEMICOLON { $$ = create_return_stmt(line_num, col_num, $2); }
    | RETURN SEMICOLON { $$ = create_return_stmt(line_num, col_num, NULL); }
    ;

expr
    : assign_expr { $$ = $1; }
    ;

assign_expr
    : logical_or_expr { $$ = $1; }
    | var ASSIGN assign_expr { $$ = create_assign_expr(line_num, col_num, $1, $3); }
    ;

logical_or_expr
    : logical_or_expr OR logical_and_expr { $$ = create_logical_or_expr(line_num, col_num, $1, $3); }
    | logical_and_expr { $$ = $1; }
    ;

logical_and_expr
    : logical_and_expr AND equality_expr { $$ = create_logical_and_expr(line_num, col_num, $1, $3); }
    | equality_expr { $$ = $1; }
    ;

equality_expr
    : equality_expr EQ relational_expr { $$ = create_equality_expr(line_num, col_num, EQUALITY_EXPR_EQUAL, $1, $3); }
    | equality_expr NE relational_expr { $$ = create_equality_expr(line_num, col_num, EQUALITY_EXPR_NOT_EQUAL, $1, $3); }
    | relational_expr { $$ = $1; }
    ;

relational_expr
    : relational_expr LT additive_expr { $$ = create_relational_expr(line_num, col_num, RELATIONAL_EXPR_LESS_THAN, $1, $3); }
    | relational_expr LE additive_expr { $$ = create_relational_expr(line_num, col_num, RELATIONAL_EXPR_LESS_THAN_OR_EQUAL, $1, $3); }
    | relational_expr GT additive_expr { $$ = create_relational_expr(line_num, col_num, RELATIONAL_EXPR_GREATER_THAN, $1, $3); }
    | relational_expr GE additive_expr { $$ = create_relational_expr(line_num, col_num, RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL, $1, $3); }
    | additive_expr { $$ = $1; }
    ;

additive_expr
    : additive_expr PLUS multiplicative_expr { $$ = create_additive_expr(line_num, col_num, ADDITIVE_EXPR_ADD, $1, $3); }
    | additive_expr MINUS multiplicative_expr { $$ = create_additive_expr(line_num, col_num, ADDITIVE_EXPR_SUBTRACT, $1, $3); }
    | multiplicative_expr { $$ = $1; }
    ;

multiplicative_expr
    : multiplicative_expr ASTERISK unary_expr { $$ = create_multiplicative_expr(line_num, col_num, MULTIPLICATIVE_EXPR_MULTIPLY, $1, $3); }
    | multiplicative_expr SLASH unary_expr { $$ = create_multiplicative_expr(line_num, col_num, MULTIPLICATIVE_EXPR_DIVIDE, $1, $3); }
    | multiplicative_expr PERCENT unary_expr { $$ = create_multiplicative_expr(line_num, col_num, MULTIPLICATIVE_EXPR_MODULO, $1, $3); }
    | unary_expr { $$ = $1; }
    ;

unary_expr
    : SIZEOF LPAREN unary_expr RPAREN { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_SIZEOF_UNARY, $3); }
    | SIZEOF LPAREN basic_type RPAREN { $$ = create_unary_expr_sizeof_basic(line_num, col_num, $3); }
    | NOT unary_expr { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_NOT_UNARY, $2); }
    | PLUS unary_expr { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_POSITIVE, $2); }
    | MINUS unary_expr { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_NEGATIVE, $2); }
    | INCREMENT unary_expr { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_PRE_INCREMENT, $2); }
    | DECREMENT unary_expr { $$ = create_unary_expr(line_num, col_num, UNARY_EXPR_PRE_DECREMENT, $2); }
    | postfix_expr { $$ = $1; }
    ;

postfix_expr
    : var { $$ = create_postfix_expr_var(line_num, col_num, $1); }
    | constant { $$ = create_postfix_expr_constant(line_num, col_num, $1); }
    | postfix_expr INCREMENT { create_postfix_expr(line_num, col_num, POSTFIX_EXPR_POST_INCREMENT, $1); }
    | postfix_expr DECREMENT { create_postfix_expr(line_num, col_num, POSTFIX_EXPR_POST_DECREMENT, $1); }
    | LPAREN expr RPAREN { $$ = create_postfix_expr(line_num, col_num, POSTFIX_EXPR_ENCLOSED, $2); }
    | function_call { $$ = create_postfix_expr_function_call(line_num, col_num, $1); }
    ;

var
    : ID { $$ = create_var_identifier(line_num, col_num, $1); }
    | var LBRACKET expr RBRACKET  { $$ = create_var_subscript(line_num, col_num, $1, $3); }
    | var PERIOD ID { $$ = create_var_field(line_num, col_num, $1, $3); }
    ;

constant
    : CONST_CHAR { $$ = create_constant_char(line_num, col_num, $1); }
    | CONST_FLOAT { $$ = create_constant_float(line_num, col_num, $1); }
    | CONST_INT { $$ = create_constant_int(line_num, col_num, $1); }
    ;

basic_type
    : CHAR { $$ = CHAR_TYPE; }
    | FLOAT { $$ = FLOAT_TYPE; }
    | INT { $$ = INT_TYPE; }
    ;

%%

struct program *parse_file
(FILE *in)
{
        yyin = in;
        yyparse();
        free_custom_types();
        if (n_errors) {
                fprintf(stderr, "cflatc: %d syntax errors\n", n_errors);
                exit(EXIT_FAILURE);
        }
        return program;
}

static void yyerror
(const char *s)
{
        fprintf(stderr, "stdin:%d:%d: error: unexpected token '%s'\n", line_num, col_num, yytext);
        fprintf(stderr, "%s\n", line_buf);
}
