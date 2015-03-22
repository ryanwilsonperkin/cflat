#include <stdio.h>
#include "ast.h"
#include "astprint.h"
#include "parser.h"

extern struct program_t *program;
extern FILE *yyin;
extern int n_errors;

extern int yyparse();

int main()
{
        if (yyin == NULL) yyin = stdin;
        yyparse();
        if (n_errors) {
            fprintf(stderr, "%d syntax errors\n", n_errors);
            return 1;
        } else {
            print_ast(stdout, program);
        }
        return 0;
}
