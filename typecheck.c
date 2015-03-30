#include <stddef.h>
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
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_logical_or_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_logical_and_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_equality_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_relational_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_additive_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_multiplicative_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_unary_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}

struct symbol *translate_postfix_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        if (!this) return NULL;
        return NULL;
}
