#include <math.h>
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

struct symbol *create_symbol_array
(struct symbol *symbol, int size)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_ARRAY;
        this->val.array.symbol = symbol;
        this->val.array.size = size;
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

void add_temp_symbol
(struct symbol_table *symbol_table, struct symbol *symbol)
{
        static int counter = 0;
        char *prefix = "temp";
        int id_length = strlen(prefix) + 1 + floor(log10(counter) + 1) + 1;
        char *id = malloc(id_length);
        sprintf(id, "%s:%d", prefix, counter++);
        add_symbol(symbol_table, id, symbol);
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

struct symbol *var_decl_to_symbol
(struct symbol_table *global, struct var_decl *var_decl)
{
        int size;
        struct symbol *this;
        switch (var_decl->type)
        {
        case BASIC_VAR:
                this = create_symbol_basic(var_decl->val.basic_var);
                break;
        case STRUCT_VAR:
                this = create_symbol_struct(var_decl->val.struct_var);
                parse_struct_type(global, this->scoped_table, var_decl->val.struct_var);
                break;
        case TYPEDEF_VAR:
                this = get_symbol(global, var_decl->val.typedef_id);
                break;
        }
        if (var_decl->array_specifier) {
                size = var_decl->array_specifier->constant->val.ival;
                this = create_symbol_array(this, size);
        }
        return this;
}

struct symbol_table *parse_symbols
(struct program *program)
{
        struct symbol_table *this = create_symbol_table();
        parse_program(this, program);
        return this;
}

void parse_program
(struct symbol_table *global, struct program *this)
{
        if (!this) return;
        parse_type_decl_list(global, this->type_decl_list);
        parse_var_decl_stmt_list(global, global, this->var_decl_stmt_list);
        parse_function_def_list(global, this->function_def_list);
}

void parse_type_decl_list
(struct symbol_table *global, struct type_decl_list *this)
{
        if (!this) return;
        parse_type_decl_list(global, this->type_decl_list);
        parse_type_decl(global, this->type_decl);
}

void parse_type_decl
(struct symbol_table *global, struct type_decl *this)
{
        struct symbol *named_symbol;
        if (!this) return;
        named_symbol = create_symbol_named(var_decl_to_symbol(global, this->var_decl));
        add_symbol(global, this->var_decl->id, named_symbol);
}

void parse_var_decl_stmt_list
(struct symbol_table *global, struct symbol_table *local, struct var_decl_stmt_list *this)
{
        if (!this) return;
        parse_var_decl_stmt_list(global, local, this->var_decl_stmt_list);
        parse_var_decl(global, local, this->var_decl);
}

void parse_var_decl
(struct symbol_table *global, struct symbol_table *local, struct var_decl *this)
{
        if (!this) return;
        add_symbol(local, this->id, var_decl_to_symbol(global, this));
}

void parse_struct_type
(struct symbol_table *global, struct symbol_table *local, struct struct_type *this)
{
        if (!this) return;
        parse_var_decl_stmt_list(global, local, this->var_decl_stmt_list);
}

void parse_function_def_list
(struct symbol_table *global, struct function_def_list *this)
{
        if (!this) return;
        parse_function_def_list(global, this->function_def_list);
        parse_function_def(global, this->function_def);
}

void parse_function_def
(struct symbol_table *global, struct function_def *this)
{
        struct symbol *symbol;
        if (!this) return;
        symbol = create_symbol_function(this);
        add_symbol(global, this->id, symbol);
        parse_function_param_list(global, symbol->scoped_table, this->function_param_list);
        parse_function_body(global, symbol->scoped_table, this->function_body);
}

void parse_function_param_list
(struct symbol_table *global, struct symbol_table *local, struct function_param_list *this)
{
        if (!this) return;
        parse_function_param_list(global, local, this->function_param_list);
        parse_var_decl(global, local, this->var_decl);
}

void parse_function_body
(struct symbol_table *global, struct symbol_table *local, struct function_body *this)
{
        if (!this) return;
        parse_var_decl_stmt_list(global, local, this->var_decl_stmt_list);
}
