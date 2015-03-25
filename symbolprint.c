#include <stdio.h>
#include "print.h"
#include "symbol.h"
#include "symbolprint.h"

void print_symbol_table
(FILE *out, struct symbol_table *this, int depth)
{
        int i;
        if (!this) return;
        for (i = 0; i < this->n_items; i++) {
                print_symbol_table_item(out, this->items[i], depth);
        }
}

void print_symbol_table_item(FILE *out, struct symbol_table_item *this, int depth)
{
        if (!this) return;
        switch (this->symbol->type) {
        case SYMBOL_BASIC:
                print_at_depth(out, depth, "BASIC: '%s'", this->id);
                break;
        case SYMBOL_NAMED:
                print_at_depth(out, depth, "NAMED: '%s'", this->id);
                break;
        case SYMBOL_ARRAY:
                print_at_depth(out, depth, "ARRAY: '%s'", this->id);
                break;
        case SYMBOL_STRUCT:
                print_at_depth(out, depth, "STRUCT: '%s'", this->id);
                print_symbol_table(out, this->symbol->scoped_table, depth+1);
                break;
        case SYMBOL_FUNCTION:
                print_at_depth(out, depth, "FUNCTION: '%s'", this->id);
                print_symbol_table(out, this->symbol->scoped_table, depth+1);
                break;
        }
}
