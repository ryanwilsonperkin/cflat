#ifndef CFLAT_AST_H
#define CFLAT_AST_H

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
