#include <stdio.h>
#include <string.h>
#include "lexeme.h"

extern int yylex();
extern char *yytext;

char *lexeme_name(int l)
{
        switch(l) {
        case CHAR: return "CHAR";
        case ELSE: return "ELSE";
        case FLOAT: return "FLOAT";
        case FOR: return "FOR";
        case IF: return "IF";
        case INT: return "INT";
        case RETURN: return "RETURN";
        case STRUCT: return "STRUCT";
        case TYPEDEF: return "TYPEDEF";
        case VOID: return "VOID";
        case WHILE: return "WHILE";
        case CONST_CHAR: return "CONST_CHAR";
        case CONST_FLOAT: return "CONST_FLOAT";
        case CONST_INT: return "CONST_INT";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case ASTERISK: return "ASTERISK";
        case SLASH: return "SLASH";
        case PERCENT: return "PERCENT";
        case EQ: return "EQ";
        case NE: return "NE";
        case LT: return "LT";
        case GT: return "GT";
        case LE: return "LE";
        case GE: return "GE";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case ASSIGN: return "ASSIGN";
        case SIZEOF: return "SIZEOF";
        case INCREMENT: return "INCREMENT";
        case DECREMENT: return "DECREMENT";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case LBRACKET: return "LBRACKET";
        case RBRACKET: return "RBRACKET";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case SEMICOLON: return "SEMICOLON";
        case PERIOD: return "PERIOD";
        case ID: return "ID";
        case OTHER: return "OTHER";
        default: return "UNRECOGNIZED";
        }
}

int main()
{
        char test_name[20] = {0};
        scanf("%s", test_name);
        while (!feof(stdin)) {
                if (strcmp(test_name, lexeme_name(yylex())) != 0) {
                        printf("error: %s doesn't match\n", yytext);
                }
        }
        return 0;
}
