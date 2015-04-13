/* typecheck.c
 * Typechecking of abstract syntax tree and symbol table.
 * by Ryan Wilson-Perkin
 * for CIS4650W15 Compilers: Cflat Assignment
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"
#include "typecheck.h"

void type_check_program
(struct symbol_table *global, struct program *this)
{
        if (!this) return;
        type_check_function_def_list(global, this->function_def_list);
}

void type_check_function_def_list
(struct symbol_table *global, struct function_def_list *this)
{
        struct symbol *main_function;
        if (!this) return;
        type_check_function_def(global, this->function_def);
        type_check_function_def_list(global, this->function_def_list);
        main_function = get_symbol(global, "main");
        if (!main_function || main_function->type != SYMBOL_FUNCTION) {
                type_error(this->pos, "missing 'main' function");
        }
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
        default:
                assert(0);  /* Invalid enum value. */
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

void type_check_function_arg_list
(struct symbol_table *global, struct symbol_table *local, struct symbol_table *fn_scoped, struct function_arg_list *function_arg_list, struct function_param_list *function_param_list)
{
        struct symbol *arg_symbol, *param_symbol;
        if (!function_arg_list && !function_param_list) return;
        if (!function_arg_list) {
                type_error(function_param_list->pos, "too few arguments to function call");
        }
        if (!function_param_list) {
                type_error(function_arg_list->pos, "too many arguments to function call");
        }

        arg_symbol = translate_expr(global, local, function_arg_list->expr);
        param_symbol = get_symbol(fn_scoped, function_param_list->var_decl->id);
        if (!symbol_equivalent(arg_symbol, param_symbol)) {
                type_error(function_arg_list->pos, "passing argument to incompatible type");
        }
        type_check_function_arg_list(global, local, fn_scoped, function_arg_list->function_arg_list, function_param_list->function_param_list);
}
