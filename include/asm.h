#ifndef CFLAT_ASM_H
#define CFLAT_ASM_H

#include "intermediate.h"
#include "symbol.h"

struct assembly {
        char **lines;
        unsigned int n_lines;
};

struct assembly *create_assembly();

struct assembly *parse_assembly(struct symbol_table *, struct instructions *);

#endif  /* CFLAT_ASM_H */
