#include <stdarg.h>
#include <stdio.h>
#include "astprint.h"

#define INDENT_CHAR '-'
#define INDENT_SIZE 2

void print_ast
(FILE *out, struct program *program)
{
        print_program(out, program, 0);
}

void print_at_depth
(FILE *out, int depth, const char *fmt, ...)
{
        int i, j;
        for (i = 0; i < depth; i++) {
                for (j = 0; j < INDENT_SIZE; j++) {
                        fprintf(out, "%c", INDENT_CHAR);
                }
        }
        va_list argptr;
        va_start(argptr, fmt);
        vfprintf(out, fmt, argptr);
        va_end(argptr);
        fprintf(out, "\n");
}

void print_basic_type
(FILE *out, enum basic_type type, int depth)
{
        switch (type) {
        case CHAR_TYPE:
                print_at_depth(out, depth, "type: CHAR");
                break;
        case FLOAT_TYPE:
                print_at_depth(out, depth, "type: FLOAT");
                break;
        case INT_TYPE:
                print_at_depth(out, depth, "type: INT");
                break;
        }
}

void print_program
(FILE *out, struct program *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "program");
        print_at_depth(out, depth+1, "type_decl_list");
        print_type_decl_list(out, this->type_decl_list, depth+1);
        print_at_depth(out, depth+1, "var_decl_stmt_list");
        print_var_decl_stmt_list(out, this->var_decl_stmt_list, depth+1);
        print_at_depth(out, depth+1, "function_def_list");
        print_function_def_list(out, this->function_def_list, depth+1);
}

void print_type_decl_list
(FILE *out, struct type_decl_list *this, int depth)
{
        if (!this) return;
        print_type_decl(out, this->type_decl, depth+1);
        print_type_decl_list(out, this->type_decl_list, depth);
}

void print_type_decl
(FILE *out, struct type_decl *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "type_decl");
        print_var_decl(out, this->var_decl, depth+1);
}

void print_var_decl_stmt_list
(FILE *out, struct var_decl_stmt_list *this, int depth)
{
        if (!this) return;
        print_var_decl(out, this->var_decl, depth+1);
        print_var_decl_stmt_list(out, this->var_decl_stmt_list, depth);
}

void print_var_decl
(FILE *out, struct var_decl *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "var_decl: '%s'", this->id);
        switch (this->type) {
        case BASIC_VAR:
                print_basic_type(out, this->val.basic_var, depth+1);
                break;
        case STRUCT_VAR:
                print_struct_type(out, this->val.struct_var, depth+1);
                break;
        case TYPEDEF_VAR:
                print_at_depth(out, depth+1, "type_name: '%s'", this->val.typedef_id);
                break;
        }
        print_array_specifier(out, this->array_specifier, depth+1);
}

void print_struct_type
(FILE *out, struct struct_type *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "struct_type: '%s'", this->id);
        print_at_depth(out, depth+1, "var_decl_stmt_list");
        print_var_decl_stmt_list(out, this->var_decl_stmt_list, depth+1);
}

void print_array_specifier
(FILE *out, struct array_specifier *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "array_specifier");
        print_constant(out, this->constant, depth+1);
}

void print_function_def_list
(FILE *out, struct function_def_list *this, int depth)
{
        if (!this) return;
        print_function_def(out, this->function_def, depth+1);
        print_function_def_list(out, this->function_def_list, depth);
}

void print_function_def
(FILE *out, struct function_def *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "function_def: '%s'", this->id);
        switch (this->type) {
        case BASIC_TYPE_FUNCTION:
                print_basic_type(out, this->type_specifier, depth+1);
                break;
        case VOID_FUNCTION:
                print_at_depth(out, depth+1, "type: VOID");
                break;
        }
        print_at_depth(out, depth+1, "function_param_list");
        print_function_param_list(out, this->function_param_list, depth+1);
        print_function_body(out, this->function_body, depth+1);
}

void print_function_param_list
(FILE *out, struct function_param_list *this, int depth)
{
        if (!this) return;
        print_var_decl(out, this->var_decl, depth+1);
        print_function_param_list(out, this->function_param_list, depth);
}

void print_function_body
(FILE *out, struct function_body *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "function_body");
        print_at_depth(out, depth+1, "var_decl_stmt_list");
        print_var_decl_stmt_list(out, this->var_decl_stmt_list, depth+1);
        print_at_depth(out, depth+1, "stmt_list");
        print_stmt_list(out, this->stmt_list, depth+1);
        print_return_stmt(out, this->return_stmt, depth+1);
}

void print_function_call
(FILE *out, struct function_call *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "function_call: '%s'", this->id);
        print_at_depth(out, depth+1, "function_arg_list");
        print_function_arg_list(out, this->function_arg_list, depth+1);
}

void print_function_arg_list
(FILE *out, struct function_arg_list *this, int depth)
{
        if (!this) return;
        print_expr(out, this->expr, depth+1);
        print_function_arg_list(out, this->function_arg_list, depth);
}

void print_stmt_list
(FILE *out, struct stmt_list *this, int depth)
{
        if (!this) return;
        print_stmt(out, this->stmt, depth+1);
        print_stmt_list(out, this->stmt_list, depth);
}

void print_stmt
(FILE *out, struct stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "stmt");
        switch (this->type) {
        case EXPR_STMT:
                print_expr_stmt(out, this->val.expr_stmt, depth+1);
                break;
        case COMPOUND_STMT:
                print_compound_stmt(out, this->val.compound_stmt, depth+1);
                break;
        case SELECT_STMT:
                print_select_stmt(out, this->val.select_stmt, depth+1);
                break;
        case ITER_STMT:
                print_iter_stmt(out, this->val.iter_stmt, depth+1);
                break;
        case RETURN_STMT:
                print_return_stmt(out, this->val.return_stmt, depth+1);
                break;
        }
}

void print_expr_stmt
(FILE *out, struct expr_stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "expr_stmt");
        print_expr(out, this->expr, depth+1);
}

void print_compound_stmt
(FILE *out, struct compound_stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "compound_stmt");
        print_at_depth(out, depth+1, "stmt_list");
        print_stmt_list(out, this->stmt_list, depth+1);
}

void print_select_stmt
(FILE *out, struct select_stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "select_stmt");
        print_expr(out, this->cond, depth+1);
        switch (this->type) {
        case IF_THEN:
                print_at_depth(out, depth+1, "type: IF_THEN");
                print_stmt(out, this->stmt_if_true, depth+1);
                break;
        case IF_THEN_ELSE:
                print_at_depth(out, depth+1, "type: IF_THEN_ELSE");
                print_stmt(out, this->stmt_if_true, depth+1);
                print_stmt(out, this->stmt_if_false, depth+1);
                break;
        }
}

void print_iter_stmt
(FILE *out, struct iter_stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "iter_stmt");
        switch (this->type) {
        case WHILE_EXPR:
                print_at_depth(out, depth+1, "type: WHILE_EXPR");
                print_expr(out, this->cond, depth+1);
        case FOR_INIT_COND_AFTER:
                print_at_depth(out, depth+1, "type: FOR_INIT_COND_AFTER");
                print_expr(out, this->init, depth+1);
                print_expr(out, this->cond, depth+1);
                print_expr(out, this->after, depth+1);
        case FOR_INIT_AFTER:
                print_at_depth(out, depth+1, "type: FOR_INIT_AFTER");
                print_expr(out, this->init, depth+1);
                print_expr(out, this->after, depth+1);
        case FOR_INIT_COND:
                print_at_depth(out, depth+1, "type: FOR_INIT_COND");
                print_expr(out, this->init, depth+1);
                print_expr(out, this->cond, depth+1);
                print_expr(out, this->after, depth+1);
        case FOR_COND_AFTER:
                print_at_depth(out, depth+1, "type: FOR_COND_AFTER");
                print_expr(out, this->cond, depth+1);
                print_expr(out, this->after, depth+1);
        case FOR_AFTER:
                print_at_depth(out, depth+1, "type: FOR_AFTER");
                print_expr(out, this->after, depth+1);
        case FOR_INIT:
                print_at_depth(out, depth+1, "type: FOR_INIT");
                print_expr(out, this->init, depth+1);
        case FOR_EMPTY:
                print_at_depth(out, depth+1, "type: FOR_EMPTY");
                break;
        }
        print_stmt(out, this->body, depth+1);
}

void print_return_stmt
(FILE *out, struct return_stmt *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "return_stmt");
        print_expr(out, this->expr, depth+1);
}

void print_expr
(FILE *out, struct expr *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "expr");
        switch (this->type) {
        case ASSIGN_EXPR:
                print_at_depth(out, depth+1, "type: ASSIGN_EXPR");
                print_var(out, this->val.assign.assignee, depth+1);
                print_expr(out, this->val.assign.assignment, depth+1);
                break;
        case LOGICAL_OR_EXPR:
                print_at_depth(out, depth+1, "type: LOGICAL_OR_EXPR");
                print_expr(out, this->val.relation.primary, depth+1);
                print_expr(out, this->val.relation.secondary, depth+1);
                break;
        case LOGICAL_AND_EXPR:
                print_at_depth(out, depth+1, "type: LOGICAL_AND_EXPR");
                print_expr(out, this->val.relation.primary, depth+1);
                print_expr(out, this->val.relation.secondary, depth+1);
                break;
        case EQUALITY_EXPR:
                print_at_depth(out, depth+1, "type: EQUALITY_EXPR");
                switch (this->subtype.equality_expr_subtype) {
                case EQUALITY_EXPR_EQUAL:
                        print_at_depth(out, depth+1, "subtype: EQUALITY_EXPR_EQUAL");
                        break;
                case EQUALITY_EXPR_NOT_EQUAL:
                        print_at_depth(out, depth+1, "subtype: EQUALITY_EXPR_NOT_EQUAL");
                        break;
                }
                print_expr(out, this->val.relation.primary, depth+1);
                print_expr(out, this->val.relation.secondary, depth+1);
                break;
        case RELATIONAL_EXPR:
                print_at_depth(out, depth+1, "type: RELATIONAL_EXPR");
                switch (this->subtype.relational_expr_subtype) {
                case RELATIONAL_EXPR_LESS_THAN:
                        print_at_depth(out, depth+1, "subtype: RELATIONAL_EXPR_LESS_THAN");
                        break;
                case RELATIONAL_EXPR_LESS_THAN_OR_EQUAL:
                        print_at_depth(out, depth+1, "subtype: RELATIONAL_EXPR_LESS_THAN_OR_EQUAL");
                        break;
                case RELATIONAL_EXPR_GREATER_THAN: 
                        print_at_depth(out, depth+1, "subtype: RELATION_EXPR_GREATER_THAN");
                        break;
                case RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL:
                        print_at_depth(out, depth+1, "subtype: RELATIONAL_EXPR_GREATER_THAN_OR_EQUAL");
                        break;
                }
                print_expr(out, this->val.relation.primary, depth+1);
                print_expr(out, this->val.relation.secondary, depth+1);
                break;
        case ADDITIVE_EXPR:
                print_at_depth(out, depth+1, "type: ADDITIVE_EXPR");
                switch (this->subtype.additive_expr_subtype) {
                case ADDITIVE_EXPR_ADD:
                        print_at_depth(out, depth+1, "subtype: ADDITIVE_EXPR_ADD");
                      break;
                case ADDITIVE_EXPR_SUBTRACT:
                      print_at_depth(out, depth+1, "subtype: ADDITIVE_EXPR_SUBTRACT");
                      break;
                }
                print_expr(out, this->val.binary_op.primary, depth+1);
                print_expr(out, this->val.binary_op.secondary, depth+1);
                break;
        case MULTIPLICATIVE_EXPR:
                print_at_depth(out, depth+1, "type: MULTIPLICATIVE_EXPR");
                switch (this->subtype.multiplicative_expr_subtype) {
                case MULTIPLICATIVE_EXPR_MULTIPLY:
                        print_at_depth(out, depth+1, "subtype: MULTIPLICATIVE_EXPR_MULTIPLY");
                        break;
                case MULTIPLICATIVE_EXPR_DIVIDE:
                        print_at_depth(out, depth+1, "subtype: MULTIPLICATIVE_EXPR_DIVIDE");
                        break;
                case MULTIPLICATIVE_EXPR_MODULO:
                        print_at_depth(out, depth+1, "subtype: MULTIPLICATIVE_EXPR_MODULO");
                        break;
                }
                print_expr(out, this->val.binary_op.primary, depth+1);
                print_expr(out, this->val.binary_op.secondary, depth+1);
                break;
        case UNARY_EXPR:
                print_at_depth(out, depth+1, "type: UNARY_EXPR");
                switch (this->subtype.unary_expr_subtype) {
                case UNARY_EXPR_SIZEOF_UNARY:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_SIZEOF_UNARY");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                case UNARY_EXPR_SIZEOF_BASIC:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_SIZEOF_BASIC");
                        print_basic_type(out, this->val.unary_op.type, depth+1);
                        break;
                case UNARY_EXPR_NOT_UNARY:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_NOT_UNARY");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                case UNARY_EXPR_POSITIVE:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_POSITIVE");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                case UNARY_EXPR_NEGATIVE:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_NEGATIVE");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                case UNARY_EXPR_PRE_INCREMENT:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_PRE_INCREMENT");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                case UNARY_EXPR_PRE_DECREMENT:
                        print_at_depth(out, depth+1, "subtype: UNARY_EXPR_PRE_DECREMENT");
                        print_expr(out, this->val.unary_op.expr, depth+1);
                        break;
                }
                break;
        case POSTFIX_EXPR:
                print_at_depth(out, depth+1, "type: POSTFIX_EXPR");
                switch (this->subtype.postfix_expr_subtype) {
                case POSTFIX_EXPR_VAR:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_VAR");
                        print_var(out, this->val.postfix_op.var, depth+1);
                        break;
                case POSTFIX_EXPR_CONSTANT:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_CONSTANT");
                        print_constant(out, this->val.postfix_op.constant, depth+1);
                        break;
                case POSTFIX_EXPR_POST_INCREMENT:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_POST_INCREMENT");
                        print_expr(out, this->val.postfix_op.expr, depth+1);
                        break;
                case POSTFIX_EXPR_POST_DECREMENT:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_POST_DECREMENT");
                        print_expr(out, this->val.postfix_op.expr, depth+1);
                        break;
                case POSTFIX_EXPR_ENCLOSED:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_ENCLOSED");
                        print_expr(out, this->val.postfix_op.expr, depth+1);
                        break;
                case POSTFIX_EXPR_FUNCTION_CALL:
                        print_at_depth(out, depth+1, "subtype: POSTFIX_EXPR_FUNCTION_CALL");
                        print_function_call(out, this->val.postfix_op.function_call, depth+1);
                        break;
                }
                break;
        }
}

void print_var
(FILE *out, struct var *this, int depth)
{
        if (!this) return;
        print_at_depth(out, depth, "var");
        switch (this->type) {
        case IDENTIFIER:
                print_at_depth(out, depth+1, "type: IDENTIFIER: '%s'", this->val.id);
                break;
        case FIELD:
                print_at_depth(out, depth+1, "type: FIELD: '%s'", this->val.field.id);
                print_var(out, this->val.field.var, depth+1);
                break;
        case SUBSCRIPT:
                print_at_depth(out, depth+1, "type: SUBSCRIPT");
                print_var(out, this->val.subscript.var, depth+1);
                print_expr(out, this->val.subscript.expr, depth+1);
                break;
        }
}

void print_constant
(FILE *out, struct constant *this, int depth)
{
        if (!this) return;
        switch (this->type) {
        case CHAR_TYPE:
                print_at_depth(out, depth, "constant: '%c'", this->val.cval);
                break;
        case FLOAT_TYPE:
                print_at_depth(out, depth, "constant: %f", this->val.fval);
                break;
        case INT_TYPE:
                print_at_depth(out, depth, "constant: %d", this->val.ival);
                break;
        }
}

