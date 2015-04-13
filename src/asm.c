#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"

struct assembly *create_assembly
()
{
        int i;
        struct assembly *this = malloc(sizeof(struct assembly));
        this->lines = NULL;
        this->n_lines = 0;
        return this;
}

struct line_address *create_line_address_name
(enum basic_type basic_type, char *name)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_NAME;
        this->basic_type = basic_type;
        this->val.name = name;
        return this;
}

struct line_address *create_line_address_constant
(enum basic_type basic_type, union value constant)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_CONSTANT;
        this->basic_type = basic_type;
        this->val.constant = constant;
        return this;
}

struct line_address *create_line_address_offset
(enum basic_type basic_type, int offset, enum reg reg)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_OFFSET;
        this->basic_type = basic_type;
        this->val.offset.offset = offset;
        this->val.offset.reg = reg;
        return this;
}

struct line_address *create_line_address_register
(enum basic_type basic_type, enum reg reg)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_REG;
        this->basic_type = basic_type;
        this->val.reg = reg;
        return this;
}

struct line *create_line_load(struct line_address *src, struct line_address *dest)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_LOAD;
        this->val.unary_op.arg = src;
        this->val.unary_op.result = dest;
        return this;
}

struct line *create_line_load_address(struct line_address *src, struct line_address *dest)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_LOAD_ADDRESS;
        this->val.unary_op.arg = src;
        this->val.unary_op.result = dest;
        return this;
}

struct line *create_line_store(struct line_address *src, struct line_address *dest)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_STORE;
        this->val.unary_op.arg = src;
        this->val.unary_op.result = dest;
        return this;
}

struct line *create_line_label
(char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_LABEL;
        this->val.label = label;
        return this;
}

struct line *create_line_add
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_ADD;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_sub
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SUB;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_or
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_OR;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_and
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_AND;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_seq
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SEQ;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_sne
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SNE;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_slt
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SLT;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_sle
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SLE;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_sgt
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SGT;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_sge
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_SGE;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_mul
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_MUL;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_div
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_DIV;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_rem
(struct line_address *arg1, struct line_address *arg2, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_REM;
        this->val.binary_op.arg1 = arg1;
        this->val.binary_op.arg2 = arg2;
        this->val.binary_op.result = result;
        return this;
}

struct line *create_line_not
(struct line_address *arg, struct line_address *result)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_NOT;
        this->val.unary_op.arg = arg;
        this->val.unary_op.result = result;
        return this;
}

struct line *create_line_jump
(char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_JUMP;
        this->val.label = label;
        return this;
}

struct line *create_line_beqz
(struct line_address *arg, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BEQZ;
        this->val.jump_conditional.arg = arg;
        this->val.jump_conditional.label = label;
        return this;
}

struct line *create_line_bnez
(struct line_address *arg, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BNEZ;
        this->val.jump_conditional.arg = arg;
        this->val.jump_conditional.label = label;
        return this;
}

struct line *create_line_beq
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BEQ;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

struct line *create_line_bne
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BNE;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

struct line *create_line_blt
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BLT;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

struct line *create_line_ble
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BLE;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

struct line *create_line_bgt
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BGT;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

struct line *create_line_bge
(struct line_address *arg1, struct line_address *arg2, char *label)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_BGE;
        this->val.jump_relational.arg1 = arg1;
        this->val.jump_relational.arg2 = arg2;
        this->val.jump_relational.label = label;
        return this;
}

void add_line
(struct assembly *assembly, struct line *line)
{
        assembly->n_lines++;
        assembly->lines = realloc(assembly->lines, sizeof(struct line *) * assembly->n_lines);
        assembly->lines[assembly->n_lines - 1] = line;
}

struct assembly *parse_assembly
(struct symbol_table *global, struct instructions *instructions)
{
        int i;
        struct symbol *symbol;
        struct quad *quad;
        struct assembly *this = create_assembly();
        struct symbol_table *local = global;
        for (i = 0; i < instructions->n_quads; i++) {
                quad = instructions->quads[i];
                switch (quad->type) {
                case QUAD_BINARY_ASSIGN:
                        parse_assembly_binary_assign(global, local, this, quad);
                        break;
                case QUAD_UNARY_ASSIGN:
                        parse_assembly_unary_assign(global, local, this, quad);
                        break;
                case QUAD_COPY:
                        parse_assembly_copy(global, local, this, quad);
                        break;
                case QUAD_COPY_ADDR:
                        parse_assembly_copy_addr(global, local, this, quad);
                        break;
                case QUAD_COPY_FROM_ADDR:
                        parse_assembly_copy_from_addr(global, local, this, quad);
                        break;
                case QUAD_COPY_TO_ADDR:
                        parse_assembly_copy_to_addr(global, local, this, quad);
                        break;
                case QUAD_LABEL:
                        symbol = get_symbol(global, quad->val.label.label);
                        if (symbol && symbol->type == SYMBOL_FUNCTION) {
                                local = symbol->scoped_table;
                        }
                        parse_assembly_label(global, local, this, quad);
                        break;
                case QUAD_UNCONDITIONAL_JUMP:
                        parse_assembly_unconditional_jump(global, local, this, quad);
                        break;
                case QUAD_CONDITIONAL_JUMP:
                        parse_assembly_conditional_jump(global, local, this, quad);
                        break;
                case QUAD_RELATIONAL_JUMP:
                        parse_assembly_relational_jump(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_PARAM:
                        parse_assembly_procedure_param(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_CALL:
                        parse_assembly_procedure_call(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_RETURN:
                        parse_assembly_procedure_return(global, local, this, quad);
                        break;
                default:
                        assert(0);  /* Invalid enum value. */
                }
        }
        return this;
}

void parse_assembly_binary_assign
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg1, *arg1_reg;
        struct line_address *arg2, *arg2_reg;
        struct line_address *result, *result_reg;
        arg1 = translate_quad_address(global, local, this->val.binary_assign.arg1);
        arg2 = translate_quad_address(global, local, this->val.binary_assign.arg2);
        result = translate_quad_address(global, local, this->val.binary_assign.result);
        arg1_reg = create_line_address_register(arg1->basic_type, REG_TEMP1);
        arg2_reg = create_line_address_register(arg2->basic_type, REG_TEMP2);
        result_reg = create_line_address_register(result->basic_type, REG_TEMP3);
        add_line(assembly, create_line_load(arg1, arg1_reg));
        add_line(assembly, create_line_load(arg2, arg2_reg));
        switch (this->val.binary_assign.op) {
        case QUAD_OP_LOGICAL_OR:
                add_line(assembly, create_line_or(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_LOGICAL_AND:
                add_line(assembly, create_line_and(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_EQUAL:
                add_line(assembly, create_line_seq(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_NOT_EQUAL:
                add_line(assembly, create_line_sne(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_LESS_THAN:
                add_line(assembly, create_line_slt(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_LESS_THAN_OR_EQUAL:
                add_line(assembly, create_line_sle(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_GREATER_THAN:
                add_line(assembly, create_line_sgt(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_GREATER_THAN_OR_EQUAL:
                add_line(assembly, create_line_sge(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_ADD:
                add_line(assembly, create_line_add(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_SUBTRACT:
                add_line(assembly, create_line_sub(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_MULTIPLY:
                add_line(assembly, create_line_mul(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_DIVIDE:
                add_line(assembly, create_line_div(arg1_reg, arg2_reg, result_reg));
                break;
        case QUAD_OP_MODULO:
                add_line(assembly, create_line_rem(arg1_reg, arg2_reg, result_reg));
                break;
        default:
                assert(0);  /* Invalid enum value for binary_op. */
        }
        /* binary op */
        add_line(assembly, create_line_store(result_reg, result));
}

void parse_assembly_unary_assign
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg;
        struct line_address *result, *result_reg;
        arg = translate_quad_address(global, local, this->val.unary_assign.arg);
        result = translate_quad_address(global, local, this->val.unary_assign.result);
        arg_reg = create_line_address_register(arg->basic_type, REG_TEMP1);
        result_reg = create_line_address_register(arg->basic_type, REG_TEMP2);
        add_line(assembly, create_line_load(arg, arg_reg));
        add_line(assembly, create_line_load(result, result_reg));
        switch (this->val.unary_assign.op) {
        case QUAD_OP_NOT_UNARY:
                add_line(assembly, create_line_not(arg_reg, result_reg));
                break;
        default:
                assert(0);  /* Invalid enum value for unary_op. */
        }
        add_line(assembly, create_line_store(result_reg, result));
}

void parse_assembly_copy
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg;
        struct line_address *result;
        arg = translate_quad_address(global, local, this->val.copy.arg);
        result = translate_quad_address(global, local, this->val.copy.result);
        arg_reg = create_line_address_register(arg->basic_type, REG_TEMP1);
        add_line(assembly, create_line_load(arg, arg_reg));
        add_line(assembly, create_line_store(arg_reg, result));
}

void parse_assembly_copy_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg;
        struct line_address *result;
        arg = translate_quad_address(global, local, this->val.copy.arg);
        result = translate_quad_address(global, local, this->val.copy.result);
        arg_reg = create_line_address_register(arg->basic_type, REG_TEMP1);
        add_line(assembly, create_line_load_address(arg, arg_reg));
        add_line(assembly, create_line_store(arg_reg, result));
}

void parse_assembly_copy_from_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg1, *arg_reg2;
        struct line_address *result;
        arg = translate_quad_address(global, local, this->val.copy.arg);
        result = translate_quad_address(global, local, this->val.copy.result);
        arg_reg1 = create_line_address_register(arg->basic_type, REG_TEMP1);
        arg_reg2 = create_line_address_register(arg->basic_type, REG_TEMP2);
        add_line(assembly, create_line_load(arg, arg_reg1));
        add_line(assembly, create_line_load_address(arg_reg1, arg_reg2));
        add_line(assembly, create_line_store(arg_reg2, result));
}

void parse_assembly_copy_to_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg1, *arg_reg2;
        struct line_address *result;
        arg = translate_quad_address(global, local, this->val.copy.arg);
        result = translate_quad_address(global, local, this->val.copy.result);
        arg_reg1 = create_line_address_register(arg->basic_type, REG_TEMP1);
        arg_reg2 = create_line_address_register(arg->basic_type, REG_TEMP2);
        add_line(assembly, create_line_load(arg, arg_reg1));
        add_line(assembly, create_line_load(result, arg_reg2));
        add_line(assembly, create_line_store(arg_reg1, arg_reg2));
}

void parse_assembly_label
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        int temp_offset, local_offset, total_offset;
        struct line_address *sp_offset, *sp;
        struct symbol *symbol;
        symbol = get_symbol(global, this->val.label.label);
        add_line(assembly, create_line_label(this->val.label.label));
        if (symbol && symbol->type == SYMBOL_FUNCTION) {
                temp_offset = symbol->scoped_table->n_temps * 4;
                local_offset = symbol->scoped_table->size;
                total_offset = temp_offset + local_offset;
                sp_offset = create_line_address_constant(INT_TYPE, (union value)total_offset);
                sp = create_line_address_register(INT_TYPE, REG_SP);
                add_line(assembly, create_line_sub(sp, sp_offset, sp));
        }
}

void parse_assembly_unconditional_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        add_line(assembly, create_line_jump(this->val.unconditional_jump.label));
}

void parse_assembly_conditional_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg, *arg_reg;
        arg = translate_quad_address(global, local, this->val.conditional_jump.arg);
        arg_reg = create_line_address_register(INT_TYPE, REG_TEMP0);
        add_line(assembly, create_line_load(arg, arg_reg));
        switch (this->val.conditional_jump.op) {
        case QUAD_OP_TRUE:
                add_line(assembly, create_line_bnez(arg_reg, this->val.conditional_jump.label));
                break;
        case QUAD_OP_FALSE:
                add_line(assembly, create_line_beqz(arg_reg, this->val.conditional_jump.label));
                break;
        default:
                assert(0);  /* Invalid enum value for conditional_jump. */
        }
}

void parse_assembly_relational_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
        struct line_address *arg1, *arg1_reg, *arg2, *arg2_reg;
        arg1 = translate_quad_address(global, local, this->val.relational_jump.arg1);
        arg2 = translate_quad_address(global, local, this->val.relational_jump.arg2);
        arg1_reg = create_line_address_register(INT_TYPE, REG_TEMP0);
        arg2_reg = create_line_address_register(INT_TYPE, REG_TEMP1);
        add_line(assembly, create_line_load(arg1, arg1_reg));
        add_line(assembly, create_line_load(arg2, arg2_reg));
        switch (this->val.relational_jump.op) {
        case QUAD_OP_EQUAL:
                add_line(assembly, create_line_beq(arg1, arg2, this->val.relational_jump.label));
                break;
        case QUAD_OP_NOT_EQUAL:
                add_line(assembly, create_line_bne(arg1, arg2, this->val.relational_jump.label));
                break;
        case QUAD_OP_LESS_THAN:
                add_line(assembly, create_line_blt(arg1, arg2, this->val.relational_jump.label));
                break;
        case QUAD_OP_LESS_THAN_OR_EQUAL:
                add_line(assembly, create_line_ble(arg1, arg2, this->val.relational_jump.label));
                break;
        case QUAD_OP_GREATER_THAN:
                add_line(assembly, create_line_bgt(arg1, arg2, this->val.relational_jump.label));
                break;
        case QUAD_OP_GREATER_THAN_OR_EQUAL:
                add_line(assembly, create_line_bge(arg1, arg2, this->val.relational_jump.label));
                break;
        default:
                assert(0);  /* Invalid enum value for relational_jump. */
        }
}

void parse_assembly_procedure_param
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_call
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_return
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

/* TODO: Get type of values, not just INT */
struct line_address *translate_quad_address
(struct symbol_table *global, struct symbol_table *local, struct quad_address *this)
{
        struct symbol *symbol;
        int offset;
        switch (this->type) {
        case ADDRESS_NAME:
                if (get_symbol(local, this->val.id)) {
                        offset = (local->n_temps * 4) + get_offset(local, this->val.id);
                        return create_line_address_offset(INT_TYPE, offset, REG_SP);
                } else {
                        return create_line_address_name(INT_TYPE, this->val.id);
                }
        case ADDRESS_CONSTANT:
                return create_line_address_constant(INT_TYPE, this->val.constant.val);
        case ADDRESS_TEMP:
                offset = this->val.temp * 4;
                return create_line_address_offset(INT_TYPE, offset, REG_SP);
        }
}
