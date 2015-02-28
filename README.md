cflat
===

Terminals
---
- CHAR
- ELSE
- FLOAT
- FOR
- IF
- INT
- RETURN
- STRUCT
- TYPEDEF
- VOID
- WHILE
- CONST_CHAR
- CONST_FLOAT
- CONST_INT
- PLUS
- MINUS
- ASTERISK
- SLASH
- PERCENT
- EQ
- NE
- LT
- GT
- LE
- GE
- AND
- OR
- NOT
- ASSIGN
- SIZEOF
- INCREMENT
- DECREMENT
- LBRACE
- RBRACE
- LBRACKET
- RBRACKET
- LPAREN
- RPAREN
- SEMICOLON
- PERIOD
- ID

Grammar
---

### program
- type_decl_list global_var_list function_def_list

### function_def_list
- function_def
- function_def_list function_def

### stmt_list
- stmt
- stmt_list stmt 

### stmt
- expr_stmt
- compound_stmt
- select_stmt
- iter_stmt
- return_stmt
- SEMICOLON

### expr_stmt
- expr SEMICOLON 

### compound_stmt
- LBRACE stmt_list RBRACE

### select_stmt
- IF LPAREN expr RPAREN stmt
- IF LPAREN expr RPAREN stmt ELSE stmt

### iter_stmt
- WHILE LPAREN expr RPAREN stmt 
- FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN stmt
- FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN stmt
- FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN stmt
- FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN stmt
- FOR LPAREN SEMICOLON SEMICOLON expr RPAREN stmt
- FOR LPAREN expr SEMICOLON SEMICOLON RPAREN stmt
- FOR LPAREN SEMICOLON expr SEMICOLON RPAREN stmt
- FOR LPAREN SEMICOLON SEMICOLON RPAREN stmt 

### return_stmt
- RETURN expr SEMICOLON
- RETURN SEMICOLON

### expr
- expr PLUS expr
- expr MINUS expr
- expr ASTERISK expr
- expr SLASH expr
- expr PERCENT expr
- expr LT expr
- expr GT expr
- expr GE expr
- expr LE expr
- expr NE expr
- expr EQ expr
- expr OR expr
- expr AND expr
- NOT expr 
- MINUS expr
- PLUS expr
- LPAREN expr RPAREN
- function_call
- var ASSIGN expr
- var
- var INCREMENT
- var DECREMENT
- INCREMENT var
- DECREMENT var
- SIZEOF LPAREN expr RPAREN
- SIZEOF LPAREN type_name RPAREN
- constant

### var
- ID
- var LBRACKET expr RBRACKET 
- var PERIOD var

### constant
- CONST_CHAR
- CONST_FLOAT
- CONST_INT
