#ifndef CFLAT_ASM_H
#define CFLAT_ASM_H

#include "intermediate.h"
#include "symbol.h"

enum reg {
        REG_A0,
        REG_V0,
        REG_SP,
        REG_FP,
        REG_RA,
        REG_TEMP0,
        REG_TEMP1,
        REG_TEMP2,
        REG_TEMP3,
        REG_TEMP4,
        REG_TEMP5,
        REG_TEMP6,
        REG_TEMP7
} type;

struct line_address {
        enum line_address_type {
                LINE_ADDRESS_NAME,
                LINE_ADDRESS_CONSTANT,
                LINE_ADDRESS_OFFSET,
                LINE_ADDRESS_REG
        } type;
        enum basic_type basic_type;
        union {
                char *name;
                union value constant;
                struct {
                        int offset;
                        enum reg reg;
                } offset;
                enum reg reg;
        } val;
};

struct line {
        enum line_type {
                LINE_LOAD,
                LINE_LOAD_ADDRESS,
                LINE_STORE,
                LINE_LABEL,
                LINE_ADD,
                LINE_SUB,
                LINE_OR,
                LINE_AND,
                LINE_SEQ,
                LINE_SNE,
                LINE_SLT,
                LINE_SLE,
                LINE_SGT,
                LINE_SGE,
                LINE_MUL,
                LINE_DIV,
                LINE_REM,
                LINE_NOT,
                LINE_JUMP,
                LINE_BEQZ,
                LINE_BNEZ,
                LINE_BEQ,
                LINE_BNE,
                LINE_BLT,
                LINE_BLE,
                LINE_BGT,
                LINE_BGE,
                LINE_SYSCALL
        } type;
        union {
                char *label;
                struct {
                        struct line_address *arg, *result;
                } unary_op;
                struct {
                        struct line_address *arg1, *arg2, *result;
                } binary_op;
                struct {
                        struct line_address *arg;
                        char *label;
                } jump_conditional;
                struct {
                        struct line_address *arg1, *arg2;
                        char *label;
                } jump_relational;
        } val;
};

struct assembly {
        struct line **lines;
        unsigned int n_lines;
};

struct assembly *create_assembly();
struct line_address *create_line_address_name(enum basic_type, char *);
struct line_address *create_line_address_constant(enum basic_type, union value);
struct line_address *create_line_address_offset(enum basic_type, int, enum reg);
struct line_address *create_line_address_register(enum basic_type, enum reg);
struct line *create_line_load(struct line_address *, struct line_address *);
struct line *create_line_load_address(struct line_address *, struct line_address *);
struct line *create_line_store(struct line_address *, struct line_address *);
struct line *create_line_add(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_sub(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_or(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_and(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_seq(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_sne(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_slt(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_sle(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_sgt(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_sge(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_mul(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_div(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_rem(struct line_address *, struct line_address *, struct line_address *);
struct line *create_line_not(struct line_address *, struct line_address *);
struct line *create_line_jump(char *);
struct line *create_line_beqz(struct line_address *, char *);
struct line *create_line_bnez(struct line_address *, char *);
struct line *create_line_beq(struct line_address *, struct line_address *, char *);
struct line *create_line_bne(struct line_address *, struct line_address *, char *);
struct line *create_line_blt(struct line_address *, struct line_address *, char *);
struct line *create_line_ble(struct line_address *, struct line_address *, char *);
struct line *create_line_bgt(struct line_address *, struct line_address *, char *);
struct line *create_line_bge(struct line_address *, struct line_address *, char *);
struct line *create_line_syscall();

void add_line(struct assembly *, struct line *);

struct assembly *parse_assembly(struct symbol_table *, struct instructions *);
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
void parse_assembly_builtin_puti(struct symbol_table *, struct symbol_table *, struct assembly *, struct quad *);

struct line_address *translate_quad_address(struct symbol_table *, struct symbol_table *, struct quad_address *);

#endif  /* CFLAT_ASM_H */
