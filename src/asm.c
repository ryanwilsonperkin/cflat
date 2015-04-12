#include <stdlib.h>

#include "asm.h"

struct assembly *create_assembly
()
{
        struct assembly *this = malloc(sizeof(struct assembly));
        this->lines = NULL;
        this->n_lines = 0;
        return this;
}

struct assembly *parse_assembly
(struct symbol_table *global, struct instructions *instructions)
{
        struct assembly *this = create_assembly();
        return this;
}
