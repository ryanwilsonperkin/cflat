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
  type_decl_list var_decl_list function_def_list { printf("type_decl_list var_decl_list function_def_list"); }
; 

type_decl_list :
  /* empty */ { printf("empty\n"); }
| type_decl { printf("type_decl\n"); }
| type_decl_list type_decl { printf("type_decl_list type_decl"); }
;

type_decl :
  TYPEDEF basic_type ID SEMICOLON { printf("TYPEDEF basic_type ID SEMICOLON"); }
;

var_decl_list :
  /* empty */ { printf("empty\n"); }
| var_decl { printf("var_decl\n"); }
| var_decl_list var_decl { printf("var_decl_list var_decl"); }
;

var_decl :
  var_type ID SEMICOLON { printf("var_type ID SEMICOLON"); }
| var_type LBRACKET INT RBRACKET ID SEMICOLON { printf("var_type LBRACKET INT RBRACKET ID SEMICOLON"); }
;

var_type :
  basic_type { printf("basic_type\n"); }
| STRUCT LBRACE var_decl_list RBRACE { printf("STRUCT LBRACE var_decl_list RBRACE"); }
;

function_def_list :
  /* null */ { printf("null\n"); }
| function_def { printf("function_def\n"); }
| function_def_list function_def { printf("function_def_list function_def"); }
;

function_def :
  function_type ID LPAREN var_decl_list RPAREN function_body { printf("function_type\n"); }
;

function_type :
  VOID { printf("VOID\n"); }
| basic_type { printf("basic_type"); }
;

function_body :
  LBRACE var_decl_list return_stmt RBRACE { printf("LBRACE var_decl_list return_stmt RBRACE"); }
| LBRACE var_decl_list stmt_list return_stmt RBRACE { printf("LBRACE var_decl_list stmt_list return_stmt RBRACE"); }
;

function_call :
  ID LPAREN arg_list RPAREN { printf("ID LPAREN arg_list RPAREN"); }
;

arg_list :
  /* null */ { printf("null"); }
| expr { printf("expr"); }
| arg_list expr { printf("arg_list expr"); }
;

stmt_list :
  stmt { printf("stmt"); }
| stmt_list stmt  { printf("stmt_list stmt "); }
;

stmt :
  expr_stmt { printf("expr_stmt"); }
| compound_stmt { printf("compound_stmt"); }
| select_stmt { printf("select_stmt"); }
| iter_stmt { printf("iter_stmt"); }
| return_stmt { printf("return_stmt"); }
| SEMICOLON { printf("SEMICOLON"); }
;

expr_stmt :
  expr SEMICOLON  { printf("expr SEMICOLON "); }
;

compound_stmt :
  LBRACE stmt_list RBRACE { printf("LBRACE stmt_list RBRACE"); }
;

select_stmt :
  IF LPAREN expr RPAREN stmt { printf("IF LPAREN expr RPAREN stmt"); }
| IF LPAREN expr RPAREN stmt ELSE stmt { printf("IF LPAREN expr RPAREN stmt ELSE stmt"); }
;

iter_stmt :
  WHILE LPAREN expr RPAREN stmt  { printf("WHILE LPAREN expr RPAREN stmt "); }
| FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt"); }
| FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt"); }
| FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt"); }
| FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt"); }
| FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt { printf("FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt"); }
| FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt { printf("FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt"); }
| FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt { printf("FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt"); }
| FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt  { printf("FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt "); }
;

return_stmt :
  RETURN expr SEMICOLON { printf("RETURN expr SEMICOLON"); }
| RETURN SEMICOLON { printf("RETURN SEMICOLON"); }
;

expr :
  expr_l1 { printf("expr_l1"); }
;

expr_l1 :
  expr_l2 { printf("expr_l2"); }
| var ASSIGN expr_l1 { printf("var ASSIGN expr_l1"); }
;

expr_l2 :
  expr_l2 OR expr_l3 { printf("expr_l2 OR expr_l3"); }
| expr_l3 { printf("expr_l3"); }
;

expr_l3 :
  expr_l3 AND expr_l4 { printf("expr_l3 AND expr_l4"); }
| expr_l4 { printf("expr_l4"); }
;

expr_l4 :
  expr_l4 EQ expr_l5 { printf("expr_l4 EQ expr_l5"); }
| expr_l4 NE expr_l5 { printf("expr_l4 NE expr_l5"); }
| expr_l5 { printf("expr_l5"); }
;

expr_l5 :
  expr_l5 LT expr_l6 { printf("expr_l5 LT expr_l6"); }
| expr_l5 LE expr_l6 { printf("expr_l5 LE expr_l6"); }
| expr_l5 GT expr_l6 { printf("expr_l5 GT expr_l6"); }
| expr_l5 GE expr_l6 { printf("expr_l5 GE expr_l6"); }
| expr_l6 { printf("expr_l6"); }
;

expr_l6 :
  expr_l6 PLUS expr_l7 { printf("expr_l6 PLUS expr_l7"); }
| expr_l6 MINUS expr_l7 { printf("expr_l6 MINUS expr_l7"); }
| expr_l7 { printf("expr_l7"); }
;

expr_l7 :
  expr_l7 ASTERISK expr_l8 { printf("expr_l7 ASTERISK expr_l8"); }
| expr_l7 SLASH expr_l8 { printf("expr_l7 SLASH expr_l8"); }
| expr_l7 PERCENT expr_l8 { printf("expr_l7 PERCENT expr_l8"); }
| expr_l8 { printf("expr_l8"); }
;

expr_l8 :
  SIZEOF LPAREN expr_l8 RPAREN { printf("SIZEOF LPAREN expr_l8 RPAREN"); }
| SIZEOF LPAREN basic_type RPAREN { printf("SIZEOF LPAREN basic_type RPAREN"); }
| NOT expr_l8 { printf("NOT expr_l8"); }
| PLUS expr_l8 { printf("PLUS expr_l8"); }
| MINUS expr_l8 { printf("MINUS expr_l8"); }
| INCREMENT var { printf("INCREMENT var"); }
| DECREMENT var { printf("DECREMENT var"); }
| expr_l9 { printf("expr_l9"); }
;

expr_l9 :
  var { printf("var"); }
| var INCREMENT { printf("var INCREMENT"); }
| var DECREMENT { printf("var DECREMENT"); }
| LPAREN expr RPAREN { printf("LPAREN expr RPAREN"); }
| function_call { printf("function_call"); }
| constant { printf("constant"); }
;

var :
  ID { printf("ID"); }
| var LBRACKET expr RBRACKET  { printf("var LBRACKET expr RBRACKET "); }
| var PERIOD var { printf("var PERIOD var"); }
;

constant :
  CONST_CHAR { printf("CONST_CHAR"); }
| CONST_FLOAT { printf("CONST_FLOAT"); }
| CONST_INT { printf("CONST_INT"); }
;

basic_type :
  CHAR { printf("CHAR"); }
| FLOAT { printf("FLOAT"); }
| INT { printf("INT"); }
;

%%


void yyerror(const char *s) { fprintf(stderr, "%s\n", s); }

int main()
{
        if (yyin == NULL) yyin = stdin;
        return yyparse();
}
