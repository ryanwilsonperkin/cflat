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

struct program_t {
        struct type_decl_list_t *type_decl_list;
        struct var_decl_list_t *var_decl_list;
        struct function_def_list_t *function_def_list;
};

struct type_decl_list_t {
        struct type_decl_t *type_decl;
        struct type_decl_list_t *type_decl_list;
};

struct type_decl_t {
        enum basic_type type;
        char *id;
};

struct var_decl_list_t {
        struct var_decl_t *var_decl;
        struct var_decl_list_t *var_decl_list;
};

struct var_decl_t {
        enum {
                BASIC_VAR,
                STRUCT_VAR
        } type;
        union {
                enum basic_type basic_var;
                struct struct_type_t *struct_var; 
        } val;
        char *id;
        struct array_specifier_t *array_specifier;
};

struct struct_type_t {
        struct var_decl_list_t *var_decl_list;
};

struct array_specifier_t {
        cflat_int size;
};

struct function_def_list_t {
        struct function_def_t *function_def;
        struct function_def_list_t *function_def_list;
};

struct function_def_t {
        enum {
                BASIC_TYPE_FUNCTION,
                VOID_FUNCTION
        } type;
        enum basic_type type_specifier;
        char *id;
        struct function_params_t *function_params;
        struct function_body_t *function_body;
};

struct function_params_t {
        struct var_decl_list_t *var_decl_list;
};

struct function_body_t {
        struct var_decl_list_t *var_decl_list;
        struct stmt_list_t *stmt_list;
        struct return_stmt_t *return_stmt;
};

struct function_call_t {
        struct arg_list_t *arg_list;
};

struct arg_list_t {
        struct expr_t *expr;
        struct arg_list_t *arg_list;
};

struct stmt_list_t {
        struct stmt_t *stmt;
        struct stmt_list_t *stmt_list;
};

struct stmt_t {
        enum stmt_type {
                EXPR_STMT,
                COMPOUND_STMT,
                SELECT_STMT,
                ITER_STMT,
                RETURN_STMT,
                EMPTY_STMT
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
        struct expr_t *expr;
};

struct compound_stmt_t {
       struct stmt_list_t *stmt_list;
};

struct select_stmt_t {
        enum {
                IF_THEN,
                IF_THEN_ELSE
        } type;
        struct {
                struct expr_t *cond;
                struct stmt_t *stmt_if_true;
                struct stmt_t *stmt_if_false;
        } val;
};

struct iter_stmt_t {
        enum {
                WHILE_EXPR,
                FOR_INIT_COND_AFTER,
                FOR_INIT_AFTER,
                FOR_INIT_COND,
                FOR_AFTER,
                FOR_INIT,
                FOR_COND,
                FOR_EMPTY
        } type;
        struct {
                struct expr_t *init;
                struct expr_t *cond;
                struct expr_t *after;
        } val;
};

struct return_stmt_t {
        struct expr_t *expr;
};

struct expr_t {
        struct assign_expr_t *assign_expr;
};

struct assign_expr_t {
        enum {
                ASSIGN_EXPR,
                LOGICAL_OR_EXPR
        } type;
        union {
                struct {
                        struct var_t *assignee;
                        struct assign_expr_t *assignment;
                } assign;
                struct logical_or_expr *logical_or_expr;
        } val;
};

struct logical_or_expr_t {
        enum {
                OR_EXPR,
                LOGICAL_AND_EXPR 
        } type;
        union {
                struct {
                        struct logical_or_expr_t *primary;
                        struct logical_and_expr_t *secondary;
                } relation;
                struct logical_and_expr_t *logical_and_expr;
        } val;
};

struct logical_and_expr_t {
        enum {
                AND_EXPR,
                EQUALITY_EXPR
        } type;
        union {
                struct {
                        struct logical_and_expr_t *primary;
                        struct equality_expr_t *secondary;
                } relation;
                struct equality_expr_t *equality_expr;
        } val;
};

struct equality_expr_t {
        enum equality_expr_type {
                EQUAL,
                NOT_EQUAL,
                RELATIONAL_EXPR
        } type;
        union {
                struct {
                        struct equality_expr_t *primary;
                        struct relational_expr_t *secondary;
                } equality;
                struct relational_expr_t *relational_expr;
        } val;
};

struct relational_expr_t {
        enum relational_expr_type {
                LESS_THAN,
                LESS_THAN_OR_EQUAL,
                GREATER_THAN,
                GREATER_THAN_OR_EQUAL,
                ADDITIVE_EXPR
        } type;
        union {
                struct {
                        struct relational_expr_t *primary;
                        struct additive_expr_t *secondary;
                } relation;
                struct additive_expr_t *additive_expr;
        } val;
};

struct additive_expr_t {
        enum additive_expr_type {
                ADD,
                SUBTRACT,
                MULTIPLICATIVE_EXPR
        } type;
        union {
                struct {
                        struct additive_expr_t *augend;
                        struct multiplicate_expr_t *addend;
                } add;
                struct {
                        struct additive_expr_t *minuend;
                        struct multiplicative_expr_t *subtrahend;
                } subtract;
                struct multiplicative_expr_t *multiplicative_expr;
        } val;
};

struct multiplicative_expr_t {
        enum multiplicative_expr_type {
                MULTIPLY,
                DIVIDE,
                MODULO,
                UNARY_EXPR
        } type;
        union {
                struct {
                        struct multiplicative_expr_t *multiplicand;
                        struct unary_expr_t *multiplier;
                } multiply;
                struct {
                        struct multiplicative_expr_t *dividend;
                        struct unary_expr_t *divisor;
                } divide;
                struct {
                        struct multiplicative_expr_t *dividend;
                        struct unary_expr_t *divisor;
                } modulo;
                struct unary_expr_t *unary_expr;
        } val;
};

struct unary_expr_t {
        enum unary_expr_type {
                SIZEOF_UNARY,
                SIZEOF_BASIC,
                NOT_UNARY,
                POSITIVE,
                NEGATIVE,
                PRE_INCREMENT,
                PRE_DECREMENT,
                POSTFIX_EXPR
        } type;
        union {
                struct unary_expr_t *sizeof_unary;
                enum basic_type sizeof_basic;
                struct unary_expr_t *not_unary;
                struct unary_expr_t *positive;
                struct unary_expr_t *negative;
                struct unary_expr_t *pre_increment;
                struct unary_expr_t *post_increment;
                struct postfix_expr_t *postfix_expr;
        } val;
};

struct postfix_expr_t {
        enum postfix_expr_type {
                VAR,
                CONSTANT,
                POST_INCREMENT,
                POST_DECREMENT,
                ENCLOSED,
                FUNCTION_CALL
        } type;
        union {
                struct var_t *var;
                struct constant_t *constant;
                struct postfix_expr_t *post_increment;
                struct postfix_expr_t *post_decrement;
                struct expr_t *enclosed;
                struct function_call_t *function_call;
        } val;
};

struct var_t {
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
                        struct expr_t *exp;
                } subscript;
        } val;
};

struct constant_t {
        enum basic_type type;
        union value val;
};

struct program_t *create_program(struct type_decl_list_t *, struct var_decl_list_t *, struct function_def_list_t *);
struct type_decl_list_t *create_type_decl_list(struct type_decl_t *, struct type_decl_list_t *);
struct type_decl_t *create_type_decl(enum basic_type, char *);
struct var_decl_list_t *create_var_decl_list(struct var_decl_t *, struct var_decl_list_t *);
struct var_decl_t *create_var_decl_basic(enum basic_type, char *, struct array_specifier_t *); 
struct var_decl_t *create_var_decl_struct(struct struct_type_t *, char *, struct array_specifier_t *); 
struct struct_type_t *create_struct_type(struct var_decl_list_t *);
struct array_specifier_t *create_array_specifier(cflat_int);
struct function_def_list_t *create_function_def_list(struct function_def_t *, struct function_def_list_t *);
struct function_def_t *create_basic_function_def(enum basic_type, char *, struct function_params_t *, struct function_body_t *);
struct function_def_t *create_void_function_def(char *, struct function_params_t *, struct function_body_t *);
struct function_params_t *create_function_params(struct var_decl_list_t *);
struct function_body_t *create_function_body(struct var_decl_list_t *, struct stmt_list_t *, struct return_stmt_t *);
struct function_call_t *create_function_call(struct arg_list_t *);
struct arg_list_t *create_arg_list(struct expr_t *, struct arg_list_t *);
struct stmt_list_t *create_stmt_list(struct stmt_t *, struct stmt_list_t *);
struct stmt_t *create_stmt(enum stmt_type, void *);
struct expr_stmt_t *create_expr_stmt(struct expr_t *);
struct compound_stmt_t *create_compound_stmt(struct stmt_list_t *);
struct select_stmt_t *create_select_stmt(struct expr_t *, struct stmt_t *, struct stmt_t *);
struct iter_stmt_t *create_iter_stmt(struct expr_t *, struct expr_t *, struct expr_t *);
struct return_stmt_t *create_return_stmt(struct expr_t *);
struct expr_t *create_expr_t(struct assign_expr_t *);
struct assign_expr_t *create_assign_expr(struct var_t *, struct assign_expr_t *);
struct assign_expr_t *wrap_logical_or_expr(struct logical_or_expr_t *);
struct logical_or_expr_t *create_logical_or_expr(struct logical_or_expr_t *, struct logical_and_expr_t *);
struct logical_or_expr_t *wrap_logical_and_expr(struct logical_and_expr_t *);
struct logical_and_expr_t *create_logical_and_expr(struct logical_and_expr_t *, struct equality_expr_t *);
struct logical_and_expr_t *wrap_equality_expr(struct equality_expr_t *);
struct equality_expr_t *create_equality_expr(enum equality_expr_type, struct equality_expr_t *, struct relational_expr_t *);
struct equality_expr_t *wrap_relational_expr(struct relational_expr_t *);
struct relational_expr_t *create_relational_expr(enum relational_expr_type, struct relational_expr_t *, struct additive_expr_t *);
struct relational_expr_t *wrap_additive_expr(struct additive_expr_t *);
struct additive_expr_t *create_additive_expr(enum additive_expr_type, struct additive_expr_t *, struct multiplicative_expr_t *);
struct additive_expr_t *wrap_multiplicative_expr(struct multiplicative_expr_t *);
struct multiplicative_expr_t *create_multiplicative_expr(enum multiplicative_expr_type, struct multiplicative_expr_t *, struct unary_expr_t *);
struct multiplicative_expr_t *wrap_unary_expr(struct unary_expr_t *);
struct unary_expr_t *create_unary_expr_sizeof_basic(enum basic_type);
struct unary_expr_t *create_unary_expr(enum unary_expr_type, struct unary_expr_t *);
struct unary_expr_t *wrap_postfix_expr(struct postfix_expr_t *);
struct postfix_expr_t *create_postfix_expr(enum postfix_expr_type, struct postfix_expr_t *);
struct postfix_expr_t *wrap_var(struct var_t *);
struct postfix_expr_t *wrap_constant(struct constant_t *);
struct postfix_expr_t *wrap_enclosed(struct expr_t *);
struct postfix_expr_t *wrap_function_call(struct function_call_t *);
struct var_t *create_var_identifier(char *);
struct var_t *create_var_field(struct var_t *, char *);
struct var_t *create_var_subscript(struct var_t *, struct expr_t *);
struct constant_t *create_constant_char(cflat_char);
struct constant_t *create_constant_float(cflat_float);
struct constant_t *create_constant_int(cflat_int);
enum basic_type recognize_basic_type(char *);

#endif  /* CFLAT_AST_H */
