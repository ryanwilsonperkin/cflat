#ifndef CFLAT_ASM_H
#define CFLAT_ASM_H

#include "intermediate.h"
#include "symbol.h"

#define NUM_IREGS 8
#define NUM_FREGS 6

enum line_address_type {
        LINE_ADDRESS_NAME,
        LINE_ADDR_CONSTANT,
        LINE_ADDR_REG
};

struct line_address {
        enum line_address_type;
        union {
                char *id;
                union value constant;
                int reg;
        } val;
        enum basic_type type;
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
