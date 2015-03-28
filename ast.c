#include <stdlib.h>
#include "ast.h"

struct program *create_program
(int line, int column, struct type_decl_list *type_decl_list, struct var_decl_stmt_list *var_decl_stmt_list, struct function_def_list *function_def_list)
{ 
        struct program *this = malloc(sizeof(struct program));
        this->pos.line = line;
        this->pos.column = column;
        this->type_decl_list = type_decl_list;
        this->var_decl_stmt_list = var_decl_stmt_list;
        this->function_def_list = function_def_list;
        return this;
}

struct type_decl_list *create_type_decl_list
(int line, int column, struct type_decl *type_decl, struct type_decl_list *type_decl_list)
{
        struct type_decl_list *this = malloc(sizeof(struct type_decl_list));
        this->pos.line = line;
        this->pos.column = column;
        this->type_decl = type_decl;
        this->type_decl_list = type_decl_list;
        return this;
}

struct type_decl *create_type_decl
(int line, int column, struct var_decl *var_decl)
{
        struct type_decl *this = malloc(sizeof(struct type_decl));
        this->pos.line = line;
        this->pos.column = column;
        this->var_decl = var_decl;
        return this;
}

struct var_decl_stmt_list *create_var_decl_stmt_list
(int line, int column, struct var_decl *var_decl, struct var_decl_stmt_list *var_decl_stmt_list)
{ 
        struct var_decl_stmt_list *this = malloc(sizeof(struct var_decl_stmt_list));
        this->pos.line = line;
        this->pos.column = column;
        this->var_decl = var_decl;
        this->var_decl_stmt_list = var_decl_stmt_list;
        return this;
}

struct var_decl *create_var_decl_basic
(int line, int column, enum basic_type type, char *id, struct array_specifier *array_specifier)
{ 
        struct var_decl *this = malloc(sizeof(struct var_decl));
        this->pos.line = line;
        this->pos.column = column;
        this->type = BASIC_VAR;
        this->val.basic_var = type;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
} 

struct var_decl *create_var_decl_struct
(int line, int column, struct struct_type *struct_type, char *id, struct array_specifier *array_specifier)
{ 
        struct var_decl *this = malloc(sizeof(struct var_decl));
        this->pos.line = line;
        this->pos.column = column;
        this->type = STRUCT_VAR;
        this->val.struct_var = struct_type;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
} 

struct var_decl *create_var_decl_typedef
(int line, int column, char *typedef_id, char *id, struct array_specifier *array_specifier)
{
        struct var_decl *this = malloc(sizeof(struct var_decl));
        this->pos.line = line;
        this->pos.column = column;
        this->type = TYPEDEF_VAR;
        this->val.typedef_id = typedef_id;
        this->id = id;
        this->array_specifier = array_specifier;
        return this;
}

struct struct_type *create_struct_type
(int line, int column, struct var_decl_stmt_list *var_decl_stmt_list)
{ 
        struct struct_type *this = malloc(sizeof(struct struct_type));
        this->pos.line = line;
        this->pos.column = column;
        this->var_decl_stmt_list = var_decl_stmt_list;
        return this;
}

struct array_specifier *create_array_specifier
(int line, int column, struct constant *constant)
{
        struct array_specifier *this = malloc(sizeof(struct array_specifier));
        this->pos.line = line;
        this->pos.column = column;
        this->constant = constant;
        return this;
}

struct function_def_list *create_function_def_list
(int line, int column, struct function_def *function_def, struct function_def_list *function_def_list)
{
        struct function_def_list *this = malloc(sizeof(struct function_def_list));
        this->pos.line = line;
        this->pos.column = column;
        this->function_def = function_def;
        this->function_def_list = function_def_list;
        return this;
}

struct function_def *create_basic_function_def
(int line, int column, enum basic_type type, char *id, struct function_param_list *function_param_list, struct function_body *function_body)
{
        struct function_def *this = malloc(sizeof(struct function_def));
        this->pos.line = line;
        this->pos.column = column;
        this->type = BASIC_TYPE_FUNCTION;
        this->type_specifier = type;
        this->id = id;
        this->function_param_list = function_param_list;
        this->function_body = function_body;
        return this;
}

struct function_def *create_void_function_def
(int line, int column, char *id, struct function_param_list *function_param_list, struct function_body *function_body)
{
        struct function_def *this = malloc(sizeof(struct function_def));
        this->pos.line = line;
        this->pos.column = column;
        this->type = VOID_FUNCTION;
        this->id = id;
        this->function_param_list = function_param_list;
        this->function_body = function_body;
        return this;
}

struct function_param_list *create_function_param_list
(int line, int column, struct var_decl *var_decl, struct function_param_list *function_param_list)
{
        struct function_param_list *this = malloc(sizeof(struct function_param_list));
        this->pos.line = line;
        this->pos.column = column;
        this->var_decl = var_decl;
        this->function_param_list = function_param_list;
        return this;
}

struct function_body *create_function_body
(int line, int column, struct var_decl_stmt_list *var_decl_stmt_list, struct stmt_list *stmt_list, struct return_stmt *return_stmt)
{
        struct function_body *this = malloc(sizeof(struct function_body));
        this->pos.line = line;
        this->pos.column = column;
        this->var_decl_stmt_list = var_decl_stmt_list;
        this->stmt_list = stmt_list;
        this->return_stmt = return_stmt;
        return this;
}

struct function_call *create_function_call
(int line, int column, char *id, struct function_arg_list *function_arg_list)
{
        struct function_call *this = malloc(sizeof(struct function_call));
        this->pos.line = line;
        this->pos.column = column;
        this->id = id;
        this->function_arg_list = function_arg_list;
        return this;
}

struct function_arg_list *create_function_arg_list
(int line, int column, struct expr *expr, struct function_arg_list *function_arg_list)
{
        struct function_arg_list *this = malloc(sizeof(struct function_arg_list));
        this->pos.line = line;
        this->pos.column = column;
        this->expr = expr;
        this->function_arg_list = function_arg_list;
        return this;
}

struct stmt_list *create_stmt_list
(int line, int column, struct stmt *stmt, struct stmt_list *stmt_list)
{
        struct stmt_list *this = malloc(sizeof(struct stmt_list));
        this->pos.line = line;
        this->pos.column = column;
        this->stmt = stmt;
        this->stmt_list = stmt_list;
        return this;
}

struct stmt *create_stmt
(int line, int column, enum stmt_type type, void *stmt)
{
        struct stmt *this = malloc(sizeof(struct stmt));
        this->pos.line = line;
        this->pos.column = column;
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

struct expr_stmt *create_expr_stmt
(int line, int column, struct expr *expr)
{
        struct expr_stmt *this = malloc(sizeof(struct expr_stmt));
        this->pos.line = line;
        this->pos.column = column;
        this->expr = expr;
        return this;
}

struct compound_stmt *create_compound_stmt
(int line, int column, struct stmt_list *stmt_list)
{
        struct compound_stmt *this = malloc(sizeof(struct compound_stmt));
        this->pos.line = line;
        this->pos.column = column;
        this->stmt_list = stmt_list;
        return this;
}

struct select_stmt *create_select_stmt
(int line, int column, struct expr *expr, struct stmt *stmt_if_true, struct stmt *stmt_if_false)
{
        struct select_stmt *this = malloc(sizeof(struct select_stmt));
        this->pos.line = line;
        this->pos.column = column;
        this->type = (stmt_if_false == NULL) ? IF_THEN : IF_THEN_ELSE;
        this->cond = expr;
        this->stmt_if_true = stmt_if_true;
        this->stmt_if_false = stmt_if_false;
        return this;
}

struct iter_stmt *create_iter_stmt
(int line, int column, struct expr *init, struct expr *cond, struct expr *after, struct stmt *body)
{
        struct iter_stmt *this = malloc(sizeof(struct iter_stmt));
        this->pos.line = line;
        this->pos.column = column;
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

struct return_stmt *create_return_stmt
(int line, int column, struct expr *expr)
{
        struct return_stmt *this = malloc(sizeof(struct return_stmt));
        this->pos.line = line;
        this->pos.column = column;
        this->expr = expr;
        return this;
}

struct expr *create_assign_expr
(int line, int column, struct var *var, struct expr *expr)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = ASSIGN_EXPR;
        this->val.assign.assignee = var;
        this->val.assign.assignment = expr;
        return this;
}

struct expr *create_logical_or_expr
(int line, int column, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = LOGICAL_OR_EXPR;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr *create_logical_and_expr
(int line, int column, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = LOGICAL_AND_EXPR;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr *create_equality_expr
(int line, int column, enum equality_expr_subtype subtype, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = EQUALITY_EXPR;
        this->subtype.equality_expr_subtype = subtype;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr *create_relational_expr
(int line, int column, enum relational_expr_subtype subtype, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = RELATIONAL_EXPR;
        this->subtype.relational_expr_subtype = subtype;
        this->val.relation.primary = primary;
        this->val.relation.secondary = secondary;
        return this;
}

struct expr *create_additive_expr
(int line, int column, enum additive_expr_subtype subtype, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = ADDITIVE_EXPR;
        this->subtype.additive_expr_subtype = subtype;
        this->val.binary_op.primary = primary;
        this->val.binary_op.secondary = secondary;
        return this;
}

struct expr *create_multiplicative_expr
(int line, int column, enum multiplicative_expr_subtype subtype, struct expr *primary, struct expr *secondary)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = MULTIPLICATIVE_EXPR;
        this->subtype.multiplicative_expr_subtype = subtype;
        this->val.binary_op.primary = primary;
        this->val.binary_op.secondary = secondary;
        return this;
}

struct expr *create_unary_expr_sizeof_basic
(int line, int column, enum basic_type type)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = UNARY_EXPR;
        this->subtype.unary_expr_subtype = UNARY_EXPR_SIZEOF_BASIC;
        this->val.unary_op.type = type;
        return this;
}

struct expr *create_unary_expr
(int line, int column, enum unary_expr_subtype subtype, struct expr *expr)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = UNARY_EXPR;
        this->subtype.unary_expr_subtype = subtype;
        this->val.unary_op.expr = expr;
        return this;
}

struct expr *create_postfix_expr
(int line, int column, enum postfix_expr_subtype subtype, struct expr *expr)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = POSTFIX_EXPR;
        this->subtype.postfix_expr_subtype = subtype;
        this->val.postfix_op.expr = expr;
        return this;
}

struct expr *create_postfix_expr_var
(int line, int column, struct var *var)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = POSTFIX_EXPR;
        this->subtype.postfix_expr_subtype = POSTFIX_EXPR_VAR;
        this->val.postfix_op.var = var;
        return this;
}

struct expr *create_postfix_expr_constant
(int line, int column, struct constant *constant)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = POSTFIX_EXPR;
        this->subtype.postfix_expr_subtype = POSTFIX_EXPR_CONSTANT;
        this->val.postfix_op.constant = constant;
        return this;
}

struct expr *create_postfix_expr_function_call
(int line, int column, struct function_call *function_call)
{
        struct expr *this = malloc(sizeof(struct expr));
        this->pos.line = line;
        this->pos.column = column;
        this->type = POSTFIX_EXPR;
        this->subtype.postfix_expr_subtype = POSTFIX_EXPR_FUNCTION_CALL;
        this->val.postfix_op.function_call = function_call;
        return this;
}

struct var *create_var_identifier
(int line, int column, char *id)
{
        struct var *this = malloc(sizeof(struct var));
        this->pos.line = line;
        this->pos.column = column;
        this->type = IDENTIFIER;
        this->val.id = id;
        return this;
}

struct var *create_var_field
(int line, int column, struct var *var, char *id)
{
        struct var *this = malloc(sizeof(struct var));
        this->pos.line = line;
        this->pos.column = column;
        this->type = FIELD;
        this->val.field.var = var;
        this->val.field.id = id;
        return this;
}

struct var *create_var_subscript
(int line, int column, struct var *var, struct expr *expr)
{
        struct var *this = malloc(sizeof(struct var));
        this->pos.line = line;
        this->pos.column = column;
        this->type = SUBSCRIPT;
        this->val.subscript.var = var;
        this->val.subscript.expr = expr;
        return this;
}

struct constant *create_constant_char
(int line, int column, cflat_char val)
{
        struct constant *this = malloc(sizeof(struct constant));
        this->pos.line = line;
        this->pos.column = column;
        this->type = CHAR_TYPE;
        this->val.cval = val;
        return this;
}

struct constant *create_constant_float
(int line, int column, cflat_float val)
{
        struct constant *this = malloc(sizeof(struct constant));
        this->pos.line = line;
        this->pos.column = column;
        this->type = FLOAT_TYPE;
        this->val.fval = val;
        return this;
}

struct constant *create_constant_int
(int line, int column, cflat_int val)
{
        struct constant *this = malloc(sizeof(struct constant));
        this->pos.line = line;
        this->pos.column = column;
        this->type = INT_TYPE;
        this->val.ival = val;
        return this;
}

void free_program
(struct program *this)
{
        if (!this) return;
        free_type_decl_list(this->type_decl_list);
        free_var_decl_stmt_list(this->var_decl_stmt_list);
        free_function_def_list(this->function_def_list);
        free(this);
}

void free_type_decl_list
(struct type_decl_list *this)
{
        if (!this) return;
        free_type_decl(this->type_decl);
        free_type_decl_list(this->type_decl_list);
        free(this);
}

void free_type_decl
(struct type_decl *this)
{
        if (!this) return;
        free_var_decl(this->var_decl);
        free(this);
}

void free_var_decl_stmt_list
(struct var_decl_stmt_list *this)
{
        if (!this) return;
        free_var_decl(this->var_decl);
        free_var_decl_stmt_list(this->var_decl_stmt_list);
        free(this);
}

void free_var_decl
(struct var_decl *this)
{
        if (!this) return;
        switch (this->type) {
        case BASIC_VAR:
                break;
        case STRUCT_VAR:
                free_struct_type(this->val.struct_var);
                break;
        case TYPEDEF_VAR:
                free(this->val.typedef_id);
                break;
        }
        free_array_specifier(this->array_specifier);
        free(this->id);
        free(this);
}

void free_struct_type
(struct struct_type *this)
{
        if (!this) return;
        free_var_decl_stmt_list(this->var_decl_stmt_list);
        free(this);
}

void free_array_specifier
(struct array_specifier *this)
{
        if (!this) return;
        free_constant(this->constant);
        free(this);
}

void free_function_def_list
(struct function_def_list *this)
{
        if (!this) return;
        free_function_def(this->function_def);
        free_function_def_list(this->function_def_list);
        free(this);
}

void free_function_def
(struct function_def *this)
{
        if (!this) return;
        free_function_param_list(this->function_param_list);
        free_function_body(this->function_body);
        free(this->id);
        free(this);
}

void free_function_param_list
(struct function_param_list *this)
{
        if (!this) return;
        free_var_decl(this->var_decl);
        free_function_param_list(this->function_param_list);
        free(this);
}

void free_function_body
(struct function_body *this)
{
        if (!this) return;
        free_var_decl_stmt_list(this->var_decl_stmt_list);
        free_stmt_list(this->stmt_list);
        free_return_stmt(this->return_stmt);
        free(this);
}

void free_function_call
(struct function_call *this)
{
        if (!this) return;
        free_function_arg_list(this->function_arg_list);
        free(this->id);
        free(this);
}

void free_function_arg_list
(struct function_arg_list *this)
{
        if (!this) return;
        free_expr(this->expr);
        free_function_arg_list(this->function_arg_list);
        free(this);
}

void free_stmt_list
(struct stmt_list *this)
{
        if (!this) return;
        free_stmt(this->stmt);
        free_stmt_list(this->stmt_list);
        free(this);
}

void free_stmt
(struct stmt *this)
{
        if (!this) return;
        switch (this->type) {
        case EXPR_STMT:
                free_expr_stmt(this->val.expr_stmt);
                break;
        case COMPOUND_STMT:
                free_compound_stmt(this->val.compound_stmt);
                break;
        case SELECT_STMT:
                free_select_stmt(this->val.select_stmt);
                break;
        case ITER_STMT:
                free_iter_stmt(this->val.iter_stmt);
                break;
        case RETURN_STMT:
                free_return_stmt(this->val.return_stmt);
                break;
        }
        free(this);
}

void free_expr_stmt
(struct expr_stmt *this)
{
        if (!this) return;
        free_expr(this->expr);
        free(this);
}

void free_compound_stmt
(struct compound_stmt *this)
{
        if (!this) return;
        free_stmt_list(this->stmt_list);
        free(this);
}

void free_select_stmt
(struct select_stmt *this)
{
        if (!this) return;
        free_expr(this->cond);
        free_stmt(this->stmt_if_true);
        free_stmt(this->stmt_if_false);
        free(this);
}

void free_iter_stmt
(struct iter_stmt *this)
{
        if (!this) return;
        free_expr(this->init);
        free_expr(this->cond);
        free_expr(this->after);
        free_stmt(this->body);
        free(this);
}

void free_return_stmt
(struct return_stmt *this)
{
        if (!this) return;
        free_expr(this->expr);
        free(this);
}

void free_expr
(struct expr *this)
{
        if (!this) return;
        switch (this->type) {
        case ASSIGN_EXPR:
                free_var(this->val.assign.assignee);
                free_expr(this->val.assign.assignment);
                break;
        case LOGICAL_OR_EXPR:
        case LOGICAL_AND_EXPR:
        case EQUALITY_EXPR:
        case RELATIONAL_EXPR:
                free_expr(this->val.relation.primary);
                free_expr(this->val.relation.secondary);
                break;
        case ADDITIVE_EXPR:
        case MULTIPLICATIVE_EXPR:
                free_expr(this->val.binary_op.primary);
                free_expr(this->val.binary_op.secondary);
                break;
        case UNARY_EXPR:
                switch (this->subtype.unary_expr_subtype) {
                case UNARY_EXPR_SIZEOF_BASIC:
                        break;
                case UNARY_EXPR_SIZEOF_UNARY:
                case UNARY_EXPR_NOT_UNARY:
                case UNARY_EXPR_POSITIVE:
                case UNARY_EXPR_NEGATIVE:
                case UNARY_EXPR_PRE_INCREMENT:
                case UNARY_EXPR_PRE_DECREMENT:
                        free_expr(this->val.unary_op.expr);
                        break;
                }
                break;
        case POSTFIX_EXPR:
                switch (this->subtype.postfix_expr_subtype) {
                case POSTFIX_EXPR_VAR:
                        free_var(this->val.postfix_op.var);
                        break;
                case POSTFIX_EXPR_CONSTANT:
                        free_constant(this->val.postfix_op.constant);
                        break;
                case POSTFIX_EXPR_POST_INCREMENT:
                case POSTFIX_EXPR_POST_DECREMENT:
                case POSTFIX_EXPR_ENCLOSED:
                        free_expr(this->val.postfix_op.expr);
                        break;
                case POSTFIX_EXPR_FUNCTION_CALL:
                        free_function_call(this->val.postfix_op.function_call);
                        break;
                }
                break;
        }
        free(this);
}

void free_var
(struct var *this)
{
        if (!this) return;
        switch (this->type) {
        case IDENTIFIER:
                free(this->val.id);
                break;
        case FIELD:
                free_var(this->val.field.var);
                free(this->val.field.id);
                break;
        case SUBSCRIPT:
                free_var(this->val.subscript.var);
                free_expr(this->val.subscript.expr);
                break;
        }
        free(this);
}

void free_constant
(struct constant *this)
{
        if (!this) return;
        free(this);
}
