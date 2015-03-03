%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE *yyin;
extern int yylex(void);
void yyerror(const char *);

struct program_t *program;
%}

%union {
    char cval;
    float fval;
    int ival;
    char *sval;
    void *pval;
}
%token CHAR ELSE FLOAT FOR IF INT RETURN STRUCT TYPEDEF VOID WHILE 
%token LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN SEMICOLON
%token ASSIGN OR AND NOT EQ NE LT LE GT GE PLUS MINUS ASTERISK SLASH PERCENT
%token SIZEOF INCREMENT DECREMENT PERIOD
%token <cval> CONST_CHAR
%token <fval> CONST_FLOAT
%token <ival> CONST_INT
%token <sval> ID

%type <pval> type_decl_list
%type <pval> type_decl
%type <pval> var_decl_list
%type <pval> var_decl
%type <pval> struct_type
%type <pval> array_specifier
%type <pval> function_def_list
%type <pval> function_def
%type <pval> function_params
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
    | var_decl_list var_decl { $$ = create_var_decl_list($1, $2); }
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
    : basic_type ID function_params function_body { $$ = create_basic_function_def($1, $2, $3, $4); }
    | VOID ID function_params function_body { $$ = create_void_function_def($2, $3, $4); }
    ;

function_params
    : LPAREN var_decl_list RPAREN { $$ = create_function_params($2); }
    ;

function_body
    : LBRACE var_decl_list return_stmt RBRACE { $$ = create_function_body($2, NULL, $3); }
    | LBRACE var_decl_list stmt_list return_stmt RBRACE { $$ = create_function_body($2, $3, $4); }
    ;

function_call
    : ID LPAREN arg_list RPAREN { $$ = create_function_call($1, $3); }
    ;

arg_list
    : /* empty */ { $$ = NULL; }
    | arg_list expr SEMICOLON { $$ = create_arg_list($2, $1); }
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
    : assign_expr { printf("assign_expr\n"); }
    ;

assign_expr
    : logical_or_expr { printf("logical_or_expr\n"); }
    | var ASSIGN assign_expr { printf("var ASSIGN assign_expr\n"); }
    ;

logical_or_expr
    : logical_or_expr OR logical_and_expr { printf("logical_or_expr OR logical_and_expr\n"); }
    | logical_and_expr { printf("logical_and_expr\n"); }
    ;

logical_and_expr
    : logical_and_expr AND equality_expr { printf("logical_and_expr AND equality_expr\n"); }
    | equality_expr { printf("equality_expr\n"); }
    ;

equality_expr
    : equality_expr EQ relational_expr { printf("equality_expr EQ relational_expr\n"); }
    | equality_expr NE relational_expr { printf("equality_expr NE relational_expr\n"); }
    | relational_expr { printf("relational_expr\n"); }
    ;

relational_expr
    : relational_expr LT additive_expr { printf("relational_expr LT additive_expr\n"); }
    | relational_expr LE additive_expr { printf("relational_expr LE additive_expr\n"); }
    | relational_expr GT additive_expr { printf("relational_expr GT additive_expr\n"); }
    | relational_expr GE additive_expr { printf("relational_expr GE additive_expr\n"); }
    | additive_expr { printf("additive_expr\n"); }
    ;

additive_expr
    : additive_expr PLUS multiplicative_expr { printf("additive_expr PLUS multiplicative_expr\n"); }
    | additive_expr MINUS multiplicative_expr { printf("additive_expr MINUS multiplicative_expr\n"); }
    | multiplicative_expr { printf("multiplicative_expr\n"); }
    ;

multiplicative_expr
    : multiplicative_expr ASTERISK unary_expr { printf("multiplicative_expr ASTERISK unary_expr\n"); }
    | multiplicative_expr SLASH unary_expr { printf("multiplicative_expr SLASH unary_expr\n"); }
    | multiplicative_expr PERCENT unary_expr { printf("multiplicative_expr PERCENT unary_expr\n"); }
    | unary_expr { printf("unary_expr\n"); }
    ;

unary_expr
    : SIZEOF LPAREN unary_expr RPAREN { printf("SIZEOF LPAREN unary_expr RPAREN\n"); }
    | SIZEOF LPAREN basic_type RPAREN { printf("SIZEOF LPAREN basic_type RPAREN\n"); }
    | NOT unary_expr { printf("NOT unary_expr\n"); }
    | PLUS unary_expr { printf("PLUS unary_expr\n"); }
    | MINUS unary_expr { printf("MINUS unary_expr\n"); }
    | INCREMENT unary_expr { printf("INCREMENT unary_expr\n"); }
    | DECREMENT unary_expr { printf("DECREMENT unary_expr\n"); }
    | postfix_expr { printf("postfix_expr\n"); }
    ;

postfix_expr
    : var { printf("var\n"); }
    | constant { printf("constant\n"); }
    | postfix_expr INCREMENT { printf("var INCREMENT\n"); }
    | postfix_expr DECREMENT { printf("var DECREMENT\n"); }
    | LPAREN expr RPAREN { printf("LPAREN expr RPAREN\n"); }
    | function_call { printf("function_call\n"); }
    ;

var
    : ID { printf("ID\n"); }
    | var LBRACKET expr RBRACKET  { printf("ID LBRACKET expr RBRACKET \n"); }
    | var PERIOD ID { printf("ID PERIOD var\n"); }
    ;

constant
    : CONST_CHAR { printf("CONST_CHAR\n"); }
    | CONST_FLOAT { printf("CONST_FLOAT\n"); }
    | CONST_INT { printf("CONST_INT\n"); }
    ;

basic_type
    : CHAR { printf("CHAR\n"); }
    | FLOAT { printf("FLOAT\n"); }
    | INT { printf("INT\n"); }
    ;

%%


void yyerror(const char *s) { fprintf(stderr, "%s\n", s); }

int main()
{
        if (yyin == NULL) yyin = stdin;
        yyparse();
}
