#ifndef CFLAT_SYMBOL_H
#define CFLAT_SYMBOL_H

#include "ast.h"

enum symbol_type {
        SYMBOL_BASIC,
        SYMBOL_NAMED,
        SYMBOL_ARRAY,
        SYMBOL_STRUCT,
        SYMBOL_FUNCTION
};

struct symbol {
        enum symbol_type type;
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
        int n_temps;
        struct symbol_table_item **items;
        struct symbol_table_item **temps;
};

void type_error(struct pos pos, const char *fmt, ...);

struct symbol_table *create_symbol_table();
struct symbol_table_item *create_symbol_table_item(char *, struct symbol *);
struct symbol *create_symbol_basic(enum basic_type);
struct symbol *create_symbol_named(struct symbol *);
struct symbol *create_symbol_array(struct symbol *, int);
struct symbol *create_symbol_struct(struct struct_type *);
struct symbol *create_symbol_function(struct function_def *);

struct symbol *get_symbol(struct symbol_table *, char *);
void add_symbol(struct symbol_table *, char *, struct symbol *);
void add_temp_symbol(struct symbol_table *, struct symbol *);

struct symbol_table *parse_symbols(struct program *);
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

struct symbol *translate_var_decl(struct symbol_table *, struct var_decl *);
struct symbol *translate_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_assign_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_logical_or_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_logical_and_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_equality_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_relational_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_additive_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_multiplicative_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_unary_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_postfix_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_function_call(struct symbol_table *, struct symbol_table *, struct function_call *);
struct symbol *translate_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_identifier_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_field_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_subscript_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_constant(struct symbol_table *, struct constant *);

int symbol_equivalent(struct symbol *, struct symbol *);

#endif  /* CFLAT_SYMBOL_H */
