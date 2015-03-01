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

Non-Terminals
---

- program
- type_decl_list
- type_decl
- var_decl_list
- ver_decl
- var_type
- function_def_list
- function_def
- function_type
- function_body
- function_call
- arg_list
- stmt_list
- stmt
- expr_stmt
- compound_stmt
- select_stmt
- iter_stmt
- return_stmt
- expr
- var
- constant
- basic_type

Grammar
---

### program
- type_decl_list var_decl_list function_def_list

### type_decl_list
- type_decl
- type_decl_list type_decl

### type_decl
- TYPEDEF basic_type ID SEMICOLON

### var_decl_list
- var_decl
- var_decl_list var_decl

### var_decl
- var_type ID SEMICOLON
- var_type LBRACKET INT RBRACKET ID SEMICOLON

### var_type
- basic_type
- STRUCT LBRACE var_decl_list RBRACE

### function_def_list
- function_def
- function_def_list function_def

### function_def
- function_type ID LPAREN var_decl_list RPAREN function_body

### function_type
- VOID
- basic_type

### function_body
- LBRACE var_decl_list return_stmt RBRACE
- LBRACE var_decl_list stmt_list return_stmt RBRACE

### function_call
- ID LPAREN arg_list RPAREN

### arg_list
- null
- arg_list expr

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
- expr_l1

### expr_l1
- expr_l2
- var ASSIGN expr_l1

### expr_l2
- expr_l2 OR expr_l3
- expr_l3

### expr_l3
- expr_l3 AND expr_l4
- expr_l4

### expr_l4
- expr_l4 EQ expr_l5
- expr_l4 NE expr_l5
- expr_l5

### expr_l5
- expr_l5 LT expr_l6
- expr_l5 LE expr_l6
- expr_l5 GT expr_l6
- expr_l5 GE expr_l6
- expr_l6

### expr_l6
- expr_l6 PLUS expr_l7
- expr_l6 MINUS expr_l7
- expr_l7

### expr_l7
- expr_l7 ASTERISK expr_l8
- expr_l7 SLASH expr_l8
- expr_l7 PERCENT expr_l8
- expr_l8

### expr_l8
- SIZEOF LPAREN expr_l8 RPAREN
- SIZEOF LPAREN basic_type RPAREN
- NOT expr_l8
- PLUS expr_l8
- MINUS expr_l8
- INCREMENT var
- DECREMENT var
- expr_l9

### expr_l9
- var
- var INCREMENT
- var DECREMENT
- LPAREN expr RPAREN
- function_call
- constant

### var
- ID
- var LBRACKET expr RBRACKET 
- var PERIOD var

### constant
- CONST_CHAR
- CONST_FLOAT
- CONST_INT

### basic_type
- CHAR
- FLOAT
- INT
