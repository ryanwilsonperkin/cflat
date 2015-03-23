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

struct pos_t {
        int line;
        int column;
};

struct program_t {
        struct pos_t pos;
        struct type_decl_list_t *type_decl_list;
        struct var_decl_stmt_list_t *var_decl_stmt_list;
        struct function_def_list_t *function_def_list;
};

struct type_decl_list_t {
        struct pos_t pos;
        struct type_decl_t *type_decl;
        struct type_decl_list_t *type_decl_list;
};

struct type_decl_t {
        struct pos_t pos;
        struct var_decl_t *var_decl;
};

struct var_decl_stmt_list_t {
        struct pos_t pos;
        struct var_decl_t *var_decl;
        struct var_decl_stmt_list_t *var_decl_stmt_list;
};

struct var_decl_t {
        struct pos_t pos;
        enum {
                BASIC_VAR,
                STRUCT_VAR,
                TYPEDEF_VAR
        } type;
        union {
                enum basic_type basic_var;
                struct struct_type_t *struct_var; 
                char *typedef_id;
        } val;
        char *id;
        struct array_specifier_t *array_specifier;
};

struct struct_type_t {
        struct pos_t pos;
        struct var_decl_stmt_list_t *var_decl_stmt_list;
};

struct array_specifier_t {
        struct pos_t pos;
        struct constant_t *constant;
        struct array_specifier_t *array_specifier;
};

struct function_def_list_t {
        struct pos_t pos;
        struct function_def_t *function_def;
        struct function_def_list_t *function_def_list;
};

struct function_def_t {
        struct pos_t pos;
        enum {
                BASIC_TYPE_FUNCTION,
                VOID_FUNCTION
        } type;
        enum basic_type type_specifier;
        char *id;
        struct function_param_list_t *function_param_list;
        struct function_body_t *function_body;
};

struct function_param_list_t {
        struct pos_t pos;
        struct var_decl_t *var_decl;
        struct function_param_list_t *function_param_list;
};

struct function_body_t {
        struct pos_t pos;
        struct var_decl_stmt_list_t *var_decl_stmt_list;
        struct stmt_list_t *stmt_list;
        struct return_stmt_t *return_stmt;
};

struct function_call_t {
        struct pos_t pos;
        char *id;
        struct function_arg_list_t *function_arg_list;
};

struct function_arg_list_t {
        struct pos_t pos;
        struct expr_t *expr;
        struct function_arg_list_t *function_arg_list;
};

struct stmt_list_t {
        struct pos_t pos;
        struct stmt_t *stmt;
        struct stmt_list_t *stmt_list;
};

struct stmt_t {
        struct pos_t pos;
        enum stmt_type {
                EXPR_STMT,
                COMPOUND_STMT,
                SELECT_STMT,
                ITER_STMT,
                RETURN_STMT
        } type;
        union {
                struct expr_stmt_t *expr_stmt;
                struct compound_stmt_t *compound_stmt;
                struct select_stmt_t *select_stmt;
                struct iter_stmt_t *iter_stmt;
                struct return_stmt_t *return_stmt;
        } val;
};

struct expr_stmt_t {
        struct pos_t pos;
        struct expr_t *expr;
};

struct compound_stmt_t {
       struct pos_t pos;
       struct stmt_list_t *stmt_list;
};

struct select_stmt_t {
        struct pos_t pos;
        enum {
                IF_THEN,
                IF_THEN_ELSE
        } type;
        struct expr_t *cond;
        struct stmt_t *stmt_if_true;
        struct stmt_t *stmt_if_false;
};

struct iter_stmt_t {
        struct pos_t pos;
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
        struct expr_t *init;
        struct expr_t *cond;
        struct expr_t *after;
        struct stmt_t *body;
};

struct return_stmt_t {
        struct pos_t pos;
        struct expr_t *expr;
};

struct expr_t {
        struct pos_t pos;
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
                        struct var_t *assignee;
                        struct expr_t *assignment;
                } assign;
                struct {
                        struct expr_t *primary;
                        struct expr_t *secondary;
                } relation;
                struct {
                        struct expr_t *primary;
                        struct expr_t *secondary;
                } binary_op;
                union {
                        enum basic_type type;
                        struct expr_t *expr;
                } unary_op;
                union {
                        struct var_t *var;
                        struct constant_t *constant;
                        struct expr_t *expr;
                        struct function_call_t *function_call;
                } postfix_op;
        } val;
};

struct var_t {
        struct pos_t pos;
        enum {
                IDENTIFIER,
                FIELD,
                SUBSCRIPT
        } type;
        union {
                char *id;
                struct {
                        struct var_t *var;
                        char *id;
                } field;
                struct {
                        struct var_t *var;
                        struct expr_t *expr;
                } subscript;
        } val;
};

struct constant_t {
        struct pos_t pos;
        enum basic_type type;
        union value val;
};

struct program_t *create_program(int, int, struct type_decl_list_t *, struct var_decl_stmt_list_t *, struct function_def_list_t *);
struct type_decl_list_t *create_type_decl_list(int, int, struct type_decl_t *, struct type_decl_list_t *);
struct type_decl_t *create_type_decl(int, int, struct var_decl_t *var_decl);
struct var_decl_stmt_list_t *create_var_decl_stmt_list(int, int, struct var_decl_t *, struct var_decl_stmt_list_t *);
struct var_decl_t *create_var_decl_basic(int, int, enum basic_type, char *, struct array_specifier_t *); 
struct var_decl_t *create_var_decl_struct(int, int, struct struct_type_t *, char *, struct array_specifier_t *); 
struct var_decl_t *create_var_decl_typedef(int, int, char *, char *, struct array_specifier_t *); 
struct struct_type_t *create_struct_type(int, int, struct var_decl_stmt_list_t *);
struct array_specifier_t *create_array_specifier(int, int, struct constant_t *constant, struct array_specifier_t *);
struct function_def_list_t *create_function_def_list(int, int, struct function_def_t *, struct function_def_list_t *);
struct function_def_t *create_basic_function_def(int, int, enum basic_type, char *, struct function_param_list_t *, struct function_body_t *);
struct function_def_t *create_void_function_def(int, int, char *, struct function_param_list_t *, struct function_body_t *);
struct function_param_list_t *create_function_param_list(int, int, struct var_decl_t *, struct function_param_list_t *);
struct function_body_t *create_function_body(int, int, struct var_decl_stmt_list_t *, struct stmt_list_t *, struct return_stmt_t *);
struct function_call_t *create_function_call(int, int, char *id, struct function_arg_list_t *);
struct function_arg_list_t *create_function_arg_list(int, int, struct expr_t *, struct function_arg_list_t *);
struct stmt_list_t *create_stmt_list(int, int, struct stmt_t *, struct stmt_list_t *);
struct stmt_t *create_stmt(int, int, enum stmt_type, void *);
struct expr_stmt_t *create_expr_stmt(int, int, struct expr_t *);
struct compound_stmt_t *create_compound_stmt(int, int, struct stmt_list_t *);
struct select_stmt_t *create_select_stmt(int, int, struct expr_t *, struct stmt_t *, struct stmt_t *);
struct iter_stmt_t *create_iter_stmt(int, int, struct expr_t *, struct expr_t *, struct expr_t *, struct stmt_t *);
struct return_stmt_t *create_return_stmt(int, int, struct expr_t *);
struct expr_t *create_assign_expr(int, int, struct var_t *, struct expr_t *);
struct expr_t *create_logical_or_expr(int, int, struct expr_t *, struct expr_t *);
struct expr_t *create_logical_and_expr(int, int, struct expr_t *, struct expr_t *);
struct expr_t *create_equality_expr(int, int, enum equality_expr_subtype, struct expr_t *, struct expr_t *);
struct expr_t *create_relational_expr(int, int, enum relational_expr_subtype, struct expr_t *, struct expr_t *);
struct expr_t *create_additive_expr(int, int, enum additive_expr_subtype, struct expr_t *, struct expr_t *);
struct expr_t *create_multiplicative_expr(int, int, enum multiplicative_expr_subtype, struct expr_t *, struct expr_t *);
struct expr_t *create_unary_expr_sizeof_basic(int, int, enum basic_type);
struct expr_t *create_unary_expr(int, int, enum unary_expr_subtype, struct expr_t *);
struct expr_t *create_postfix_expr(int, int, enum postfix_expr_subtype, struct expr_t *);
struct expr_t *create_postfix_expr_var(int, int, struct var_t *);
struct expr_t *create_postfix_expr_constant(int, int, struct constant_t *);
struct expr_t *create_postfix_expr_function_call(int, int, struct function_call_t *);
struct var_t *create_var_identifier(int, int, char *);
struct var_t *create_var_field(int, int, struct var_t *, char *);
struct var_t *create_var_subscript(int, int, struct var_t *, struct expr_t *);
struct constant_t *create_constant_char(int, int, cflat_char);
struct constant_t *create_constant_float(int, int, cflat_float);
struct constant_t *create_constant_int(int, int, cflat_int);
enum basic_type recognize_basic_type(char *);

#endif  /* CFLAT_AST_H */
