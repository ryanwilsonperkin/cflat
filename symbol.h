#ifndef CFLAT_SYMBOL_H
#define CFLAT_SYMBOL_H

#include "ast.h"

struct symbol {
        enum symbol_type {
                SYMBOL_BASIC,
                SYMBOL_NAMED,
                SYMBOL_ARRAY,
                SYMBOL_STRUCT,
                SYMBOL_FUNCTION
        } type;
        union {
                enum basic_type basic_type;
                struct symbol *symbol;
                struct {
                        struct symbol *symbol;
                        int size;
                } array;
                struct struct_type *struct_type;
                struct function_def *function_def;
        } val;
        struct symbol_table *scoped_table;
};

struct symbol_table_item {
        char *id;
        struct symbol *symbol;
};

struct symbol_table {
        int n_items;
        struct symbol_table_item **items;
};

struct symbol_table *create_symbol_table();
struct symbol_table_item *create_symbol_table_item(char *, struct symbol *);
struct symbol *create_symbol_basic(enum basic_type);
struct symbol *create_symbol_named(struct symbol *);
struct symbol *create_symbol_array(struct symbol *, int);
struct symbol *create_symbol_struct(struct struct_type *);
struct symbol *create_symbol_function(struct function_def *);
void add_symbol(struct symbol_table *, char *, struct symbol *);
struct symbol *get_symbol(struct symbol_table *, char *);

struct symbol *var_decl_to_symbol(struct symbol_table *, struct var_decl *);
void parse_program(struct symbol_table *, struct program *);
void parse_type_decl_list(struct symbol_table *, struct type_decl_list *);
void parse_type_decl(struct symbol_table *, struct type_decl *);
void parse_var_decl_stmt_list(struct symbol_table *, struct symbol_table *, struct var_decl_stmt_list *);
void parse_var_decl(struct symbol_table *, struct symbol_table *, struct var_decl *);
void parse_struct_type(struct symbol_table *, struct symbol_table *, struct struct_type *);
void parse_function_def_list(struct symbol_table *, struct function_def_list *);
void parse_function_def(struct symbol_table *, struct function_def *);
void parse_function_param_list(struct symbol_table *, struct symbol_table *, struct function_param_list *);
void parse_function_body(struct symbol_table *, struct symbol_table *, struct function_body *);

#endif  /* CFLAT_SYMBOL_H */
