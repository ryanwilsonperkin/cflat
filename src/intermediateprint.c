#include <assert.h>
#include <stdio.h>

#include "intermediate.h"
#include "intermediateprint.h"

void print_intermediate
(FILE *out, struct instructions *instructions)
{
        int i;
        if (!instructions) return;
        for (i = 0; i < instructions->n_quads; i++) {
                print_quad(out, instructions->quads[i]);
                fprintf(out, "\n");
        }
}

void print_quad
(FILE *out, struct quad *quad)
{
        if (quad->type != QUAD_LABEL) {
                fprintf(out, "\t");
        }
        switch (quad->type) {
        case QUAD_BINARY_ASSIGN:
                print_quad_binary_assign(out, quad);
                break;
        case QUAD_UNARY_ASSIGN:
                print_quad_unary_assign(out, quad);
                break;
        case QUAD_COPY:
                print_quad_copy(out, quad);
                break;
        case QUAD_COPY_ADDR:
                print_quad_copy_addr(out, quad);
                break;
        case QUAD_COPY_FROM_ADDR:
                print_quad_copy_from_addr(out, quad);
                break;
        case QUAD_COPY_TO_ADDR:
                print_quad_copy_to_addr(out, quad);
                break;
        case QUAD_LABEL:
                print_quad_label(out, quad);
                break;
        case QUAD_UNCONDITIONAL_JUMP:
                print_quad_unconditional_jump(out, quad);
                break;
        case QUAD_CONDITIONAL_JUMP:
                print_quad_conditional_jump(out, quad);
                break;
        case QUAD_RELATIONAL_JUMP:
                print_quad_relational_jump(out, quad);
                break;
        case QUAD_PROCEDURE_PARAM:
                print_quad_procedure_param(out, quad);
                break;
        case QUAD_PROCEDURE_CALL:
                print_quad_procedure_call(out, quad);
                break;
        case QUAD_PROCEDURE_RETURN:
                print_quad_procedure_return(out, quad);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void print_quad_binary_assign
(FILE *out, struct quad *quad)
{
        print_quad_address(out, quad->val.binary_assign.result);
        fprintf(out, " := ");
        print_quad_op_binary(out, quad->val.binary_assign.arg1, quad->val.binary_assign.arg2, quad->val.binary_assign.op);
}

void print_quad_unary_assign
(FILE *out, struct quad *quad)
{
        print_quad_address(out, quad->val.unary_assign.result);
        fprintf(out, " := ");
        print_quad_op_unary(out, quad->val.unary_assign.arg, quad->val.unary_assign.op);
}

void print_quad_copy
(FILE *out, struct quad *quad)
{
        print_quad_address(out, quad->val.copy.result);
        fprintf(out, " := ");
        print_quad_address(out, quad->val.copy.arg);
}

void print_quad_copy_addr
(FILE *out, struct quad *quad)
{
        print_quad_address(out, quad->val.copy_addr.result);
        fprintf(out, " := &");
        print_quad_address(out, quad->val.copy_addr.arg);
}

void print_quad_copy_from_addr
(FILE *out, struct quad *quad)
{
        print_quad_address(out, quad->val.copy_from_addr.result);
        fprintf(out, " := *");
        print_quad_address(out, quad->val.copy_from_addr.arg);
}

void print_quad_copy_to_addr
(FILE *out, struct quad *quad)
{
        fprintf(out, "*");
        print_quad_address(out, quad->val.copy_to_addr.result);
        fprintf(out, " := ");
        print_quad_address(out, quad->val.copy_to_addr.arg);
}

void print_quad_label
(FILE *out, struct quad *quad)
{
        fprintf(out, "%s:", quad->val.label.label);
}

void print_quad_unconditional_jump
(FILE *out, struct quad *quad)
{
        fprintf(out, "goto %s", quad->val.unconditional_jump.label);
}

void print_quad_conditional_jump
(FILE *out, struct quad *quad)
{
        switch (quad->val.conditional_jump.op) {
        case QUAD_OP_TRUE:
                fprintf(out, "if ");
                break;
        case QUAD_OP_FALSE:
                fprintf(out, "ifFalse ");
                break;
        default:
                assert(0);  /* Invalid enum value for conditional jump. */
        }
        print_quad_address(out, quad->val.conditional_jump.arg);
        fprintf(out, " goto %s", quad->val.conditional_jump.label);
}

void print_quad_relational_jump
(FILE *out, struct quad *quad)
{
        fprintf(out, "if ");
        print_quad_op_binary(out, quad->val.relational_jump.arg1, quad->val.relational_jump.arg2, quad->val.relational_jump.op);
        fprintf(out, " goto %s", quad->val.relational_jump.label);
}

void print_quad_procedure_param
(FILE *out, struct quad *quad)
{
        fprintf(out, "param ");
        print_quad_address(out, quad->val.procedure_param.arg);
}

void print_quad_procedure_call
(FILE *out, struct quad *quad)
{
        fprintf(out, "call %s,%u", quad->val.procedure_call.label, quad->val.procedure_call.n_params);
}

void print_quad_procedure_return
(FILE *out, struct quad *quad)
{
        fprintf(out, "return ");
        if (quad->val.procedure_return.result) {
                print_quad_address(out, quad->val.procedure_return.result);
        }
}


void print_quad_op_binary
(FILE *out, struct quad_address *arg1, struct quad_address *arg2, enum quad_op op)
{
        print_quad_address(out, arg1);
        switch (op) {
        case QUAD_OP_EQUAL:
                fprintf(out, " == ");
                break;
        case QUAD_OP_NOT_EQUAL:
                fprintf(out, " != ");
                break;
        case QUAD_OP_LESS_THAN:
                fprintf(out, " < ");
                break;
        case QUAD_OP_LESS_THAN_OR_EQUAL:
                fprintf(out, " <= ");
                break;
        case QUAD_OP_GREATER_THAN:
                fprintf(out, " > ");
                break;
        case QUAD_OP_GREATER_THAN_OR_EQUAL:
                fprintf(out, " >= ");
                break;
        case QUAD_OP_ADD:
                fprintf(out, " + ");
                break;
        case QUAD_OP_SUBTRACT:
                fprintf(out, " - ");
                break;
        case QUAD_OP_MULTIPLY:
                fprintf(out, " * ");
                break;
        case QUAD_OP_DIVIDE:
                fprintf(out, " / ");
                break;
        case QUAD_OP_MODULO:
                fprintf(out, " %% ");
                break;
        default:
                assert(0);  /* Invalid enum value for binary op. */
        }
        print_quad_address(out, arg2);
}

void print_quad_op_unary
(FILE *out, struct quad_address *arg, enum quad_op op)
{
        switch (op) {
        case QUAD_OP_NOT_UNARY:
                fprintf(out, "!");
                print_quad_address(out, arg);
                break;
        case QUAD_OP_POSITIVE:
                fprintf(out, "+");
                print_quad_address(out, arg);
                break;
        case QUAD_OP_NEGATIVE:
                fprintf(out, "-");
                print_quad_address(out, arg);
                break;
        case QUAD_OP_PRE_INCREMENT:
                fprintf(out, "++");
                print_quad_address(out, arg);
                break;
        case QUAD_OP_PRE_DECREMENT:
                fprintf(out, "--");
                print_quad_address(out, arg);
                break;
        case QUAD_OP_POST_INCREMENT:
                print_quad_address(out, arg);
                fprintf(out, "++");
                break;
        case QUAD_OP_POST_DECREMENT:
                print_quad_address(out, arg);
                fprintf(out, "--");
                break;
        default:
                assert(0);  /* Invalid enum value for unary op. */
        }
}

void print_quad_address
(FILE *out, struct quad_address *quad_address)
{
        switch (quad_address->type) {
        case ADDRESS_NAME:
                fprintf(out, "%s", quad_address->val.id);
                break;
        case ADDRESS_CONSTANT:
                switch (quad_address->val.constant.type) {
                case CHAR_TYPE:
                        switch (quad_address->val.constant.val.cval) {
                        case '\t':
                                fprintf(out, "'\\t'");
                                break;
                        case '\n':
                                fprintf(out, "'\\n'");
                                break;
                        default:
                                fprintf(out, "'%c'", quad_address->val.constant.val.cval);
                                break;
                        }
                        break;
                case INT_TYPE:
                        fprintf(out, "%d", quad_address->val.constant.val.ival);
                        break;
                case FLOAT_TYPE:
                        fprintf(out, "%f", quad_address->val.constant.val.fval);
                        break;
                default:
                        assert(0); /* Invalid enum value. */
                }
                break;
        case ADDRESS_TEMP:
                fprintf(out, "t%u", quad_address->val.temp);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}
