#include <stdlib.h>
#include "ast.h"

struct program_t *create_program
(struct type_decl_list_t *type_decl_list, struct var_decl_stmt_list_t *var_decl_stmt_list, struct function_def_list_t *function_def_list)
{ 
        struct program_t *this = malloc(sizeof(struct program_t));
        this->type_decl_list = type_decl_list;
        this->var_decl_stmt_list = var_decl_stmt_list;
        this->function_def_list = function_def_list;
        return this;
}

struct type_decl_list_t *create_type_decl_list
(struct type_decl_t *type_decl, struct type_decl_list_t *type_decl_list)
{
        struct type_decl_list_t *this = malloc(sizeof(struct type_decl_list_t));
        this->type_decl = type_decl;
        this->type_decl_list = type_decl_list;
        return this;
}

struct type_decl_t *create_type_decl
(struct var_decl_t *var_decl)
{
        struct type_decl_t *this = malloc(sizeof(struct type_decl_t));
        this->var_decl = var_decl;
        return this;
}

struct var_decl_stmt_list_t *create_var_decl_stmt_list
(struct var_decl_t *var_decl, struct var_decl_stmt_list_t *var_decl_stmt_list)
{ 
        struct var_decl_stmt_list_t *this = malloc(sizeof(struct var_decl_stmt_list_t));
        this->var_decl = var_decl;
        this->var_decl_stmt_list = var_decl_stmt_list;
        return this;
}

struct var_decl_t *create_var_decl_basic
(enum basic_type type, char *id, struct array_specifier_t *array_specifier)
{ 
        struct var_decl_t *this = malloc(sizeof(struct var_decl_t));
        this->type = BASIC_VAR;
        this->val.basic_var = type;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
} 

struct var_decl_t *create_var_decl_struct
(struct struct_type_t *struct_type, char *id, struct array_specifier_t *array_specifier)
{ 
        struct var_decl_t *this = malloc(sizeof(struct var_decl_t));
        this->type = STRUCT_VAR;
        this->val.struct_var = struct_type;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
} 

struct var_decl_t *create_var_decl_typedef
(char *typedef_id, char *id, struct array_specifier_t *array_specifier)
{
        struct var_decl_t *this = malloc(sizeof(struct var_decl_t));
        this->type = TYPEDEF_VAR;
        this->val.typedef_id = typedef_id;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
}

struct struct_type_t *create_struct_type
(struct var_decl_stmt_list_t *var_decl_stmt_list)
{ 
        struct struct_type_t *this = malloc(sizeof(struct struct_type_t));
        this->var_decl_stmt_list = var_decl_stmt_list;
        return this;
}

struct array_specifier_t *create_array_specifier
(cflat_int size, struct array_specifier_t *array_specifier)
{
        struct array_specifier_t *this = malloc(sizeof(struct array_specifier_t));
        this->size = size;
        this->array_specifier = array_specifier;
        return this;
}

struct function_def_list_t *create_function_def_list
(struct function_def_t *function_def, struct function_def_list_t *function_def_list)
{
        struct function_def_list_t *this = malloc(sizeof(struct function_def_list_t));
        this->function_def = function_def;
        this->function_def_list = function_def_list;
        return this;
}

struct function_def_t *create_basic_function_def
(enum basic_type type, char *id, struct function_param_list_t *function_param_list, struct function_body_t *function_body)
{
        struct function_def_t *this = malloc(sizeof(struct function_def_t));
        this->type = BASIC_TYPE_FUNCTION;
        this->type_specifier = type;
        this->id = id;
        this->function_param_list = function_param_list;
        this->function_body = function_body;
        return this;
}

struct function_def_t *create_void_function_def
(char *id, struct function_param_list_t *function_param_list, struct function_body_t *function_body)
{
        struct function_def_t *this = malloc(sizeof(struct function_def_t));
        this->type = VOID_FUNCTION;
        this->id = id;
        this->function_param_list = function_param_list;
        this->function_body = function_body;
        return this;
}

struct function_param_list_t *create_function_param_list
(struct var_decl_t *var_decl, struct function_param_list_t *function_param_list)
{
        struct function_param_list_t *this = malloc(sizeof(struct function_param_list_t));
        this->var_decl = var_decl;
        this->function_param_list = function_param_list;
        return this;
}

struct function_body_t *create_function_body
(struct var_decl_stmt_list_t *var_decl_stmt_list, struct stmt_list_t *stmt_list, struct return_stmt_t *return_stmt)
{
        struct function_body_t *this = malloc(sizeof(struct function_body_t));
        this->var_decl_stmt_list = var_decl_stmt_list;
        this->stmt_list = stmt_list;
        this->return_stmt = return_stmt;
        return this;
}

struct function_call_t *create_function_call
(char *id, struct function_arg_list_t *function_arg_list)
{
        struct function_call_t *this = malloc(sizeof(struct function_call_t));
        this->id = id;
        this->function_arg_list = function_arg_list;
        return this;
}

struct function_arg_list_t *create_function_arg_list
(struct expr_t *expr, struct function_arg_list_t *function_arg_list)
{
        struct function_arg_list_t *this = malloc(sizeof(struct function_arg_list_t));
        this->expr = expr;
        this->function_arg_list = function_arg_list;
        return this;
}

struct stmt_list_t *create_stmt_list
(struct stmt_t *stmt, struct stmt_list_t *stmt_list)
{
        struct stmt_list_t *this = malloc(sizeof(struct stmt_list_t));
        this->stmt = stmt;
        this->stmt_list = stmt_list;
        return this;
}

struct stmt_t *create_stmt
(enum stmt_type type, void *stmt)
{
        struct stmt_t *this = malloc(sizeof(struct stmt_t));
        this->type = type;
        switch (type) {
        case EXPR_STMT:
                this->val.expr_stmt = stmt;
                break;
        case COMPOUND_STMT:
                this->val.compound_stmt = stmt;
                break;
        case SELECT_STMT:
                this->val.select_stmt = stmt;
                break;
        case ITER_STMT:
                this->val.iter_stmt = stmt;
                break;
        case RETURN_STMT:
                this->val.return_stmt = stmt;
                break;
        }
        return this;
}

struct expr_stmt_t *create_expr_stmt
(struct expr_t *expr)
{
        struct expr_stmt_t *this = malloc(sizeof(struct expr_stmt_t));
        this->expr = expr;
        return this;
}

struct compound_stmt_t *create_compound_stmt
(struct stmt_list_t *stmt_list)
{
        struct compound_stmt_t *this = malloc(sizeof(struct compound_stmt_t));
        this->stmt_list = stmt_list;
        return this;
}

struct select_stmt_t *create_select_stmt
(struct expr_t *expr, struct stmt_t *stmt_if_true, struct stmt_t *stmt_if_false)
{
        struct select_stmt_t *this = malloc(sizeof(struct select_stmt_t));
        this->type = (stmt_if_false == NULL) ? IF_THEN : IF_THEN_ELSE;
        this->cond = expr;
        this->stmt_if_true = stmt_if_true;
        this->stmt_if_false = stmt_if_false;
        return this;
}

struct iter_stmt_t *create_iter_stmt
(struct expr_t *init, struct expr_t *cond, struct expr_t *after, struct stmt_t *body)
{
        struct iter_stmt_t *this = malloc(sizeof(struct iter_stmt_t));
        if (init == NULL && cond == NULL && after == NULL) {
                this->type = FOR_EMPTY;
        } else if (init == NULL && cond == NULL) {
                this->type = FOR_AFTER;
        } else if (init == NULL && after == NULL) {
                this->type = WHILE_EXPR;
        } else if (cond == NULL && after == NULL) {
                this->type = FOR_INIT;
        } else if (init == NULL) {
                this->type = FOR_COND_AFTER;
        } else if (cond == NULL) {
                this->type = FOR_INIT_AFTER;
        } else if (after == NULL) {
                this->type = FOR_INIT_COND;
        } else {
                this->type = FOR_INIT_COND_AFTER;
        }
        this->init = init;
        this->cond = cond;
        this->after = after;
        this->body = body;
        return this;
}

struct return_stmt_t *create_return_stmt
(struct expr_t *expr)
{
        struct return_stmt_t *this = malloc(sizeof(struct return_stmt_t));
        this->expr = expr;
        return this;
}

struct expr_t *create_assign_expr
(struct var_t *var, struct expr_t *expr)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = ASSIGN_EXPR;
        this->val.assign.assignee = var;
        this->val.assign.assignment = expr;
        return this;
}

struct expr_t *create_logical_or_expr
(struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = LOGICAL_OR_EXPR;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr_t *create_logical_and_expr
(struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = LOGICAL_AND_EXPR;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr_t *create_equality_expr
(enum expr_subtype subtype, struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = EQUALITY_EXPR;
        this->subtype = subtype;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr_t *create_relational_expr
(enum expr_subtype subtype, struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = RELATIONAL_EXPR;
        this->subtype = subtype;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr_t *create_additive_expr
(enum expr_subtype subtype, struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = ADDITIVE_EXPR;
        this->subtype = subtype;
        this->val.binary_op.primary = primary;
        this->val.binary_op.secondary = secondary;
        return this;
}

struct expr_t *create_multiplicative_expr
(enum expr_subtype subtype, struct expr_t *primary, struct expr_t *secondary)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = MULTIPLICATIVE_EXPR;
        this->subtype = subtype;
        this->val.binary_op.primary = primary;
        this->val.binary_op.secondary = secondary;
        return this;
}

struct expr_t *create_unary_expr_sizeof_basic
(enum basic_type type)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = UNARY_EXPR;
        this->subtype = UNARY_EXPR_SIZEOF_BASIC;
        this->val.unary_op.type = type;
        return this;
}

struct expr_t *create_unary_expr
(enum expr_subtype subtype, struct expr_t *expr)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = UNARY_EXPR;
        this->subtype = subtype;
        this->val.unary_op.expr = expr;
        return this;
}

struct expr_t *create_postfix_expr
(enum expr_subtype subtype, void *ptr)
{
        struct expr_t *this = malloc(sizeof(struct expr_t));
        this->type = POSTFIX_EXPR;
        this->subtype = subtype;
        this->val.postfix_op.var = ptr;
        return this;
}

struct var_t *create_var_identifier
(char *id)
{
        struct var_t *this = malloc(sizeof(struct var_t));
        this->type = IDENTIFIER;
        this->val.id = id;
        return this;
}

struct var_t *create_var_field
(struct var_t *var, char *id)
{
        struct var_t *this = malloc(sizeof(struct var_t));
        this->type = FIELD;
        this->val.field.var = var;
        this->val.field.id = id;
        return this;
}

struct var_t *create_var_subscript
(struct var_t *var, struct expr_t *expr)
{
        struct var_t *this = malloc(sizeof(struct var_t));
        this->type = SUBSCRIPT;
        this->val.subscript.var = var;
        this->val.subscript.expr = expr;
        return this;
}

struct constant_t *create_constant_char
(cflat_char val)
{
        struct constant_t *this = malloc(sizeof(struct constant_t));
        this->type = CHAR_TYPE;
        this->val.cval = val;
        return this;
}

struct constant_t *create_constant_float
(cflat_float val)
{
        struct constant_t *this = malloc(sizeof(struct constant_t));
        this->type = FLOAT_TYPE;
        this->val.fval = val;
        return this;
}

struct constant_t *create_constant_int
(cflat_int val)
{
        struct constant_t *this = malloc(sizeof(struct constant_t));
        this->type = INT_TYPE;
        this->val.ival = val;
        return this;
}
