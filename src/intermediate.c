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

struct quad *create_quad_copy_addr
(struct quad_address *arg, struct quad_address *result)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_COPY_ADDR;
        this->val.copy_addr.arg = arg;
        this->val.copy_addr.result = result;
        return this;
}

struct quad *create_quad_copy_from_addr
(struct quad_address *arg, struct quad_address *result)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_COPY_FROM_ADDR;
        this->val.copy_from_addr.arg = arg;
        this->val.copy_from_addr.result = result;
        return this;
}

struct quad *create_quad_copy_to_addr
(struct quad_address *arg, struct quad_address *result)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_COPY_TO_ADDR;
        this->val.copy_to_addr.arg = arg;
        this->val.copy_to_addr.result = result;
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
(struct quad_address *arg, enum quad_op op, char *label)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_CONDITIONAL_JUMP;
        this->val.conditional_jump.arg = arg;
        this->val.conditional_jump.op = op;
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
(struct quad_address *result, char *label, unsigned int n_params)
{
        struct quad *this = malloc(sizeof(struct quad));
        this->type = QUAD_PROCEDURE_CALL;
        this->val.procedure_call.result = result;
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

struct quad_address *create_quad_address_const_char
(cflat_char c)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_CONSTANT;
        this->val.constant.type = CHAR_TYPE;
        this->val.constant.val.cval = c;
        return this;
}

struct quad_address *create_quad_address_const_float
(cflat_float f)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_CONSTANT;
        this->val.constant.type = FLOAT_TYPE;
        this->val.constant.val.fval = f;
        return this;
}

struct quad_address *create_quad_address_const_int
(cflat_int i)
{
        struct quad_address *this = malloc(sizeof(struct quad_address));
        this->type = ADDRESS_CONSTANT;
        this->val.constant.type = INT_TYPE;
        this->val.constant.val.ival = i;
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
        char *label, *prefix = "L";
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
(struct symbol_table *global, struct program *program)
{
        struct instructions *this = create_instructions();
        parse_instructions_program(global, this, program);
        return this;
}

void parse_instructions_program
(struct symbol_table *global, struct instructions *instructions, struct program *this)
{
        if (!this) return;
        parse_instructions_function_def_list(global, instructions, this->function_def_list);
}

void parse_instructions_function_def_list
(struct symbol_table *global, struct instructions *instructions, struct function_def_list *this)
{
        if (!this) return;
        parse_instructions_function_def_list(global, instructions, this->function_def_list);
        parse_instructions_function_def(global, instructions, this->function_def);
}

void parse_instructions_function_def
(struct symbol_table *global, struct instructions *instructions, struct function_def *this)
{
        struct symbol_table *local;
        if (!this) return;
        local = get_symbol(global, this->id)->scoped_table;
        add_instruction(instructions, create_quad_label(this->id));
        parse_instructions_function_body(global, local, instructions, this->function_body);
}

void parse_instructions_function_body
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct function_body *this)
{
        if (!this) return;
        parse_instructions_stmt_list(global, local, instructions, this->stmt_list);
        parse_instructions_return_stmt(global, local, instructions, this->return_stmt);
}

void parse_instructions_stmt_list
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct stmt_list *this)
{
        if (!this) return;
        parse_instructions_stmt_list(global, local, instructions, this->stmt_list);
        parse_instructions_stmt(global, local, instructions, this->stmt);
}

void parse_instructions_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct stmt *this)
{
        if (!this) return;
        switch (this->type) {
        case EXPR_STMT:
                parse_instructions_expr_stmt(global, local, instructions, this->val.expr_stmt);
                break;
        case COMPOUND_STMT:
                parse_instructions_compound_stmt(global, local, instructions, this->val.compound_stmt);
                break;
        case SELECT_STMT:
                parse_instructions_select_stmt(global, local, instructions, this->val.select_stmt);
                break;
        case ITER_STMT:
                parse_instructions_iter_stmt(global, local, instructions, this->val.iter_stmt);
                break;
        case RETURN_STMT:
                parse_instructions_return_stmt(global, local, instructions, this->val.return_stmt);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
}

void parse_instructions_expr_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr_stmt *this)
{
        parse_instructions_expr(global, local, instructions, this->expr);
}

void parse_instructions_compound_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct compound_stmt *this)
{
        parse_instructions_stmt_list(global, local, instructions, this->stmt_list);
}

void parse_instructions_select_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct select_stmt *this)
{
        struct quad_address *result;
        struct quad *jump_to_if, *jump_to_else, *jump_to_end;
        struct quad *label_if, *label_else, *label_end;
        label_if = get_next_label(instructions);
        label_else = get_next_label(instructions);
        label_end = get_next_label(instructions);
        result = parse_instructions_expr(global, local, instructions, this->cond);
        jump_to_if = create_quad_conditional_jump(result, QUAD_OP_TRUE, label_if->val.label.label);
        jump_to_else = create_quad_unconditional_jump(label_else->val.label.label);
        jump_to_end = create_quad_unconditional_jump(label_end->val.label.label);
        add_instruction(instructions, jump_to_if);
        add_instruction(instructions, jump_to_else);
        add_instruction(instructions, label_if);
        parse_instructions_stmt(global, local, instructions, this->stmt_if_true);
        add_instruction(instructions, jump_to_end);
        add_instruction(instructions, label_else);
        parse_instructions_stmt(global, local, instructions, this->stmt_if_false);
        add_instruction(instructions, label_end);
}

void parse_instructions_iter_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct iter_stmt *this)
{
        struct quad_address *result;
        struct quad *label_loop, *label_end;
        struct quad *jump_to_loop, *jump_to_end;
        label_loop = get_next_label(instructions);
        label_end = get_next_label(instructions);
        if (this->init) {
                parse_instructions_expr(global, local, instructions, this->init);
        }
        add_instruction(instructions, label_loop);
        if (this->cond) {
                result = parse_instructions_expr(global, local, instructions, this->cond);
                jump_to_end = create_quad_conditional_jump(result, QUAD_OP_TRUE, label_end->val.label.label);
                add_instruction(instructions, jump_to_end);
        }
        parse_instructions_stmt(global, local, instructions, this->body);
        if (this->after) {
                parse_instructions_expr(global, local, instructions, this->after);
        }
        jump_to_loop = create_quad_unconditional_jump(label_loop->val.label.label);
        add_instruction(instructions, jump_to_loop);
        add_instruction(instructions, label_end);
}

void parse_instructions_return_stmt
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct return_stmt *this)
{
        struct quad_address *result;
        struct quad *procedure_return;
        result = parse_instructions_expr(global, local, instructions, this->expr);
        procedure_return = create_quad_procedure_return(result);
        add_instruction(instructions, procedure_return);
}

struct quad_address *parse_instructions_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        if (!this) return NULL;
        switch (this->type) {
        case ASSIGN_EXPR:
                return parse_instructions_assign_expr(global, local, instructions, this);
        case LOGICAL_OR_EXPR:
                return parse_instructions_logical_or_expr(global, local, instructions, this);
        case LOGICAL_AND_EXPR:
                return parse_instructions_logical_and_expr(global, local, instructions, this);
        case EQUALITY_EXPR:
                return parse_instructions_equality_expr(global, local, instructions, this);
        case RELATIONAL_EXPR:
                return parse_instructions_relational_expr(global, local, instructions, this);
        case ADDITIVE_EXPR:
                return parse_instructions_additive_expr(global, local, instructions, this);
        case MULTIPLICATIVE_EXPR:
                return parse_instructions_multiplicative_expr(global, local, instructions, this);
        case UNARY_EXPR:
                return parse_instructions_unary_expr(global, local, instructions, this);
        case POSTFIX_EXPR:
                return parse_instructions_postfix_expr(global, local, instructions, this);
        default:
                assert(0);  /* Invalid enum value. */
        }
}

struct quad_address *parse_instructions_assign_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        struct quad_address *assignee, *assignee_addr, *assignment;
        assignee = parse_instructions_var(global, local, instructions, this->val.assign.assignee);
        assignee_addr = get_next_temp(instructions);
        add_instruction(instructions, create_quad_copy_addr(assignee, assignee_addr));
        assignment = parse_instructions_expr(global, local, instructions, this->val.assign.assignment);
        add_instruction(instructions, create_quad_copy_to_addr(assignment, assignee_addr));
        return assignee;
}

struct quad_address *parse_instructions_logical_or_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        struct quad_address *primary, *secondary, *result;
        primary = parse_instructions_expr(global, local, instructions, this->val.relation.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.relation.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, QUAD_OP_LOGICAL_OR));
        return result;
}

struct quad_address *parse_instructions_logical_and_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        struct quad_address *primary, *secondary, *result;
        primary = parse_instructions_expr(global, local, instructions, this->val.relation.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.relation.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, QUAD_OP_LOGICAL_AND));
        return result;
}

struct quad_address *parse_instructions_equality_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        enum quad_op op;
        struct quad_address *primary, *secondary, *result;
        switch (this->subtype.equality_expr_subtype) {
        case EQUALITY_EXPR_EQUAL:
                op = QUAD_OP_EQUAL;
                break;
        case EQUALITY_EXPR_NOT_EQUAL:
                op = QUAD_OP_NOT_EQUAL;
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        primary = parse_instructions_expr(global, local, instructions, this->val.relation.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.relation.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, op));
        return result;
}

struct quad_address *parse_instructions_relational_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        enum quad_op op;
        struct quad_address *primary, *secondary, *result;
        switch (this->subtype.relational_expr_subtype) {
        case RELATIONAL_EXPR_LESS_THAN:
                op = QUAD_OP_LESS_THAN;
                break;
        case RELATIONAL_EXPR_LESS_THAN_OR_EQUAL:
                op = QUAD_OP_LESS_THAN_OR_EQUAL;
                break;
        case RELATIONAL_EXPR_GREATER_THAN:
                op = QUAD_OP_GREATER_THAN;
                break;
        case RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL:
                op = QUAD_OP_GREATER_THAN_OR_EQUAL;
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        primary = parse_instructions_expr(global, local, instructions, this->val.relation.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.relation.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, op));
        return result;
}

struct quad_address *parse_instructions_additive_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        enum quad_op op;
        struct quad_address *primary, *secondary, *result;
        switch (this->subtype.additive_expr_subtype) {
        case ADDITIVE_EXPR_ADD:
                op = QUAD_OP_ADD;
                break;
        case ADDITIVE_EXPR_SUBTRACT:
                op = QUAD_OP_SUBTRACT;
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        primary = parse_instructions_expr(global, local, instructions, this->val.binary_op.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.binary_op.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, op));
        return result;
}

struct quad_address *parse_instructions_multiplicative_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        enum quad_op op;
        struct quad_address *primary, *secondary, *result;
        switch (this->subtype.multiplicative_expr_subtype) {
        case MULTIPLICATIVE_EXPR_MULTIPLY:
                op = QUAD_OP_MULTIPLY;
                break;
        case MULTIPLICATIVE_EXPR_DIVIDE:
                op = QUAD_OP_DIVIDE;
                break;
        case MULTIPLICATIVE_EXPR_MODULO:
                op = QUAD_OP_MODULO;
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        primary = parse_instructions_expr(global, local, instructions, this->val.binary_op.primary);
        secondary = parse_instructions_expr(global, local, instructions, this->val.binary_op.secondary);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_binary_assign(primary, secondary, result, op));
        return result;
}

struct quad_address *parse_instructions_unary_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        enum quad_op op;
        struct quad_address *expr, *result;
        struct symbol *symbol;
        struct expr *sub_expr;
        switch (this->subtype.unary_expr_subtype) {
        case UNARY_EXPR_SIZEOF_UNARY:
                sub_expr = this->val.unary_op.expr;
                if (sub_expr->type == POSTFIX_EXPR && sub_expr->subtype.postfix_expr_subtype == POSTFIX_EXPR_VAR) {
                        symbol = translate_var(global, local, sub_expr->val.postfix_op.var);
                        return create_quad_address_const_int(symbol->size);
                } else {
                        return create_quad_address_const_int(4);
                }
        case UNARY_EXPR_SIZEOF_BASIC:
                return create_quad_address_const_int(4);
        case UNARY_EXPR_NOT_UNARY:
                op = QUAD_OP_NOT_UNARY;
                break;
        case UNARY_EXPR_POSITIVE:
                op = QUAD_OP_POSITIVE;
                break;
        case UNARY_EXPR_NEGATIVE:
                op = QUAD_OP_NEGATIVE;
                break;
        case UNARY_EXPR_PRE_INCREMENT:
                op = QUAD_OP_PRE_INCREMENT;
                break;
        case UNARY_EXPR_PRE_DECREMENT:
                op = QUAD_OP_PRE_DECREMENT;
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        expr = parse_instructions_expr(global, local, instructions, this->val.unary_op.expr);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_unary_assign(expr, result, op));
        return result;
}

struct quad_address *parse_instructions_postfix_expr
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct expr *this)
{
        struct quad_address *expr, *result;
        switch (this->subtype.postfix_expr_subtype) {
        case POSTFIX_EXPR_VAR:
                return parse_instructions_var(global, local, instructions, this->val.postfix_op.var);
        case POSTFIX_EXPR_CONSTANT:
                return parse_instructions_constant(global, local, instructions, this->val.postfix_op.constant);
        case POSTFIX_EXPR_POST_INCREMENT:
                expr = parse_instructions_expr(global, local, instructions, this->val.postfix_op.expr);
                result = get_next_temp(instructions);
                add_instruction(instructions, create_quad_unary_assign(expr, result, QUAD_OP_POST_INCREMENT));
                return result;
        case POSTFIX_EXPR_POST_DECREMENT:
                expr = parse_instructions_expr(global, local, instructions, this->val.postfix_op.expr);
                result = get_next_temp(instructions);
                add_instruction(instructions, create_quad_unary_assign(expr, result, QUAD_OP_POST_DECREMENT));
                return result;
        case POSTFIX_EXPR_ENCLOSED:
                return parse_instructions_expr(global, local, instructions, this->val.postfix_op.expr);
        case POSTFIX_EXPR_FUNCTION_CALL:
                return parse_instructions_function_call(global, local, instructions, this->val.postfix_op.function_call);
        default:
                assert(0);  /* Invalid enum value. */
        }
        return create_quad_address_const_int(1);
}

struct quad_address *parse_instructions_var
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct var *this)
{
        struct quad_address *offset, *final_offset, *base, *base_addr, *result, *result_addr, *width, *index;
        struct var *cur_var, *parent;
        struct symbol *parent_symbol;
        unsigned int offset_size;

        if (this->type == IDENTIFIER) {
                return create_quad_address_name(this->val.id);
        }

        cur_var = this;
        final_offset = get_next_temp(instructions);
        add_instruction(instructions, create_quad_copy(create_quad_address_const_int(0), final_offset));
        while (cur_var->type != IDENTIFIER) {
                final_offset = (final_offset) ? final_offset : get_next_temp(instructions);
                if (cur_var->type == FIELD) {
                        parent = cur_var->val.field.var;
                        parent_symbol = translate_var(global, local, parent);
                        offset_size = get_offset(parent_symbol->scoped_table, cur_var->val.field.id);
                        offset = create_quad_address_const_int(offset_size);
                } else if (cur_var->type == SUBSCRIPT) {
                        parent = cur_var->val.subscript.var;
                        parent_symbol = translate_var(global, local, parent);
                        offset_size = parent_symbol->val.symbol->size;
                        width = create_quad_address_const_int(offset_size);
                        index = parse_instructions_expr(global, local, instructions, cur_var->val.subscript.expr);
                        offset = get_next_temp(instructions);
                        add_instruction(instructions, create_quad_binary_assign(width, index, offset, QUAD_OP_MULTIPLY));
                }
                add_instruction(instructions, create_quad_binary_assign(final_offset, offset, final_offset, QUAD_OP_ADD));
                cur_var = parent;
        }
        base = create_quad_address_name(cur_var->val.id);
        base_addr = get_next_temp(instructions);
        result_addr = get_next_temp(instructions);
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_copy_addr(base, base_addr));
        add_instruction(instructions, create_quad_binary_assign(base_addr, final_offset, result_addr, QUAD_OP_ADD));
        add_instruction(instructions, create_quad_copy_from_addr(result_addr, result));
        return result;
}

struct quad_address *parse_instructions_constant
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct constant *this)
{
        switch (this->type) {
        case CHAR_TYPE:
                return create_quad_address_const_char(this->val.cval);
        case FLOAT_TYPE:
                return create_quad_address_const_float(this->val.fval);
        case INT_TYPE:
                return create_quad_address_const_int(this->val.ival);
        default:
                assert(0);  /* Invalid enum value. */
        }
}

struct quad_address *parse_instructions_function_call
(struct symbol_table *global, struct symbol_table *local, struct instructions *instructions, struct function_call *this)
{
        struct quad_address *expr, *stored_expr, *result;
        int n_args = 0;
        struct function_arg_list *function_arg_list = this->function_arg_list;
        while (function_arg_list) {
                n_args++;
                expr = parse_instructions_expr(global, local, instructions, function_arg_list->expr);
                stored_expr = get_next_temp(instructions);
                add_instruction(instructions, create_quad_copy(expr, stored_expr));
                add_instruction(instructions, create_quad_procedure_param(stored_expr));
                function_arg_list = function_arg_list->function_arg_list;
        }
        result = get_next_temp(instructions);
        add_instruction(instructions, create_quad_procedure_call(result, this->id, n_args));
        return result;
}
