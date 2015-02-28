#ifndef CFLAT_LEXEME_H
#define CFLAT_LEXEME_H
enum lexeme_type {
        /* Reserved keywords */
        CHAR,
        ELSE,
        FLOAT,
        FOR,
        IF,
        INT,
        RETURN,
        STRUCT,
        TYPEDEF,
        VOID,
        WHILE,
        /* Operators */
        PLUS,
        MINUS,
        ASTERISK,
        SLASH,
        PERCENT,
        EQ,  /* Equal */
        NE,  /* Not equal */
        LT,  /* Less than */
        GT,  /* Greater than */
        LE,  /* Less than or equal */
        GE,  /* Greater than or equal */
        AND,
        OR,
        NOT,
        ASSIGN,
        SIZEOF,
        INCREMENT,
        DECREMENT,
        /* Control structures */
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        LPAREN,
        RPAREN,
        SEMICOLON,
        PERIOD,
        /* Identifier */
        ID,
        /* Anything not matching */
        OTHER
};
#endif  /* CFLAT_LEXEME_H */
