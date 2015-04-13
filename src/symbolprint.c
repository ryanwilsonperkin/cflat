/* symbolprint.c
 * Print representation of symbol table.
 * by Ryan Wilson-Perkin
 * for CIS4650W15 Compilers: Cflat Assignment
 */
#include <assert.h>
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

void print_symbol_table_item
(FILE *out, struct symbol_table_item *this, int depth)
{
        print_symbol(out, this->id, this->symbol, depth);
}

void print_symbol
(FILE *out, char *id, struct symbol *symbol, int depth)
{
        switch (symbol->type) {
        case SYMBOL_BASIC:
                switch (symbol->val.basic_type) {
                case CHAR_TYPE:
                        print_at_depth(out, depth, "CHAR: '%s' (%u)", id, symbol->size);
                        break;
                case INT_TYPE:
                        print_at_depth(out, depth, "INT: '%s' (%u)", id, symbol->size);
                        break;
                case FLOAT_TYPE:
                        print_at_depth(out, depth, "FLOAT: '%s' (%u)", id, symbol->size);
                        break;
                default:
                        assert(0);  /* Invalid enum value. */
                }
                break;
        case SYMBOL_NAMED:
                print_at_depth(out, depth, "NAMED: '%s' (%u)", id, symbol->size);
                print_symbol(out, "", symbol->val.symbol, depth+1);
                break;
        case SYMBOL_ARRAY:
                print_at_depth(out, depth, "ARRAY: %d (%u)", symbol->val.array.size, symbol->size);
                print_symbol(out, id, symbol->val.array.symbol, depth+1);
                break;
        case SYMBOL_STRUCT:
                print_at_depth(out, depth, "STRUCT: '%s' (%u)", id, symbol->size);
                print_symbol_table(out, symbol->scoped_table, depth+1);
                break;
        case SYMBOL_FUNCTION:
                print_at_depth(out, depth, "FUNCTION: '%s' (%u)", id, symbol->size);
                print_symbol_table(out, symbol->scoped_table, depth+1);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}
