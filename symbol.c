#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol.h"

struct symbol_table *create_symbol_table
()
{
        struct symbol_table *this = malloc(sizeof(struct symbol_table));
        this->n_items = 0;
        this->items = NULL;
        return this;
}

struct symbol_table_item *create_symbol_table_item
(char *id, struct symbol *symbol)
{
        struct symbol_table_item *this = malloc(sizeof(struct symbol_table_item));
        this->id = id;
        this->symbol = symbol;
        return this;
}

struct symbol *create_symbol_basic
(enum basic_type basic_type)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_BASIC;
        this->val.basic_type = basic_type;
        return this;
}

struct symbol *create_symbol_named
(struct symbol *symbol)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_NAMED;
        this->val.symbol = symbol;
        return this;
}

struct symbol *create_symbol_struct
(struct struct_type *struct_type)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_STRUCT;
        this->val.struct_type = struct_type;
        this->scoped_table = create_symbol_table();
        return this;
}

struct symbol *create_symbol_function
(struct function_def *function_def)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_FUNCTION;
        this->val.function_def = function_def;
        this->scoped_table = create_symbol_table();
        return this;
}

void add_symbol
(struct symbol_table *symbol_table, char *id, struct symbol *symbol)
{
        if (get_symbol(symbol_table, id)) {
                fprintf(stderr, "error: redefinition of symbol '%s'\n", id);
                exit(EXIT_FAILURE);
        }
        symbol_table->n_items++;
        symbol_table->items = realloc(symbol_table->items, sizeof(struct symbol_table_item *) * symbol_table->n_items);
        symbol_table->items[symbol_table->n_items - 1] = create_symbol_table_item(id, symbol);
}

struct symbol *get_symbol
(struct symbol_table *symbol_table, char *id)
{
        int i;
        struct symbol_table_item *item;
        for (i = 0; i < symbol_table->n_items; i++) {
                item = symbol_table->items[i];
                if (strcmp(item->id, id) == 0) {
                        return item->symbol;
                }
        }
        return NULL;
}
