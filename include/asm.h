#ifndef CFLAT_ASM_H
#define CFLAT_ASM_H

#include "intermediate.h"
#include "symbol.h"

#define NUM_IREGS 8
#define NUM_FREGS 6

enum line_address_type {
        LINE_ADDRESS_NAME,
        LINE_ADDRESS_CONSTANT,
        LINE_ADDRESS_OFFSET,
        LINE_ADDRESS_REG
};

struct line_address {
        enum line_address_type type;
        enum basic_type basic_type;
        union {
                char *name;
                union value constant;
                struct {
                        int offset;
                        int reg;
                } offset;
                int reg;
        } val;
};

enum line_type {
        LINE_LOAD,
        LINE_STORE
};

struct line {
        enum line_type type;
        union {
                struct {
                        struct line_address *src, *dest;
                } load;
                struct {
                        struct line_address *src, *dest;
                } store;
        } val;
};

struct assembly {
        struct line **lines;
        unsigned int n_lines;
        char iregs[NUM_IREGS];
        char fregs[NUM_FREGS];
};

struct assembly *create_assembly();
struct line_address *create_line_address_name(enum basic_type, char *);
struct line_address *create_line_address_constant(enum basic_type, union value);
struct line_address *create_line_address_offset(enum basic_type, int, int);
struct line_address *create_line_address_register(enum basic_type, int);
struct line *create_line_load(struct line_address *, struct line_address *);
struct line *create_line_store(struct line_address *, struct line_address *);

void add_line(struct assembly *, struct line *);
int get_ireg(struct assembly *);
int get_freg(struct assembly *);
void unget_ireg(struct assembly *, int);
void unget_freg(struct assembly *, int);

struct assembly *parse_assembly(struct symbol_table *, struct instructions *);
void parse_assembly_instruction(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_binary_assign(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_unary_assign(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_copy(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_copy_addr(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_copy_from_addr(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_copy_to_addr(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_label(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_unconditional_jump(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_conditional_jump(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_relational_jump(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_procedure_param(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_procedure_call(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);
void parse_assembly_procedure_return(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);

#endif  /* CFLAT_ASM_H */
