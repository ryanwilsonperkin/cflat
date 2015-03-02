#ifndef CFLAT_AST_H
#define CFLAT_AST_H

struct unary_expr_t {
        enum {
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
}

struct postfix_expr_t {
        enum {
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
}

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
}

struct constant_t {
        enum basic_type type;
        union value val;
}

union value {
        unsigned char cval;
        float fval;
        signed int ival;
}

enum basic_type {
        CHAR_TYPE,
        FLOAT_TYPE,
        INT_TYPE
}

#endif  /* CFLAT_AST_H */
