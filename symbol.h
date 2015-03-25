#ifndef CFLAT_SYMBOL_H
#define CFLAT_SYMBOL_H

struct symbol {
        enum symbol_type {
                SYMBOL_BASIC,
                SYMBOL_NAMED,
                SYMBOL_STRUCT,
                SYMBOL_FUNCTION
        } type;
        union {
                enum basic_type basic_type;
                struct symbol *symbol;
                struct struct_type *struct_type;
                struct function_def *function_def;
        } val;
        int size;
        int array_size;
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
struct symbol *create_symbol_basic(enum basic_type, int, int);
struct symbol *create_symbol_named(struct symbol *);
struct symbol *create_symbol_struct(struct struct_type *, int, int);
struct symbol *create_symbol_function(struct function_def *);
void add_symbol(struct symbol_table *, char *, struct symbol *);
struct symbol *get_symbol(struct symbol_table *, char *);

#endif  /* CFLAT_SYMBOL_H */
