#ifndef CFLAT_AST_H
#define CFLAT_AST_H

typedef struct constant_t {
        enum basic_type type;
        union value val;
} constant;

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
