%{
#include <stdio.h>
extern FILE *yyin;
extern int yylex(void);
void yyerror(const char *);
%}

%union {
    char cval;
    float fval;
    int ival;
    char *sval;
}
%token CHAR ELSE FLOAT FOR IF INT RETURN STRUCT TYPEDEF VOID WHILE 
%token LBRACE RBRACE LBRACKET RBRACKET LPAREN RPAREN SEMICOLON
%token ASSIGN OR AND NOT EQ NE LT LE GT GE PLUS MINUS ASTERISK SLASH PERCENT
%token SIZEOF INCREMENT DECREMENT PERIOD
%token <cval> CONST_CHAR
%token <fval> CONST_FLOAT
%token <ival> CONST_INT
%token <sval> ID

%%

program 
    : type_decl_list var_decl_list function_def_list { printf("type_decl_list var_decl_list function_def_list\n"); }
    ; 

type_decl_list
    : type_decl { printf("type_decl\n"); }
    | type_decl_list type_decl { printf("type_decl_list type_decl\n"); }
    ;

type_decl
    : TYPEDEF basic_type ID SEMICOLON { printf("TYPEDEF basic_type ID SEMICOLON\n"); }
    ;

var_decl_list
    : var_decl { printf("var_decl\n"); }
    | var_decl_list var_decl { printf("var_decl_list var_decl\n"); }
    ;

var_decl
    : var_type ID SEMICOLON { printf("var_type ID SEMICOLON\n"); }
    | var_type LBRACKET CONST_INT RBRACKET ID SEMICOLON { printf("var_type LBRACKET CONST_INT RBRACKET ID SEMICOLON\n"); }
    ;

var_type 
    : basic_type { printf("basic_type\n"); }
    | STRUCT LBRACE var_decl_list RBRACE { printf("STRUCT LBRACE var_decl_list RBRACE\n"); }
    ;

function_def_list
    : function_def { printf("function_def\n"); }
    | function_def_list function_def { printf("function_def_list function_def\n"); }
    ;

function_def
    : basic_type ID LPAREN var_decl_list RPAREN function_body { printf("basic_type ID LPAREN var_decl_list RPAREN function_body\n"); }
    | VOID ID LPAREN var_decl_list RPAREN function_body { printf("VOID ID LPAREN var_decl_list RPAREN function_body\n"); }
    ;

function_body
    :  LBRACE var_decl_list return_stmt RBRACE { printf("LBRACE var_decl_list return_stmt RBRACE\n"); }
    | LBRACE var_decl_list stmt_list return_stmt RBRACE { printf("LBRACE var_decl_list stmt_list return_stmt RBRACE\n"); }
    ;

function_call
    : ID LPAREN arg_list RPAREN { printf("ID LPAREN arg_list RPAREN\n"); }
    ;

arg_list
    : /* empty */ { printf("empty\n"); }
    | arg_list expr { printf("arg_list expr\n"); }
    ;

stmt_list
    : stmt { printf("stmt\n"); }
    | stmt_list stmt  { printf("stmt_list stmt \n"); }
    ;

stmt
    : expr_stmt { printf("expr_stmt\n"); }
    | compound_stmt { printf("compound_stmt\n"); }
    | select_stmt { printf("select_stmt\n"); }
    | iter_stmt { printf("iter_stmt\n"); }
    | return_stmt { printf("return_stmt\n"); }
    | SEMICOLON { printf("SEMICOLON\n"); }
    ;

expr_stmt
    : expr SEMICOLON  { printf("expr SEMICOLON \n"); }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE { printf("LBRACE stmt_list RBRACE\n"); }
    ;

select_stmt
    : IF LPAREN expr RPAREN stmt { printf("IF LPAREN expr RPAREN stmt\n"); }
    | IF LPAREN expr RPAREN stmt ELSE stmt { printf("IF LPAREN expr RPAREN stmt ELSE stmt\n"); }
    ;

iter_stmt
    : WHILE LPAREN expr RPAREN stmt  { printf("WHILE LPAREN expr RPAREN stmt \n"); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt\n"); }
    | FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt\n"); }
    | FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt\n"); }
    | FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt\n"); }
    | FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt\n"); }
    | FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt\n"); }
    | FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt\n"); }
    | FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt  { printf("FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt \n"); }
    ;

return_stmt
    : RETURN expr SEMICOLON { printf("RETURN expr SEMICOLON\n"); }
    | RETURN SEMICOLON { printf("RETURN SEMICOLON\n"); }
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
    | INCREMENT var { printf("INCREMENT var\n"); }
    | DECREMENT var { printf("DECREMENT var\n"); }
    | postfix_expr { printf("postfix_expr\n"); }
    ;

postfix_expr
    : var { printf("var\n"); }
    | postfix_expr INCREMENT { printf("var INCREMENT\n"); }
    | postfix_expr DECREMENT { printf("var DECREMENT\n"); }
    | LPAREN expr RPAREN { printf("LPAREN expr RPAREN\n"); }
    | function_call { printf("function_call\n"); }
    ;

var
    : ID { printf("ID\n"); }
    | var LBRACKET expr RBRACKET  { printf("var LBRACKET expr RBRACKET \n"); }
    | var PERIOD var { printf("var PERIOD var\n"); }
    | constant
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
        return yyparse();
}
