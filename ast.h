#ifndef CFLAT_AST_H
#define CFLAT_AST_H

struct relational_expr_t {
        enum {
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
}

struct additive_expr_t {
        enum {
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
}

struct multiplicative_expr_t {
        enum {
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
}

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
