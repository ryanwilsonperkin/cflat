#ifndef CFLAT_ASM_H
#define CFLAT_ASM_H

#include "intermediate.h"
#include "symbol.h"

#define NUM_REGS 8

typedef char* line;

struct assembly {
        line *lines;
        unsigned int n_lines;
        char regs[NUM_REGS];
};

struct assembly *create_assembly();

void add_line(struct assembly *, line);
int get_reg(struct assembly *);
void unget_reg(struct assembly *, int);

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

line create_load_word(int, struct quad_address *);
line create_store_word(int, struct quad_address *);

#endif  /* CFLAT_ASM_H */
