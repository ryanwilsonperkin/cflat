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
- basic_type ID LPAREN var_decl_list RPAREN function_body
- VOID ID LPAREN var_decl_list RPAREN function_body

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
- assign_expr

### assign_expr
- logical_or_expr
- var ASSIGN assign_expr

### logical_or_expr
- logical_or_expr OR logical_and_expr
- logical_and_expr

### logical_and_expr
- logical_and_expr AND equality_expr
- equality_expr

### equality_expr
- equality_expr EQ relational_expr
- equality_expr NE relational_expr
- relational_expr

### relational_expr
- relational_expr LT additive_expr
- relational_expr LE additive_expr
- relational_expr GT additive_expr
- relational_expr GE additive_expr
- additive_expr

### additive_expr
- additive_expr PLUS multiplicative_expr
- additive_expr MINUS multiplicative_expr
- multiplicative_expr

### multiplicative_expr
- multiplicative_expr ASTERISK unary_expression
- multiplicative_expr SLASH unary_expression
- multiplicative_expr PERCENT unary_expression
- unary_expression

### unary_expression
- SIZEOF LPAREN unary_expression RPAREN
- SIZEOF LPAREN basic_type RPAREN
- NOT unary_expression
- PLUS unary_expression
- MINUS unary_expression
- INCREMENT var
- DECREMENT var
- postfix_expr

### postfix_expr
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
