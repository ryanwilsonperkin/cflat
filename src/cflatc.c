#include <argp.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"
#include "ast.h"
#include "astprint.h"
#include "intermediate.h"
#include "intermediateprint.h"
#include "parser.h"
#include "symbol.h"
#include "symbolprint.h"
#include "typecheck.h"

extern struct program *parse_file(FILE *in);

const char *argp_program_version = "cflatc 0.2";
const char *argp_program_bug_address= "rwilsonp@mail.uoguelph.ca";
static char doc[] =
        "cflatc - compile Cflat programs\n"
        "cflatc reads from the named input file (or standard input if no file is named.\n"
        "http://www.uoguelph.ca/~dmccaugh/teaching/CIS4650/Project/";

static char args_doc[] = "[FILE]";

static struct argp_option options[] = {
        {"abstract",     'a', 0, 0, "Perform syntax analysis and output abstract syntax (.abs)"},
        {"symbol",       's', 0, 0, "Perform type checking and output symbol table (.sym)"},
        {"intermediate", 'i', 0, 0, "Perform sematic analysis and output intermediate representation (.inr)"},
        {"compile",      'c', 0, 0, "Compile and output MIPS R2000 assembler (.asm)"},
        {"output",       'o', "FILE", 0, "Output results to FILE"},
        {0}
};

struct arguments
{
        char *args[1];
        int abstract_flag, symbol_flag, intermediate_flag, compile_flag;
        char *out_fname;
};

struct out_fnames {
        char *abstract, *symbol, *intermediate, *compile;
};

static error_t parse_opt
(int key, char *arg, struct argp_state *state)
{
        struct arguments *arguments = state->input;
        switch (key)
        {
        case 'a':
                arguments->abstract_flag = 1;
                break;
        case 's':
                arguments->symbol_flag = 1;
                break;
        case 'i':
                arguments->intermediate_flag = 1;
                break;
        case 'c':
                arguments->compile_flag = 1;
                break;
        case 'o':
                arguments->out_fname = arg;
                break;
        case ARGP_KEY_ARG:
                if (state->arg_num >= 1) argp_usage (state);
                arguments->args[state->arg_num] = arg;
                break;
        case ARGP_KEY_END:
                if (state->arg_num < 1) argp_usage (state);
                break;
        default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
}

static void set_out_fnames
(char *prefix, struct out_fnames *fnames)
{
        int prefix_len = strlen(prefix);
        fnames->abstract = malloc(prefix_len + 5);
        fnames->abstract = strcpy(fnames->abstract, prefix);
        fnames->abstract = strcat(fnames->abstract, ".abs");
        fnames->symbol = malloc(prefix_len + 5);
        fnames->symbol = strcpy(fnames->symbol, prefix);
        fnames->symbol = strcat(fnames->symbol, ".sym");
        fnames->intermediate = malloc(prefix_len + 5);
        fnames->intermediate = strcpy(fnames->intermediate, prefix);
        fnames->intermediate = strcat(fnames->intermediate, ".inr");
        fnames->compile = malloc(prefix_len + 5);
        fnames->compile = strcpy(fnames->compile, prefix);
        fnames->compile = strcat(fnames->compile, ".asm");
}

int main
(int argc, char *argv[])
{
        FILE *in, *out;
        char *in_fname;
        struct out_fnames out_fnames;
        struct arguments arguments = {
                .abstract_flag = 0,
                .symbol_flag = 0,
                .intermediate_flag = 0,
                .compile_flag = 0,
                .out_fname = "-"
        };
        static struct argp argp = {
                options, parse_opt, args_doc, doc
        };
        struct program *program;
        struct symbol_table *symbol_table;
        struct instructions *instructions;
        struct assembly *assembly;

        /* Parse command line arguments */
        argp_parse (&argp, argc, argv, 0, 0, &arguments);
        set_out_fnames(arguments.out_fname, &out_fnames);

        /* Get input file name from arguments */
        in_fname = arguments.args[0];
        if (!(in = fopen(in_fname, "r"))) {
                fprintf(stderr, "cflatc: '%s': %s\n", in_fname, strerror(errno));
                exit(EXIT_FAILURE);
        }

        /* Invoke parser on input file */
        program = parse_file(in);
        symbol_table = parse_symbols(program);
        type_check_program(symbol_table, program);
        instructions = parse_instructions(symbol_table, program);
        assembly = parse_assembly(symbol_table, instructions);

        /* Output abstract syntax */
        if (arguments.abstract_flag) {
                if (strcmp(arguments.out_fname, "-") == 0) {
                        out = stdout;
                } else if (!(out = fopen(out_fnames.abstract, "w"))) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.abstract, strerror(errno));
                        exit(EXIT_FAILURE);
                }
                print_ast(out, program);
                if (out != stdout && fclose(out)) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.abstract, strerror(errno));
                        exit(EXIT_FAILURE);
                }
        }

        /* Output symbol table */
        if (arguments.symbol_flag) {
                if (strcmp(arguments.out_fname, "-") == 0) {
                        out = stdout;
                } else if (!(out = fopen(out_fnames.symbol, "w"))) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.symbol, strerror(errno));
                        exit(EXIT_FAILURE);
                }
                print_symbol_table(out, symbol_table, 0);
                if (out != stdout && fclose(out)) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.symbol, strerror(errno));
                        exit(EXIT_FAILURE);
                }
        }

        /* Output intermediate representation */
        if (arguments.intermediate_flag) {
                if (strcmp(arguments.out_fname, "-") == 0) {
                        out = stdout;
                } else if (!(out = fopen(out_fnames.symbol, "w"))) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.symbol, strerror(errno));
                        exit(EXIT_FAILURE);
                }
                print_intermediate(out, instructions);
                if (out != stdout && fclose(out)) {
                        fprintf(stderr, "cflatc: '%s': %s\n", out_fnames.symbol, strerror(errno));
                        exit(EXIT_FAILURE);
                }
        }

        /* Output assembler */

        free_program(program);
        return EXIT_SUCCESS;
}
