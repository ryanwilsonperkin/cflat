#include <assert.h>
#include <stdlib.h>

#include "asm.h"

struct assembly *create_assembly
()
{
        int i;
        struct assembly *this = malloc(sizeof(struct assembly));
        this->lines = NULL;
        this->n_lines = 0;
        for (i = 0; i < NUM_REGS; i++) {
                this->regs[i] = 0;
        }
        return this;
}

void add_line
(struct assembly *assembly, line l)
{
        assembly->n_lines++;
        assembly->lines = realloc(assembly->lines, sizeof(line) * assembly->n_lines);
        assembly->lines[assembly->n_lines - 1] = l;
}

int get_reg
(struct assembly *assembly)
{
        int i;
        for (i = 0; i < NUM_REGS; i++) {
                if (!assembly->regs[i]) {
                        assembly->regs[i] = 1;
                        return i;
                }
        }
        assert(0);  /* Out of registers. */
}

void unget_reg
(struct assembly *assembly, int reg)
{
        assert(assembly->regs[reg]);  /* Register must be allocated. */
        assembly->regs[reg] = 0;
}

struct assembly *parse_assembly
(struct symbol_table *global, struct instructions *instructions)
{
        int i;
        struct assembly *this = create_assembly();
        for (i = 0; i < instructions->n_quads; i++) {
                parse_assembly_instruction(global, this, instructions->quads[i]);
        }
        return this;
}

void parse_assembly_instruction
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
        switch (this->type) {
        case QUAD_BINARY_ASSIGN:
                parse_assembly_binary_assign(global, assembly, this);
                break;
        case QUAD_UNARY_ASSIGN:
                parse_assembly_unary_assign(global, assembly, this);
                break;
        case QUAD_COPY:
                parse_assembly_copy(global, assembly, this);
                break;
        case QUAD_COPY_ADDR:
                parse_assembly_copy_addr(global, assembly, this);
                break;
        case QUAD_COPY_FROM_ADDR:
                parse_assembly_copy_from_addr(global, assembly, this);
                break;
        case QUAD_COPY_TO_ADDR:
                parse_assembly_copy_to_addr(global, assembly, this);
                break;
        case QUAD_LABEL:
                parse_assembly_label(global, assembly, this);
                break;
        case QUAD_UNCONDITIONAL_JUMP:
                parse_assembly_unconditional_jump(global, assembly, this);
                break;
        case QUAD_CONDITIONAL_JUMP:
                parse_assembly_conditional_jump(global, assembly, this);
                break;
        case QUAD_RELATIONAL_JUMP:
                parse_assembly_relational_jump(global, assembly, this);
                break;
        case QUAD_PROCEDURE_PARAM:
                parse_assembly_procedure_param(global, assembly, this);
                break;
        case QUAD_PROCEDURE_CALL:
                parse_assembly_procedure_call(global, assembly, this);
                break;
        case QUAD_PROCEDURE_RETURN:
                parse_assembly_procedure_return(global, assembly, this);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void parse_assembly_binary_assign
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
        struct quad_address *arg1, *arg2, *result;
        int reg_arg1, reg_arg2, reg_result;
        reg_arg1 = get_reg(assembly);
        reg_arg2 = get_reg(assembly);
        reg_result = get_reg(assembly);
        arg1 = this->val.binary_assign.arg1;
        arg2 = this->val.binary_assign.arg2;
        result = this->val.binary_assign.result;
        add_line(assembly, create_load_word(reg_arg1, arg1));
        add_line(assembly, create_load_word(reg_arg2, arg2));
        add_line(assembly, create_load_word(reg_result, result));
        add_line(assembly, create_store_word(reg_arg1, arg1));
        add_line(assembly, create_store_word(reg_arg2, arg2));
        add_line(assembly, create_store_word(reg_result, result));
        unget_reg(assembly, reg_arg1);
        unget_reg(assembly, reg_arg2);
        unget_reg(assembly, reg_result);
}

void parse_assembly_unary_assign
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_addr
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_from_addr
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_to_addr
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_label
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_unconditional_jump
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_conditional_jump
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_relational_jump
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_param
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_call
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_return
(struct symbol_table *global, struct assembly *assembly, struct quad *this)
{
}

line create_load_word(int reg, struct quad_address *addr)
{
        return "";
}

line create_store_word(int reg, struct quad_address *addr)
{
        return "";
}
