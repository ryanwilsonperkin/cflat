#ifndef CFLAT_AST_H
#define CFLAT_AST_H

typedef signed int cflat_int;
typedef unsigned char cflat_char;
typedef float cflat_float;

union value {
        cflat_char cval;
        cflat_float fval;
        cflat_int ival;
};

enum basic_type {
        CHAR_TYPE,
        FLOAT_TYPE,
        INT_TYPE
};

struct pos {
        int line;
        int column;
};

struct program {
        struct pos pos;
        struct type_decl_list *type_decl_list;
        struct var_decl_stmt_list *var_decl_stmt_list;
        struct function_def_list *function_def_list;
};

struct type_decl_list {
        struct pos pos;
        struct type_decl *type_decl;
        struct type_decl_list *type_decl_list;
};

struct type_decl {
        struct pos pos;
        struct var_decl *var_decl;
};

struct var_decl_stmt_list {
        struct pos pos;
        struct var_decl *var_decl;
        struct var_decl_stmt_list *var_decl_stmt_list;
};

struct var_decl {
        struct pos pos;
        enum {
                BASIC_VAR,
                STRUCT_VAR,
                TYPEDEF_VAR
        } type;
        union {
                enum basic_type basic_var;
                struct struct_type *struct_var; 
                char *typedef_id;
        } val;
        char *id;
        struct array_specifier *array_specifier;
};

struct struct_type {
        struct pos pos;
        char *id;
        struct var_decl_stmt_list *var_decl_stmt_list;
};

struct array_specifier {
        struct pos pos;
        struct constant *constant;
};

struct function_def_list {
        struct pos pos;
        struct function_def *function_def;
        struct function_def_list *function_def_list;
};

struct function_def {
        struct pos pos;
        enum {
                BASIC_TYPE_FUNCTION,
                VOID_FUNCTION
        } type;
        enum basic_type type_specifier;
        char *id;
        struct function_param_list *function_param_list;
        struct function_body *function_body;
};

struct function_param_list {
        struct pos pos;
        struct var_decl *var_decl;
        struct function_param_list *function_param_list;
};

struct function_body {
        struct pos pos;
        struct var_decl_stmt_list *var_decl_stmt_list;
        struct stmt_list *stmt_list;
        struct return_stmt *return_stmt;
};

struct function_call {
        struct pos pos;
        char *id;
        struct function_arg_list *function_arg_list;
};

struct function_arg_list {
        struct pos pos;
        struct expr *expr;
        struct function_arg_list *function_arg_list;
};

struct stmt_list {
        struct pos pos;
        struct stmt *stmt;
        struct stmt_list *stmt_list;
};

struct stmt {
        struct pos pos;
        enum stmt_type {
                EXPR_STMT,
                COMPOUND_STMT,
                SELECT_STMT,
                ITER_STMT,
                RETURN_STMT
        } type;
        union {
                struct expr_stmt *expr_stmt;
                struct compound_stmt *compound_stmt;
                struct select_stmt *select_stmt;
                struct iter_stmt *iter_stmt;
                struct return_stmt *return_stmt;
        } val;
};

struct expr_stmt {
        struct pos pos;
        struct expr *expr;
};

struct compound_stmt {
       struct pos pos;
       struct stmt_list *stmt_list;
};

struct select_stmt {
        struct pos pos;
        enum {
                IF_THEN,
                IF_THEN_ELSE
        } type;
        struct expr *cond;
        struct stmt *stmt_if_true;
        struct stmt *stmt_if_false;
};

struct iter_stmt {
        struct pos pos;
        enum {
                WHILE_EXPR,
                FOR_INIT_COND_AFTER,
                FOR_INIT_AFTER,
                FOR_INIT_COND,
                FOR_COND_AFTER,
                FOR_AFTER,
                FOR_INIT,
                /* FOR_COND is equivalent to WHILE */
                FOR_EMPTY
        } type;
        struct expr *init;
        struct expr *cond;
        struct expr *after;
        struct stmt *body;
};

struct return_stmt {
        struct pos pos;
        struct expr *expr;
};

struct expr {
        struct pos pos;
        enum {
                ASSIGN_EXPR,
                LOGICAL_OR_EXPR,
                LOGICAL_AND_EXPR,
                EQUALITY_EXPR,
                RELATIONAL_EXPR,
                ADDITIVE_EXPR,
                MULTIPLICATIVE_EXPR,
                UNARY_EXPR,
                POSTFIX_EXPR
        } type;
        union {
                enum equality_expr_subtype {
                        EQUALITY_EXPR_EQUAL,
                        EQUALITY_EXPR_NOT_EQUAL
                } equality_expr_subtype;
                enum relational_expr_subtype {
                        RELATIONAL_EXPR_LESS_THAN,
                        RELATIONAL_EXPR_LESS_THAN_OR_EQUAL,
                        RELATIONAL_EXPR_GREATER_THAN,
                        RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL
                } relational_expr_subtype;
                enum additive_expr_subtype {
                        ADDITIVE_EXPR_ADD,
                        ADDITIVE_EXPR_SUBTRACT
                } additive_expr_subtype;
                enum multiplicative_expr_subtype {
                        MULTIPLICATIVE_EXPR_MULTIPLY,
                        MULTIPLICATIVE_EXPR_DIVIDE,
                        MULTIPLICATIVE_EXPR_MODULO
                } multiplicative_expr_subtype;
                enum unary_expr_subtype {
                        UNARY_EXPR_SIZEOF_UNARY,
                        UNARY_EXPR_SIZEOF_BASIC,
                        UNARY_EXPR_NOT_UNARY,
                        UNARY_EXPR_POSITIVE,
                        UNARY_EXPR_NEGATIVE,
                        UNARY_EXPR_PRE_INCREMENT,
                        UNARY_EXPR_PRE_DECREMENT
                } unary_expr_subtype;
                enum postfix_expr_subtype {
                        POSTFIX_EXPR_VAR,
                        POSTFIX_EXPR_CONSTANT,
                        POSTFIX_EXPR_POST_INCREMENT,
                        POSTFIX_EXPR_POST_DECREMENT,
                        POSTFIX_EXPR_ENCLOSED,
                        POSTFIX_EXPR_FUNCTION_CALL
                } postfix_expr_subtype;
        } subtype;
        union {
                struct {
                        struct var *assignee;
                        struct expr *assignment;
                } assign;
                struct {
                        struct expr *primary;
                        struct expr *secondary;
                } relation;
                struct {
                        struct expr *primary;
                        struct expr *secondary;
                } binary_op;
                union {
                        enum basic_type type;
                        struct expr *expr;
                } unary_op;
                union {
                        struct var *var;
                        struct constant *constant;
                        struct expr *expr;
                        struct function_call *function_call;
                } postfix_op;
        } val;
};

struct var {
        struct pos pos;
        enum {
                IDENTIFIER,
                FIELD,
                SUBSCRIPT
        } type;
        union {
                char *id;
                struct {
                        struct var *var;
                        char *id;
                } field;
                struct {
                        struct var *var;
                        struct expr *expr;
                } subscript;
        } val;
};

struct constant {
        struct pos pos;
        enum basic_type type;
        union value val;
};

struct program *create_program(int, int, struct type_decl_list *, struct var_decl_stmt_list *, struct function_def_list *);
struct type_decl_list *create_type_decl_list(int, int, struct type_decl *, struct type_decl_list *);
struct type_decl *create_type_decl(int, int, struct var_decl *var_decl);
struct var_decl_stmt_list *create_var_decl_stmt_list(int, int, struct var_decl *, struct var_decl_stmt_list *);
struct var_decl *create_var_decl_basic(int, int, enum basic_type, char *, struct array_specifier *); 
struct var_decl *create_var_decl_struct(int, int, struct struct_type *, char *, struct array_specifier *); 
struct var_decl *create_var_decl_typedef(int, int, char *, char *, struct array_specifier *); 
struct struct_type *create_struct_type(int, int, char *, struct var_decl_stmt_list *);
struct array_specifier *create_array_specifier(int, int, struct constant *constant);
struct function_def_list *create_function_def_list(int, int, struct function_def *, struct function_def_list *);
struct function_def *create_basic_function_def(int, int, enum basic_type, char *, struct function_param_list *, struct function_body *);
struct function_def *create_void_function_def(int, int, char *, struct function_param_list *, struct function_body *);
struct function_param_list *create_function_param_list(int, int, struct var_decl *, struct function_param_list *);
struct function_body *create_function_body(int, int, struct var_decl_stmt_list *, struct stmt_list *, struct return_stmt *);
struct function_call *create_function_call(int, int, char *id, struct function_arg_list *);
struct function_arg_list *create_function_arg_list(int, int, struct expr *, struct function_arg_list *);
struct stmt_list *create_stmt_list(int, int, struct stmt *, struct stmt_list *);
struct stmt *create_stmt(int, int, enum stmt_type, void *);
struct expr_stmt *create_expr_stmt(int, int, struct expr *);
struct compound_stmt *create_compound_stmt(int, int, struct stmt_list *);
struct select_stmt *create_select_stmt(int, int, struct expr *, struct stmt *, struct stmt *);
struct iter_stmt *create_iter_stmt(int, int, struct expr *, struct expr *, struct expr *, struct stmt *);
struct return_stmt *create_return_stmt(int, int, struct expr *);
struct expr *create_assign_expr(int, int, struct var *, struct expr *);
struct expr *create_logical_or_expr(int, int, struct expr *, struct expr *);
struct expr *create_logical_and_expr(int, int, struct expr *, struct expr *);
struct expr *create_equality_expr(int, int, enum equality_expr_subtype, struct expr *, struct expr *);
struct expr *create_relational_expr(int, int, enum relational_expr_subtype, struct expr *, struct expr *);
struct expr *create_additive_expr(int, int, enum additive_expr_subtype, struct expr *, struct expr *);
struct expr *create_multiplicative_expr(int, int, enum multiplicative_expr_subtype, struct expr *, struct expr *);
struct expr *create_unary_expr_sizeof_basic(int, int, enum basic_type);
struct expr *create_unary_expr(int, int, enum unary_expr_subtype, struct expr *);
struct expr *create_postfix_expr(int, int, enum postfix_expr_subtype, struct expr *);
struct expr *create_postfix_expr_var(int, int, struct var *);
struct expr *create_postfix_expr_constant(int, int, struct constant *);
struct expr *create_postfix_expr_function_call(int, int, struct function_call *);
struct var *create_var_identifier(int, int, char *);
struct var *create_var_field(int, int, struct var *, char *);
struct var *create_var_subscript(int, int, struct var *, struct expr *);
struct constant *create_constant_char(int, int, cflat_char);
struct constant *create_constant_float(int, int, cflat_float);
struct constant *create_constant_int(int, int, cflat_int);

void free_program(struct program *);
void free_type_decl_list(struct type_decl_list *);
void free_type_decl(struct type_decl *);
void free_var_decl_stmt_list(struct var_decl_stmt_list *);
void free_var_decl(struct var_decl *);
void free_struct_type(struct struct_type *);
void free_array_specifier(struct array_specifier *);
void free_function_def_list(struct function_def_list *);
void free_function_def(struct function_def *);
void free_function_param_list(struct function_param_list *);
void free_function_body(struct function_body *);
void free_function_call(struct function_call *);
void free_function_arg_list(struct function_arg_list *);
void free_stmt_list(struct stmt_list *);
void free_stmt(struct stmt *);
void free_expr_stmt(struct expr_stmt *);
void free_compound_stmt(struct compound_stmt *);
void free_select_stmt(struct select_stmt *);
void free_iter_stmt(struct iter_stmt *);
void free_return_stmt(struct return_stmt *);
void free_expr(struct expr *);
void free_var(struct var *);
void free_constant(struct constant *);

#endif  /* CFLAT_AST_H */
