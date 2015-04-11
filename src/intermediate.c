#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intermediate.h"

static inline int num_digits
(unsigned int n)
{
        /* http://stackoverflow.com/a/1489928/3676516 */
        return n > 0 ? (int) log10(n) + 1 : 1;
}

struct instructions *create_instructions
()
{
        struct instructions *this = malloc(sizeof(struct instructions));
        this->quads = NULL;
        this->n_quads = 0;
        this->n_labels = 0;
        this->n_temps = 0;
        return this;
}

struct quad *create_quad_binary_assign
(struct quad_address *arg1, struct quad_address *arg2, struct quad_address *result, enum quad_op op)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_BINARY_ASSIGN;
        this->val.binary_assign.arg1 = arg1;
        this->val.binary_assign.arg2 = arg2;
        this->val.binary_assign.result = result;
        this->val.binary_assign.op = op;
        return this;
}

struct quad *create_quad_unary_assign
(struct quad_address *arg, struct quad_address *result, enum quad_op op)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_UNARY_ASSIGN;
        this->val.unary_assign.arg = arg;
        this->val.unary_assign.result = result;
        this->val.unary_assign.op = op;
        return this;
}

struct quad *create_quad_copy
(struct quad_address *arg, struct quad_address *result)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_COPY;
        this->val.copy.arg = arg;
        this->val.copy.result = result;
        return this;
}

struct quad *create_quad_label
(char *label)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_LABEL;
        this->val.label.label = label;
        return this;
}

struct quad *create_quad_unconditional_jump
(char *label)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_UNCONDITIONAL_JUMP;
        this->val.unconditional_jump.label = label;
        return this;
}

struct quad *create_quad_conditional_jump
(struct quad_address *arg, char *label)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_CONDITIONAL_JUMP;
        this->val.conditional_jump.arg = arg;
        this->val.conditional_jump.label = label;
        return this;
}

struct quad *create_quad_relational_jump
(struct quad_address *arg1, struct quad_address *arg2, enum quad_op op, char *label)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_RELATIONAL_JUMP;
        this->val.relational_jump.arg1 = arg1;
        this->val.relational_jump.arg2 = arg2;
        this->val.relational_jump.op = op;
        this->val.relational_jump.label = label;
        return this;
}

struct quad *create_quad_procedure_param
(struct quad_address *arg)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_PROCEDURE_PARAM;
        this->val.procedure_param.arg = arg;
        return this;
}

struct quad *create_quad_procedure_call
(char *label, unsigned int n_params)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_PROCEDURE_CALL;
        this->val.procedure_call.label = label;
        this->val.procedure_call.n_params = n_params;
        return this;
}

struct quad *create_quad_procedure_return
(struct quad_address *result)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_PROCEDURE_RETURN;
        this->val.procedure_return.result = result;
        return this;
}

struct quad_address *create_quad_address_name
(char *id)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_NAME;
        this->val.id = id;
        return this;
}

struct quad_address *create_quad_address_constant
(struct constant *constant)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_CONSTANT;
        this->val.constant = constant;
        return this;
}

struct quad_address *create_quad_address_temp
(unsigned int temp)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_TEMP;
        this->val.temp = temp;
        return this;
}

void add_instruction
(struct instructions *instructions, struct quad *quad)
{
        instructions->n_quads++;
        instructions->quads = realloc(instructions->quads, sizeof(struct quad *) * instructions->n_quads);
        instructions->quads[instructions->n_quads - 1] = quad;
}

struct quad *get_next_label
(struct instructions *instructions)
{
        char *label, *prefix = "L~";
        instructions->n_labels++;
        label = malloc(strlen(prefix) + num_digits(instructions->n_labels) + 1);
        sprintf(label, "%s%u", prefix, instructions->n_labels);
        return create_quad_label(label);
}

struct quad_address *get_next_temp
(struct instructions *instructions)
{
        return create_quad_address_temp(++instructions->n_temps);
}

struct instructions *parse_instructions
(struct program *program)
{
        struct instructions *this = create_instructions();
        parse_instructions_program(this, program);
        return this;
}

void parse_instructions_program
(struct instructions *instructions, struct program *this)
{
        if (!this) return;
        parse_instructions_function_def_list(instructions, this->function_def_list);
}

void parse_instructions_function_def_list
(struct instructions *instructions, struct function_def_list *this)
{
        if (!this) return;
        parse_instructions_function_def_list(instructions, this->function_def_list);
        parse_instructions_function_def(instructions, this->function_def);
}

void parse_instructions_function_def
(struct instructions *instructions, struct function_def *this)
{
        if (!this) return;
        add_instruction(instructions, create_quad_label(this->id));
        parse_instructions_function_body(instructions, this->function_body);
}

void parse_instructions_function_body
(struct instructions *instructions, struct function_body *this)
{
        if (!this) return;
        parse_instructions_stmt_list(instructions, this->stmt_list);
        parse_instructions_return_stmt(instructions, this->return_stmt);
}

void parse_instructions_stmt_list
(struct instructions *instructions, struct stmt_list *this)
{
        if (!this) return;
        parse_instructions_stmt_list(instructions, this->stmt_list);
        parse_instructions_stmt(instructions, this->stmt);
}

void parse_instructions_stmt
(struct instructions *instructions, struct stmt *this)
{
        if (!this) return;
        switch (this->type) {
        case EXPR_STMT:
                parse_instructions_expr_stmt(instructions, this->val.expr_stmt);
                break;
        case COMPOUND_STMT:
                parse_instructions_compound_stmt(instructions, this->val.compound_stmt);
                break;
        case SELECT_STMT:
                parse_instructions_select_stmt(instructions, this->val.select_stmt);
                break;
        case ITER_STMT:
                parse_instructions_iter_stmt(instructions, this->val.iter_stmt);
                break;
        case RETURN_STMT:
                parse_instructions_return_stmt(instructions, this->val.return_stmt);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void parse_instructions_expr_stmt
(struct instructions *instructions, struct expr_stmt *this)
{
        parse_instructions_expr(instructions, this->expr);
}

void parse_instructions_compound_stmt
(struct instructions *instructions, struct compound_stmt *this)
{
        parse_instructions_stmt_list(instructions, this->stmt_list);
}

void parse_instructions_select_stmt
(struct instructions *instructions, struct select_stmt *this)
{
        struct quad_address *result;
        struct quad *jump_to_if, *jump_to_else, *jump_to_end;
        struct quad *label_if, *label_else, *label_end;
        label_if = get_next_label(instructions);
        label_else = get_next_label(instructions);
        label_end = get_next_label(instructions);
        result = parse_instructions_expr(instructions, this->cond);
        jump_to_if = create_quad_conditional_jump(result, label_if->val.label.label);
        jump_to_else = create_quad_unconditional_jump(label_else->val.label.label);
        jump_to_end = create_quad_unconditional_jump(label_end->val.label.label);
        add_instruction(instructions, jump_to_if);
        add_instruction(instructions, jump_to_else);
        add_instruction(instructions, label_if);
        parse_instructions_stmt(instructions, this->stmt_if_true);
        add_instruction(instructions, jump_to_end);
        add_instruction(instructions, label_else);
        parse_instructions_stmt(instructions, this->stmt_if_false);
        add_instruction(instructions, label_end);
}

void parse_instructions_iter_stmt
(struct instructions *instructions, struct iter_stmt *this)
{
        struct quad_address *result;
        struct quad *label_loop, *label_end;
        struct quad *jump_to_loop, *jump_to_end;
        label_loop = get_next_label(instructions);
        label_end = get_next_label(instructions);
        if (this->init) {
                parse_instructions_expr(instructions, this->init);
        }
        add_instruction(instructions, label_loop);
        if (this->cond) {
                result = parse_instructions_expr(instructions, this->cond);
                jump_to_end = create_quad_conditional_jump(result, label_end->val.label.label);
                add_instruction(instructions, jump_to_end);
        }
        parse_instructions_stmt(instructions, this->body);
        if (this->after) {
                parse_instructions_expr(instructions, this->after);
        }
        jump_to_loop = create_quad_unconditional_jump(label_loop->val.label.label);
        add_instruction(instructions, jump_to_loop);
        add_instruction(instructions, label_end);
}

void parse_instructions_return_stmt
(struct instructions *instructions, struct return_stmt *this)
{
        struct quad_address *result;
        struct quad *procedure_return;
        result = parse_instructions_expr(instructions, this->expr);
        procedure_return = create_quad_procedure_return(result);
        add_instruction(instructions, procedure_return);
}

struct quad_address *parse_instructions_expr
(struct instructions *instructions, struct expr *this)
{
        if (!this) return NULL;
}

struct quad_address *parse_instructions_assign_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_logical_or_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_logical_and_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_equality_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_relational_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_additive_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_multiplicative_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_unary_expr
(struct instructions *instructions, struct expr *this)
{
}

struct quad_address *parse_instructions_postfix_expr
(struct instructions *instructions, struct expr *this)
{
}
