#include <assert.h>
#include <stdio.h>

#include "asm.h"
#include "asmprint.h"

#define ENCLOSED 1
#define UNENCLOSED 0

void print_assembly
(FILE *out, struct symbol_table *global, struct assembly *this)
{
        int i;
        fprintf(out, ".data\n");
        print_assembly_symbols(out, global);
        fprintf(out, ".text\n");
        fprintf(out, ".globl main\n");
        for (i = 0; i < this->n_lines; i++) {
                print_line(out, this->lines[i]);
                fprintf(out, "\n");
        }
        print_putc(out);
        print_putf(out);
        print_puti(out);
}

void print_assembly_symbols
(FILE *out, struct symbol_table *global)
{
        int i;
        char *id;
        struct symbol *symbol;
        for (i = 0; i < global->n_items; i++) {
                id = global->items[i]->id;
                symbol = global->items[i]->symbol;
                if (symbol->type == SYMBOL_FUNCTION) {
                        continue;
                }
                fprintf(out, "%s:\t.space %d\n", id, symbol->size);
        }
}

void print_putc
(FILE *out)
{
}

void print_putf
(FILE *out)
{
}

void print_puti
(FILE *out)
{
}

void print_reg
(FILE *out, enum reg this)
{
        switch (this) {
        case REG_A0:
                fprintf(out, "$a0");
                break;
        case REG_V0:
                fprintf(out, "$v0");
                break;
        case REG_SP:
                fprintf(out, "$sp");
                break;
        case REG_FP:
                fprintf(out, "$fp");
                break;
        case REG_RA:
                fprintf(out, "$ra");
                break;
        case REG_TEMP0:
                fprintf(out, "$t0");
                break;
        case REG_TEMP1:
                fprintf(out, "$t1");
                break;
        case REG_TEMP2:
                fprintf(out, "$t2");
                break;
        case REG_TEMP3:
                fprintf(out, "$t3");
                break;
        case REG_TEMP4:
                fprintf(out, "$t4");
                break;
        case REG_TEMP5:
                fprintf(out, "$t5");
                break;
        case REG_TEMP6:
                fprintf(out, "$t6");
                break;
        case REG_TEMP7:
                fprintf(out, "$t7");
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void print_line_address
(FILE *out, struct line_address *this, int enclosed)
{
        switch (this->type) {
        case LINE_ADDRESS_NAME:
                print_line_address_name(out, this);
                break;
        case LINE_ADDRESS_CONSTANT:
                print_line_address_constant(out, this);
                break;
        case LINE_ADDRESS_OFFSET:
                print_line_address_offset(out, this);
                break;
        case LINE_ADDRESS_REG:
                print_line_address_reg(out, this, enclosed);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void print_line_address_name
(FILE *out, struct line_address *this)
{
        fprintf(out, "%s", this->val.name);
}

void print_line_address_constant
(FILE *out, struct line_address *this)
{
        switch (this->basic_type) {
        case CHAR_TYPE:
                fprintf(out, "%d", (int)this->val.constant.cval);
                break;
        case INT_TYPE:
                fprintf(out, "%d", this->val.constant.ival);
                break;
        case FLOAT_TYPE:
                fprintf(out, "%f", this->val.constant.fval);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void print_line_address_offset
(FILE *out, struct line_address *this)
{
       fprintf(out, "%d(", this->val.offset.offset); 
       print_reg(out, this->val.offset.reg);
       fprintf(out, ")");
}

void print_line_address_reg
(FILE *out, struct line_address *this, int enclosed)
{
        if (enclosed) {
                fprintf(out, "(");
        }
        print_reg(out, this->val.reg);
        if (enclosed) {
                fprintf(out, ")");
        }
}

void print_line
(FILE *out, struct line *this)
{
        if (this->type != LINE_LABEL) {
                fprintf(out, "\t");
        }
        switch (this->type) {
        case LINE_LOAD:
                print_line_load(out, this);
                break;
        case LINE_LOAD_ADDRESS:
                print_line_load_address(out, this);
                break;
        case LINE_STORE:
                print_line_store(out, this);
                break;
        case LINE_LABEL:
                print_line_label(out, this);
                break;
        case LINE_ADD:
                print_line_add(out, this);
                break;
        case LINE_SUB:
                print_line_sub(out, this);
                break;
        case LINE_OR:
                print_line_or(out, this);
                break;
        case LINE_AND:
                print_line_and(out, this);
                break;
        case LINE_SEQ:
                print_line_seq(out, this);
                break;
        case LINE_SNE:
                print_line_sne(out, this);
                break;
        case LINE_SLT:
                print_line_slt(out, this);
                break;
        case LINE_SLE:
                print_line_sle(out, this);
                break;
        case LINE_SGT:
                print_line_sgt(out, this);
                break;
        case LINE_SGE:
                print_line_sge(out, this);
                break;
        case LINE_MUL:
                print_line_mul(out, this);
                break;
        case LINE_DIV:
                print_line_div(out, this);
                break;
        case LINE_REM:
                print_line_rem(out, this);
                break;
        case LINE_NOT:
                print_line_not(out, this);
                break;
        case LINE_JUMP:
                print_line_jump(out, this);
                break;
        case LINE_BEQZ:
                print_line_beqz(out, this);
                break;
        case LINE_BNEZ:
                print_line_bnez(out, this);
                break;
        case LINE_BEQ:
                print_line_beq(out, this);
                break;
        case LINE_BNE:
                print_line_bne(out, this);
                break;
        case LINE_BLT:
                print_line_blt(out, this);
                break;
        case LINE_BLE:
                print_line_ble(out, this);
                break;
        case LINE_BGT:
                print_line_bgt(out, this);
                break;
        case LINE_BGE:
                print_line_bge(out, this);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void print_line_load
(FILE *out, struct line *this)
{
        if (this->val.unary_op.arg->type == LINE_ADDRESS_CONSTANT) {
                fprintf(out, "li ");
        } else {
                fprintf(out, "lw ");
        }
        print_line_address(out, this->val.unary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.unary_op.arg, ENCLOSED);
}

void print_line_load_address
(FILE *out, struct line *this)
{
        fprintf(out, "la ");
        print_line_address(out, this->val.unary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.unary_op.arg, ENCLOSED);
}

void print_line_store
(FILE *out, struct line *this)
{
        fprintf(out, "sw ");
        print_line_address(out, this->val.unary_op.arg, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.unary_op.result, ENCLOSED);
}

void print_line_label
(FILE *out, struct line *this)
{
        fprintf(out, "%s:", this->val.label);
}

void print_line_add
(FILE *out, struct line *this)
{
        fprintf(out, "add ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_sub
(FILE *out, struct line *this)
{
        fprintf(out, "sub ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_or
(FILE *out, struct line *this)
{
        fprintf(out, "or ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_and
(FILE *out, struct line *this)
{
        fprintf(out, "and ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_seq
(FILE *out, struct line *this)
{
        fprintf(out, "seq ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_sne
(FILE *out, struct line *this)
{
        fprintf(out, "sne ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_slt
(FILE *out, struct line *this)
{
        fprintf(out, "slt ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_sle
(FILE *out, struct line *this)
{
        fprintf(out, "sle ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_sgt
(FILE *out, struct line *this)
{
        fprintf(out, "sgt ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_sge
(FILE *out, struct line *this)
{
        fprintf(out, "sge ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_mul
(FILE *out, struct line *this)
{
        fprintf(out, "mul ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_div
(FILE *out, struct line *this)
{
        fprintf(out, "div ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_rem
(FILE *out, struct line *this)
{
        fprintf(out, "rem ");
        print_line_address(out, this->val.binary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.binary_op.arg2, UNENCLOSED);
}

void print_line_not
(FILE *out, struct line *this)
{
        fprintf(out, "not ");
        print_line_address(out, this->val.unary_op.result, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.unary_op.arg, UNENCLOSED);
}

void print_line_jump
(FILE *out, struct line *this)
{
        fprintf(out, "j %s", this->val.label);
}

void print_line_beqz
(FILE *out, struct line *this)
{
        fprintf(out, "beqz ");
        print_line_address(out, this->val.jump_conditional.arg, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_conditional.label);
}

void print_line_bnez
(FILE *out, struct line *this)
{
        fprintf(out, "bnez ");
        print_line_address(out, this->val.jump_conditional.arg, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_conditional.label);
}

void print_line_beq
(FILE *out, struct line *this)
{
        fprintf(out, "beq ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}

void print_line_bne
(FILE *out, struct line *this)
{
        fprintf(out, "bne ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}

void print_line_blt
(FILE *out, struct line *this)
{
        fprintf(out, "blt ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}

void print_line_ble
(FILE *out, struct line *this)
{
        fprintf(out, "ble ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}

void print_line_bgt
(FILE *out, struct line *this)
{
        fprintf(out, "bgt ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}

void print_line_bge
(FILE *out, struct line *this)
{
        fprintf(out, "bge ");
        print_line_address(out, this->val.jump_relational.arg1, UNENCLOSED);
        fprintf(out, ",");
        print_line_address(out, this->val.jump_relational.arg2, UNENCLOSED);
        fprintf(out, ",%s", this->val.jump_relational.label);
}
