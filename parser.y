%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int line_num;
extern int col_num;
extern char *line_buf;
extern char *yytext;
extern FILE *yyin;
extern int yylex(void);
void yyerror(const char *);

struct program_t *program;
int n_errors = 0;
%}

%union {
    char cval;
    float fval;
    int ival;
    char *sval;
    void *pval;
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

%type <pval> type_decl_list
%type <pval> type_decl
%type <pval> var_decl_list
%type <pval> var_decl_stmt
%type <pval> var_decl
%type <pval> struct_type
%type <pval> array_specifier
%type <pval> function_def_list
%type <pval> function_def
%type <pval> function_param_list
%type <pval> function_body
%type <pval> function_call
%type <pval> arg_list
%type <pval> stmt_list
%type <pval> stmt
%type <pval> expr_stmt
%type <pval> compound_stmt
%type <pval> select_stmt
%type <pval> iter_stmt
%type <pval> return_stmt
%type <pval> expr
%type <pval> assign_expr
%type <pval> logical_or_expr
%type <pval> logical_and_expr
%type <pval> equality_expr
%type <pval> relational_expr
%type <pval> additive_expr
%type <pval> multiplicative_expr
%type <pval> unary_expr
%type <pval> postfix_expr
%type <pval> var
%type <pval> constant
%type <ival> basic_type

/* Expect a single shift/reduce conflict for dangling else. */
%expect 1

%%

program 
    : type_decl_list var_decl_list function_def_list { program = create_program($1, NULL, NULL); }
    ; 

type_decl_list
    : /* empty */ { $$ = NULL; }
    | type_decl_list type_decl { $$ = create_type_decl_list($1, $2); }
    ;

type_decl
    : TYPEDEF basic_type ID SEMICOLON { $$ = create_type_decl($2, $3); }
    ;

var_decl_list
    : /* empty */ { $$ = NULL; }
    | var_decl_list var_decl_stmt { $$ = create_var_decl_list($1, $2); }
    ;

var_decl_stmt
    : var_decl SEMICOLON { $$ = $1 }
    ;

var_decl
    : basic_type ID array_specifier SEMICOLON { $$ = create_var_decl_basic($1, $2, $3); }
    | struct_type ID array_specifier SEMICOLON { $$ = create_var_decl_struct($1, $2, $3); }
    ;

struct_type
    : STRUCT LBRACE var_decl_list RBRACE { $$ = create_struct_type($3); }
    ;

array_specifier
    : /* empty */ { $$ = NULL; }
    | LBRACKET CONST_INT RBRACKET { $$ = create_array_specifier($2); }
    ;

function_def_list
    : function_def { $$ = create_function_def_list($1, NULL); }
    | function_def_list function_def { $$ = create_function_def_list($2, $1); }
    ;

function_def
    : basic_type ID LPAREN function_param_list RPAREN function_body { $$ = create_basic_function_def($1, $2, $4, $6); }
    | VOID ID LPAREN function_param_list RPAREN function_body { $$ = create_void_function_def($2, $4, $6); }
    | basic_type ID LPAREN RPAREN function_body { $$ = create_basic_function_def($1, $2, NULL, $5); }
    | VOID ID LPAREN RPAREN function_body { $$ = create_void_function_def($2, NULL, $5); }
    ;

function_param_list
    : var_decl { $$ = create_function_param_list($1, NULL); }
    | function_param_list COMMA var_decl { $$ = create_function_param_list($3, $1); }
    ;

function_body
    : LBRACE var_decl_list return_stmt RBRACE { $$ = create_function_body($2, NULL, $3); }
    | LBRACE var_decl_list stmt_list return_stmt RBRACE { $$ = create_function_body($2, $3, $4); }
    ;

function_call
    : ID LPAREN arg_list RPAREN { $$ = create_function_call($1, $3); }
    | ID LPAREN RPAREN { $$ = create_function_call($1, NULL); }
    ;

arg_list
    : expr { $$ = $1; }
    | arg_list COMMA expr { $$ = create_arg_list($3, $1); }
    ;

stmt_list
    : stmt { $$ = create_stmt_list($1, NULL); }
    | stmt_list stmt  { $$ = create_stmt_list($2, $1); }
    ;

stmt
    : expr_stmt { $$ = create_stmt(EXPR_STMT, $1); }
    | compound_stmt { $$ = create_stmt(COMPOUND_STMT, $1); }
    | select_stmt { $$ = create_stmt(SELECT_STMT, $1); }
    | iter_stmt { $$ = create_stmt(ITER_STMT, $1); }
    | return_stmt { $$ = create_stmt(RETURN_STMT, $1); }
    | SEMICOLON { $$ = NULL; }
    ;

expr_stmt
    : expr SEMICOLON  { $$ = create_expr_stmt($1); }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE { $$ = create_compound_stmt($2); }
    ;

select_stmt
    : IF LPAREN expr RPAREN stmt { $$ = create_select_stmt($3, $5, NULL); }
    | IF LPAREN expr RPAREN stmt ELSE stmt { $$ = create_select_stmt($3, $5, $7); }
    ;

iter_stmt
    : WHILE LPAREN expr RPAREN stmt  { $$ = create_iter_stmt(NULL, $3, NULL, $5); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt($3, $5, $7, $9); }
    | FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(NULL, $4, $6, $8); }
    | FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt($3, NULL, $6, $8); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt { $$ = create_iter_stmt($3, $5, NULL, $8); }
    | FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt { $$ = create_iter_stmt(NULL, NULL, $5, $7); }
    | FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt { $$ = create_iter_stmt($3, NULL, NULL, $7); }
    | FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt { $$ = create_iter_stmt(NULL, $4, NULL,  $7); }
    | FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt  { $$ = create_iter_stmt(NULL, NULL, NULL, $6); }
    ;

return_stmt
    : RETURN expr SEMICOLON { $$ = create_return_stmt($2); }
    | RETURN SEMICOLON { $$ = create_return_stmt(NULL); }
    ;

expr
    : assign_expr { $$ = $1; }
    ;

assign_expr
    : logical_or_expr { $$ = $1; }
    | var ASSIGN assign_expr { $$ = create_assign_expr($1, $3); }
    ;

logical_or_expr
    : logical_or_expr OR logical_and_expr { $$ = create_logical_or_expr($1, $3); }
    | logical_and_expr { $$ = $1; }
    ;

logical_and_expr
    : logical_and_expr AND equality_expr { $$ = create_logical_and_expr($1, $3); }
    | equality_expr { $$ = $1; }
    ;

equality_expr
    : equality_expr EQ relational_expr { $$ = create_equality_expr(EQUALITY_EXPR_EQUAL, $1, $3); }
    | equality_expr NE relational_expr { $$ = create_equality_expr(EQUALITY_EXPR_NOT_EQUAL, $1, $3); }
    | relational_expr { $$ = $1; }
    ;

relational_expr
    : relational_expr LT additive_expr { $$ = create_relational_expr(RELATIONAL_EXPR_LESS_THAN, $1, $3); }
    | relational_expr LE additive_expr { $$ = create_relational_expr(RELATIONAL_EXPR_LESS_THAN_OR_EQUAL, $1, $3); }
    | relational_expr GT additive_expr { $$ = create_relational_expr(RELATIONAL_EXPR_GREATER_THAN, $1, $3); }
    | relational_expr GE additive_expr { $$ = create_relational_expr(RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL, $1, $3); }
    | additive_expr { $$ = $1; }
    ;

additive_expr
    : additive_expr PLUS multiplicative_expr { $$ = create_additive_expr(ADDITIVE_EXPR_ADD, $1, $3); }
    | additive_expr MINUS multiplicative_expr { $$ = create_additive_expr(ADDITIVE_EXPR_SUBTRACT, $1, $3); }
    | multiplicative_expr { $$ = $1; }
    ;

multiplicative_expr
    : multiplicative_expr ASTERISK unary_expr { $$ = create_multiplicative_expr(MULTIPLICATIVE_EXPR_MULTIPLY, $1, $3); }
    | multiplicative_expr SLASH unary_expr { $$ = create_multiplicative_expr(MULTIPLICATIVE_EXPR_DIVIDE, $1, $3); }
    | multiplicative_expr PERCENT unary_expr { $$ = create_multiplicative_expr(MULTIPLICATIVE_EXPR_MODULO, $1, $3); }
    | unary_expr { $$ = $1; }
    ;

unary_expr
    : SIZEOF LPAREN unary_expr RPAREN { $$ = create_unary_expr(UNARY_EXPR_SIZEOF_UNARY, $3); }
    | SIZEOF LPAREN basic_type RPAREN { $$ = create_unary_expr_sizeof_basic($3); }
    | NOT unary_expr { $$ = create_unary_expr(UNARY_EXPR_NOT_UNARY, $2); }
    | PLUS unary_expr { $$ = create_unary_expr(UNARY_EXPR_POSITIVE, $2); }
    | MINUS unary_expr { $$ = create_unary_expr(UNARY_EXPR_NEGATIVE, $2); }
    | INCREMENT unary_expr { $$ = create_unary_expr(UNARY_EXPR_PRE_INCREMENT, $2); }
    | DECREMENT unary_expr { $$ = create_unary_expr(UNARY_EXPR_PRE_DECREMENT, $2); }
    | postfix_expr { $$ = $1; }
    ;

postfix_expr
    : var { $$ = create_postfix_expr(POSTFIX_EXPR_VAR, $1); }
    | constant { $$ = create_postfix_expr(POSTFIX_EXPR_CONSTANT, $1); }
    | postfix_expr INCREMENT { create_postfix_expr(POSTFIX_EXPR_POST_INCREMENT, $1); }
    | postfix_expr DECREMENT { create_postfix_expr(POSTFIX_EXPR_POST_DECREMENT, $1); }
    | LPAREN expr RPAREN { $$ = create_postfix_expr(POSTFIX_EXPR_ENCLOSED, $2); }
    | function_call { $$ = create_postfix_expr(POSTFIX_EXPR_FUNCTION_CALL, $1); }
    ;

var
    : ID { $$ = create_var_identifier($1); }
    | var LBRACKET expr RBRACKET  { $$ = create_var_subscript($1, $3); }
    | var PERIOD ID { $$ = create_var_field($1, $3); }
    ;

constant
    : CONST_CHAR { $$ = create_constant_char($1); }
    | CONST_FLOAT { $$ = create_constant_float($1); }
    | CONST_INT { $$ = create_constant_int($1); }
    ;

basic_type
    : CHAR { $$ = CHAR_TYPE; }
    | FLOAT { $$ = FLOAT_TYPE; }
    | INT { $$ = INT_TYPE; }
    ;

%%


void yyerror(const char *s)
{
        fprintf(stderr, "stdin:%d:%d: error: unexpected token: %s\n", line_num, col_num, yytext);
        fprintf(stderr, "%s\n", line_buf);
}

int main()
{
        if (yyin == NULL) yyin = stdin;
        yyparse();
        if (n_errors) {
            fprintf(stderr, "%d syntax errors\n", n_errors);
            return 1;
        }
        return 0;
}
