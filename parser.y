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

program : 
  type_decl_list var_decl_list function_def_list { printf("type_decl_list var_decl_list function_def_list\n"); }
; 

type_decl_list :
  type_decl { printf("type_decl\n"); }
| type_decl_list type_decl { printf("type_decl_list type_decl\n"); }
;

type_decl :
  TYPEDEF basic_type ID SEMICOLON { printf("TYPEDEF basic_type ID SEMICOLON\n"); }
;

var_decl_list :
  var_decl { printf("var_decl\n"); }
| var_decl_list var_decl { printf("var_decl_list var_decl\n"); }
;

var_decl :
  var_type ID SEMICOLON { printf("var_type ID SEMICOLON\n"); }
| var_type LBRACKET INT RBRACKET ID SEMICOLON { printf("var_type LBRACKET INT RBRACKET ID SEMICOLON\n"); }
;

var_type :
  basic_type { printf("basic_type\n"); }
| STRUCT LBRACE var_decl_list RBRACE { printf("STRUCT LBRACE var_decl_list RBRACE\n"); }
;

function_def_list :
  function_def { printf("function_def\n"); }
| function_def_list function_def { printf("function_def_list function_def\n"); }
;

function_def :
  function_type ID LPAREN var_decl_list RPAREN function_body { printf("function_type\n"); }
;

function_type :
  VOID { printf("VOID\n"); }
| basic_type { printf("basic_type\n"); }
;

function_body :
  LBRACE var_decl_list return_stmt RBRACE { printf("LBRACE var_decl_list return_stmt RBRACE\n"); }
| LBRACE var_decl_list stmt_list return_stmt RBRACE { printf("LBRACE var_decl_list stmt_list return_stmt RBRACE\n"); }
;

function_call :
  ID LPAREN arg_list RPAREN { printf("ID LPAREN arg_list RPAREN\n"); }
;

arg_list :
  /* empty */ { printf("empty\n"); }
| arg_list expr { printf("arg_list expr\n"); }
;

stmt_list :
  stmt { printf("stmt\n"); }
| stmt_list stmt  { printf("stmt_list stmt \n"); }
;

stmt :
  expr_stmt { printf("expr_stmt\n"); }
| compound_stmt { printf("compound_stmt\n"); }
| select_stmt { printf("select_stmt\n"); }
| iter_stmt { printf("iter_stmt\n"); }
| return_stmt { printf("return_stmt\n"); }
| SEMICOLON { printf("SEMICOLON\n"); }
;

expr_stmt :
  expr SEMICOLON  { printf("expr SEMICOLON \n"); }
;

compound_stmt :
  LBRACE stmt_list RBRACE { printf("LBRACE stmt_list RBRACE\n"); }
;

select_stmt :
  IF LPAREN expr RPAREN stmt { printf("IF LPAREN expr RPAREN stmt\n"); }
| IF LPAREN expr RPAREN stmt ELSE stmt { printf("IF LPAREN expr RPAREN stmt ELSE stmt\n"); }
;

iter_stmt :
  WHILE LPAREN expr RPAREN stmt  { printf("WHILE LPAREN expr RPAREN stmt \n"); }
| FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt\n"); }
| FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt\n"); }
| FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt\n"); }
| FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt\n"); }
| FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt\n"); }
| FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt\n"); }
| FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt\n"); }
| FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt  { printf("FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt \n"); }
;

return_stmt :
  RETURN expr SEMICOLON { printf("RETURN expr SEMICOLON\n"); }
| RETURN SEMICOLON { printf("RETURN SEMICOLON\n"); }
;

expr :
  expr_l1 { printf("expr_l1\n"); }
;

expr_l1 :
  expr_l2 { printf("expr_l2\n"); }
| var ASSIGN expr_l1 { printf("var ASSIGN expr_l1\n"); }
;

expr_l2 :
  expr_l2 OR expr_l3 { printf("expr_l2 OR expr_l3\n"); }
| expr_l3 { printf("expr_l3\n"); }
;

expr_l3 :
  expr_l3 AND expr_l4 { printf("expr_l3 AND expr_l4\n"); }
| expr_l4 { printf("expr_l4\n"); }
;

expr_l4 :
  expr_l4 EQ expr_l5 { printf("expr_l4 EQ expr_l5\n"); }
| expr_l4 NE expr_l5 { printf("expr_l4 NE expr_l5\n"); }
| expr_l5 { printf("expr_l5\n"); }
;

expr_l5 :
  expr_l5 LT expr_l6 { printf("expr_l5 LT expr_l6\n"); }
| expr_l5 LE expr_l6 { printf("expr_l5 LE expr_l6\n"); }
| expr_l5 GT expr_l6 { printf("expr_l5 GT expr_l6\n"); }
| expr_l5 GE expr_l6 { printf("expr_l5 GE expr_l6\n"); }
| expr_l6 { printf("expr_l6\n"); }
;

expr_l6 :
  expr_l6 PLUS expr_l7 { printf("expr_l6 PLUS expr_l7\n"); }
| expr_l6 MINUS expr_l7 { printf("expr_l6 MINUS expr_l7\n"); }
| expr_l7 { printf("expr_l7\n"); }
;

expr_l7 :
  expr_l7 ASTERISK expr_l8 { printf("expr_l7 ASTERISK expr_l8\n"); }
| expr_l7 SLASH expr_l8 { printf("expr_l7 SLASH expr_l8\n"); }
| expr_l7 PERCENT expr_l8 { printf("expr_l7 PERCENT expr_l8\n"); }
| expr_l8 { printf("expr_l8\n"); }
;

expr_l8 :
  SIZEOF LPAREN expr_l8 RPAREN { printf("SIZEOF LPAREN expr_l8 RPAREN\n"); }
| SIZEOF LPAREN basic_type RPAREN { printf("SIZEOF LPAREN basic_type RPAREN\n"); }
| NOT expr_l8 { printf("NOT expr_l8\n"); }
| PLUS expr_l8 { printf("PLUS expr_l8\n"); }
| MINUS expr_l8 { printf("MINUS expr_l8\n"); }
| INCREMENT var { printf("INCREMENT var\n"); }
| DECREMENT var { printf("DECREMENT var\n"); }
| expr_l9 { printf("expr_l9\n"); }
;

expr_l9 :
  var { printf("var\n"); }
| var INCREMENT { printf("var INCREMENT\n"); }
| var DECREMENT { printf("var DECREMENT\n"); }
| LPAREN expr RPAREN { printf("LPAREN expr RPAREN\n"); }
| function_call { printf("function_call\n"); }
| constant { printf("constant\n"); }
;

var :
  ID { printf("ID\n"); }
| var LBRACKET expr RBRACKET  { printf("var LBRACKET expr RBRACKET \n"); }
| var PERIOD var { printf("var PERIOD var\n"); }
;

constant :
  CONST_CHAR { printf("CONST_CHAR\n"); }
| CONST_FLOAT { printf("CONST_FLOAT\n"); }
| CONST_INT { printf("CONST_INT\n"); }
;

basic_type :
  CHAR { printf("CHAR\n"); }
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
