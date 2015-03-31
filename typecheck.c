#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"
#include "typecheck.h"

void type_error
(struct pos pos, const char *fmt, ...)
{
        va_list argptr;
        fprintf(stderr, "%d:%d: type error: ", pos.line, pos.column);
        va_start(argptr, fmt);
        vfprintf(stderr, fmt, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
}

void type_check_program
(struct symbol_table *global, struct program *this)
{
        if (!this) return;
        type_check_function_def_list(global, this->function_def_list);
}

void type_check_function_def_list
(struct symbol_table *global, struct function_def_list *this)
{
        if (!this) return;
        type_check_function_def(global, this->function_def);
        type_check_function_def_list(global, this->function_def_list);
}

void type_check_function_def
(struct symbol_table *global, struct function_def *this)
{
        struct symbol_table *local;
        if (!this) return;
        local = get_symbol(global, this->id)->scoped_table;
        type_check_function_body(global, local, this->function_body);
}

void type_check_function_body
(struct symbol_table *global, struct symbol_table *local, struct function_body *this)
{
        if (!this) return;
        type_check_stmt_list(global, local, this->stmt_list);
        type_check_return_stmt(global, local, this->return_stmt);
}

void type_check_stmt_list
(struct symbol_table *global, struct symbol_table *local, struct stmt_list *this)
{
        if (!this) return;
        type_check_stmt(global, local, this->stmt);
        type_check_stmt_list(global, local, this->stmt_list);
}

void type_check_stmt
(struct symbol_table *global, struct symbol_table *local, struct stmt *this)
{
        if (!this) return;
        switch (this->type) {
        case EXPR_STMT:
                type_check_expr_stmt(global, local, this->val.expr_stmt);
                break;
        case COMPOUND_STMT:
                type_check_compound_stmt(global, local, this->val.compound_stmt);
                break;
        case SELECT_STMT:
                type_check_select_stmt(global, local, this->val.select_stmt);
                break;
        case ITER_STMT:
                type_check_iter_stmt(global, local, this->val.iter_stmt);
                break;
        case RETURN_STMT:
                type_check_return_stmt(global, local, this->val.return_stmt);
                break;
        }
}

void type_check_expr_stmt
(struct symbol_table *global, struct symbol_table *local, struct expr_stmt *this)
{
        if (!this) return;
        translate_expr(global, local, this->expr);
}

void type_check_compound_stmt
(struct symbol_table *global, struct symbol_table *local, struct compound_stmt *this)
{
        if (!this) return;
        type_check_stmt_list(global, local, this->stmt_list);
}

void type_check_select_stmt
(struct symbol_table *global, struct symbol_table *local, struct select_stmt *this)
{
        if (!this) return;
        translate_expr(global, local, this->cond);
        type_check_stmt(global, local, this->stmt_if_true);
        type_check_stmt(global, local, this->stmt_if_false);
}

void type_check_iter_stmt
(struct symbol_table *global, struct symbol_table *local, struct iter_stmt *this)
{
        if (!this) return;
        translate_expr(global, local, this->init);
        translate_expr(global, local, this->cond);
        translate_expr(global, local, this->after);
        type_check_stmt(global, local, this->body);
}

void type_check_return_stmt
(struct symbol_table *global, struct symbol_table *local, struct return_stmt *this)
{
        if (!this) return;
        translate_expr(global, local, this->expr);
}

struct symbol *translate_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        switch (this->type) {
        case ASSIGN_EXPR:
                return translate_assign_expr(global, local, this);
        case LOGICAL_OR_EXPR:
                return translate_logical_or_expr(global, local, this);
        case LOGICAL_AND_EXPR:
                return translate_logical_and_expr(global, local, this);
        case EQUALITY_EXPR:
                return translate_equality_expr(global, local, this);
        case RELATIONAL_EXPR:
                return translate_relational_expr(global, local, this);
        case ADDITIVE_EXPR:
                return translate_additive_expr(global, local, this);
        case MULTIPLICATIVE_EXPR:
                return translate_multiplicative_expr(global, local, this);
        case UNARY_EXPR:
                return translate_unary_expr(global, local, this);
        case POSTFIX_EXPR:
                return translate_postfix_expr(global, local, this);
        }
}

struct symbol *translate_assign_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *assignee, *assignment;
        if (!this) return NULL;
        assignee = translate_var(global, local, this->val.assign.assignee);
        assignment = translate_expr(global, local, this->val.assign.assignment);
        if (assignee->type != assignment->type) {
                type_error(this->pos, "assignment from incompatible type");
        }
        return assignee;
}

struct symbol *translate_logical_or_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!primary || primary->type != SYMBOL_BASIC || primary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression (requires int)");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC || secondary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.secondary->pos, "invalid operand to expression (requires int)");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_logical_and_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!primary || primary->type != SYMBOL_BASIC || primary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression (requires int)");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC || secondary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.secondary->pos, "invalid operand to expression (requires int)");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_equality_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!primary || primary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.secondary->pos, "invalid operand to expression");
        }
        if (primary->val.basic_type != secondary->val.basic_type) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_relational_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!primary || primary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.secondary->pos, "invalid operand to expression");
        }
        if (primary->val.basic_type != secondary->val.basic_type) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_additive_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.binary_op.primary);
        secondary = translate_expr(global, local, this->val.binary_op.secondary);
        if (!primary || primary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.primary->pos, "invalid operand to expression");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.secondary->pos, "invalid operand to expression");
        }
        if (primary->val.basic_type != secondary->val.basic_type) {
                type_error(this->val.binary_op.primary->pos, "comparison of incompatible types");
        }
        symbol = create_symbol_basic(primary->val.basic_type);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_multiplicative_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        if (!this) return NULL;
        primary = translate_expr(global, local, this->val.binary_op.primary);
        secondary = translate_expr(global, local, this->val.binary_op.secondary);
        if (!primary || primary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.primary->pos, "invalid operand to expression");
        }
        if (!secondary || secondary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.secondary->pos, "invalid operand to expression");
        }
        if (primary->val.basic_type != secondary->val.basic_type) {
                type_error(this->val.binary_op.primary->pos, "comparison of incompatible types");
        }
        symbol = create_symbol_basic(primary->val.basic_type);
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_unary_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *symbol;
        if (!this) return NULL;
        switch (this->subtype.unary_expr_subtype) {
        case UNARY_EXPR_SIZEOF_BASIC:
                symbol = create_symbol_basic(INT_TYPE);
                break;
        case UNARY_EXPR_SIZEOF_UNARY:
        case UNARY_EXPR_NOT_UNARY:
        case UNARY_EXPR_POSITIVE:
        case UNARY_EXPR_NEGATIVE:
        case UNARY_EXPR_PRE_INCREMENT:
        case UNARY_EXPR_PRE_DECREMENT:
                symbol = translate_expr(global, local, this->val.unary_op.expr);
                if (!symbol || symbol->type != SYMBOL_BASIC) {
                        type_error(this->val.unary_op.expr->pos, "invalid operand to expression");
                }
                symbol = create_symbol_basic(symbol->val.basic_type);
                break;
        }
        add_temp_symbol(local, symbol);
        return symbol;
}

struct symbol *translate_postfix_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *symbol;
        if (!this) return NULL;
        switch (this->subtype.postfix_expr_subtype) {
        case POSTFIX_EXPR_VAR:
                symbol = translate_var(global, local, this->val.postfix_op.var);
                break;
        case POSTFIX_EXPR_CONSTANT:
                symbol = translate_constant(local, this->val.postfix_op.constant);
                break;
        case POSTFIX_EXPR_POST_INCREMENT:
        case POSTFIX_EXPR_POST_DECREMENT:
        case POSTFIX_EXPR_ENCLOSED:
                symbol = translate_expr(global, local, this->val.postfix_op.expr);
                break;
        case POSTFIX_EXPR_FUNCTION_CALL:
                symbol = translate_function_call(global, local, this->val.postfix_op.function_call);
                break;
        }
        return symbol;
}

struct symbol *translate_function_call
(struct symbol_table *global, struct symbol_table *local, struct function_call *this)
{
        struct symbol *symbol, *parent, *arg_symbol, *param_symbol;
        struct function_arg_list *function_arg_list;
        struct function_param_list *function_param_list;
        if (!this) return NULL;
        parent = get_symbol(global, this->id);
        if (!parent) {
                type_error(this->pos, "use of undeclared function '%s'", this->id);
        }
        function_arg_list = this->function_arg_list;
        function_param_list = parent->val.function_def->function_param_list;
        while (function_arg_list || function_param_list) {
                if (!function_arg_list) {
                        type_error(this->pos, "too few arguments to function call");
                }
                if (!function_param_list) {
                        type_error(this->pos, "too many arguments to function call");
                }
                arg_symbol = translate_expr(global, local, function_arg_list->expr);
                param_symbol = get_symbol(parent->scoped_table, function_param_list->var_decl->id);
                if (arg_symbol->type != param_symbol->type) {
                        type_error(function_arg_list->pos, "passing argument to incompatible type");
                }
                function_arg_list = function_arg_list->function_arg_list;
                function_param_list = function_param_list->function_param_list;
        }
        if (parent->val.function_def->type == VOID_FUNCTION) {
                return NULL;
        }
        symbol = create_symbol_basic(parent->val.function_def->type_specifier);
        add_temp_symbol(global, symbol);
        return symbol;
}

struct symbol *translate_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        if (!this) return NULL;
        switch (this->type) {
        case IDENTIFIER:
                return translate_identifier_var(global, local, this);
                break;
        case FIELD:
                return translate_field_var(global, local, this);
                break;
        case SUBSCRIPT:
                return translate_subscript_var(global, local, this);
                break;
        }
}

struct symbol *translate_identifier_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *symbol;
        if (!this) return NULL;
        if ((symbol = get_symbol(local, this->val.id))) {
                return symbol;
        } else {
                symbol = get_symbol(global, this->val.id);
        }
        if (!symbol) {
                type_error(this->pos, "use of undeclared identifier '%s'", this->val.id);
        }
        return symbol;
}

struct symbol *translate_field_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *symbol, *parent;
        if (!this) return NULL;
        parent = translate_var(global, local, this->val.field.var);
        if (!parent || parent->type != SYMBOL_STRUCT) {
                type_error(this->pos, "member reference is not a structure");
        }
        symbol = get_symbol(parent->scoped_table, this->val.field.id);
        if (!symbol) {
                type_error(this->pos, "no member '%s' in struct", this->val.field.id);
        }
        return symbol;
}

struct symbol *translate_subscript_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *subscript, *parent;
        if (!this) return NULL;
        subscript = translate_expr(global, local, this->val.subscript.expr);
        parent = translate_var(global, local, this->val.field.var);
        if (!parent || parent->type != SYMBOL_ARRAY) {
                type_error(this->pos, "subscripted value is not an array");
        }
        if (!subscript || subscript->type != SYMBOL_BASIC || subscript->val.basic_type != INT_TYPE) {
                type_error(this->pos, "array subscript is not an integer");
        }
        return parent->val.array.symbol;
}

struct symbol *translate_constant
(struct symbol_table *local, struct constant *this)
{
        struct symbol *symbol;
        if (!this) return NULL;
        symbol = create_symbol_basic(this->type);
        add_temp_symbol(local, symbol);
        return symbol;
}
