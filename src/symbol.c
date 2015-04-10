#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol.h"
#include "typecheck.h"

static inline int num_digits
(unsigned int n)
{
        /* http://stackoverflow.com/a/1489928/3676516 */
        return n > 0 ? (int) log10(n) + 1 : 1;
}

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

struct symbol_table *create_symbol_table
()
{
        struct symbol_table *this = malloc(sizeof(struct symbol_table));
        this->size = 0;
        this->n_items = 0;
        this->n_temps = 0;
        this->items = NULL;
        this->temps = NULL;
        return this;
}

struct symbol_table_item *create_symbol_table_item
(char *id, unsigned int offset, struct symbol *symbol)
{
        struct symbol_table_item *this = malloc(sizeof(struct symbol_table_item));
        this->id = id;
        this->symbol = symbol;
        this->offset = offset;
        return this;
}

struct symbol *create_symbol_basic
(enum basic_type basic_type)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_BASIC;
        this->val.basic_type = basic_type;
        this->scoped_table = NULL;
        this->size = 4;
        return this;
}

struct symbol *create_symbol_named
(struct symbol *symbol)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_NAMED;
        this->val.symbol = symbol;
        this->scoped_table = NULL;
        this->size = symbol->size;
        return this;
}

struct symbol *create_symbol_array
(struct symbol *symbol, int size)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_ARRAY;
        this->val.array.symbol = symbol;
        this->val.array.size = size;
        this->scoped_table = NULL;
        this->size = symbol->size * size;
        return this;
}

struct symbol *create_symbol_struct
(struct struct_type *struct_type)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_STRUCT;
        this->val.struct_type = struct_type;
        this->scoped_table = create_symbol_table();
        this->size = 0;
        return this;
}

struct symbol *create_symbol_function
(struct function_def *function_def)
{
        struct symbol *this = malloc(sizeof(struct symbol));
        this->type = SYMBOL_FUNCTION;
        this->val.function_def = function_def;
        this->scoped_table = create_symbol_table();
        this->size = 4;
        return this;
}

struct symbol *get_symbol
(struct symbol_table *symbol_table, char *id)
{
        int i;
        struct symbol_table_item *item;
        for (i = 0; i < symbol_table->n_items; i++) {
                item = symbol_table->items[i];
                if (strcmp(item->id, id) == 0) {
                        return item->symbol;
                }
        }
        return NULL;
}

void add_symbol
(struct symbol_table *symbol_table, char *id, struct symbol *symbol)
{
        if (get_symbol(symbol_table, id)) {
                fprintf(stderr, "error: redefinition of symbol '%s'\n", id);
                exit(EXIT_FAILURE);
        }
        symbol_table->n_items++;
        symbol_table->items = realloc(symbol_table->items, sizeof(struct symbol_table_item *) * symbol_table->n_items);
        symbol_table->items[symbol_table->n_items - 1] = create_symbol_table_item(id, symbol_table->size, symbol);
        symbol_table->size += symbol->size;
}

void add_symbol_temp
(struct symbol_table *symbol_table, struct symbol *symbol)
{
        char *id, *prefix = "temp~";
        symbol_table->n_temps++;
        id = malloc(strlen(prefix) + num_digits(symbol_table->n_temps) + 1);
        sprintf(id, "%s%d", prefix, symbol_table->n_temps);
        if (get_symbol(symbol_table, id)) {
                fprintf(stderr, "error: redefinition of symbol '%s'\n", id);
                exit(EXIT_FAILURE);
        }
        symbol_table->temps = realloc(symbol_table->temps, sizeof(struct symbol_table_item *) * symbol_table->n_temps);
        symbol_table->temps[symbol_table->n_temps - 1] = create_symbol_table_item(id, 0, symbol);
}

void add_symbol_type
(struct symbol_table *symbol_table, char *id, struct symbol *symbol)
{
        if (get_symbol(symbol_table, id)) {
                fprintf(stderr, "error: redefinition of symbol '%s'\n", id);
                exit(EXIT_FAILURE);
        }
        symbol_table->n_items++;
        symbol_table->items = realloc(symbol_table->items, sizeof(struct symbol_table_item *) * symbol_table->n_items);
        symbol_table->items[symbol_table->n_items - 1] = create_symbol_table_item(id, 0, symbol);
}

struct symbol_table *parse_symbols
(struct program *program)
{
        struct symbol_table *this = create_symbol_table();
        parse_program(this, program);
        return this;
}

void parse_program
(struct symbol_table *global, struct program *this)
{
        if (!this) return;
        parse_type_decl_list(global, this->type_decl_list);
        parse_var_decl_stmt_list(global, global, this->var_decl_stmt_list);
        parse_function_def_list(global, this->function_def_list);
}

void parse_type_decl_list
(struct symbol_table *global, struct type_decl_list *this)
{
        if (!this) return;
        parse_type_decl_list(global, this->type_decl_list);
        parse_type_decl(global, this->type_decl);
}

void parse_type_decl
(struct symbol_table *global, struct type_decl *this)
{
        struct symbol *named_symbol;
        if (!this) return;
        named_symbol = create_symbol_named(translate_var_decl(global, this->var_decl));
        add_symbol_type(global, this->var_decl->id, named_symbol);
}

void parse_var_decl_stmt_list
(struct symbol_table *global, struct symbol_table *local, struct var_decl_stmt_list *this)
{
        if (!this) return;
        parse_var_decl_stmt_list(global, local, this->var_decl_stmt_list);
        parse_var_decl(global, local, this->var_decl);
}

void parse_var_decl
(struct symbol_table *global, struct symbol_table *local, struct var_decl *this)
{
        if (!this) return;
        add_symbol(local, this->id, translate_var_decl(global, this));
}

void parse_struct_type
(struct symbol_table *global, struct struct_type *this)
{
        struct symbol *symbol;
        if (!this) return;
        symbol = create_symbol_struct(this);
        parse_var_decl_stmt_list(global, symbol->scoped_table, this->var_decl_stmt_list);
        symbol->size = symbol->scoped_table->size;
        add_symbol_type(global, this->id, symbol);
}

void parse_function_def_list
(struct symbol_table *global, struct function_def_list *this)
{
        if (!this) return;
        parse_function_def_list(global, this->function_def_list);
        parse_function_def(global, this->function_def);
}

void parse_function_def
(struct symbol_table *global, struct function_def *this)
{
        struct symbol *symbol;
        if (!this) return;
        symbol = create_symbol_function(this);
        add_symbol_type(global, this->id, symbol);
        parse_function_param_list(global, symbol->scoped_table, this->function_param_list);
        parse_function_body(global, symbol->scoped_table, this->function_body);
        symbol->size = symbol->scoped_table->size;
}

void parse_function_param_list
(struct symbol_table *global, struct symbol_table *local, struct function_param_list *this)
{
        if (!this) return;
        parse_function_param_list(global, local, this->function_param_list);
        parse_var_decl(global, local, this->var_decl);
}

void parse_function_body
(struct symbol_table *global, struct symbol_table *local, struct function_body *this)
{
        if (!this) return;
        parse_var_decl_stmt_list(global, local, this->var_decl_stmt_list);
}

struct symbol *translate_var_decl
(struct symbol_table *global, struct var_decl *var_decl)
{
        int size;
        struct symbol *this;
        switch (var_decl->type)
        {
        case BASIC_VAR:
                this = create_symbol_basic(var_decl->val.basic_var);
                break;
        case STRUCT_VAR:
                parse_struct_type(global, var_decl->val.struct_var);
                this = get_symbol(global, var_decl->val.struct_var->id);
                break;
        case TYPEDEF_VAR:
                this = get_symbol(global, var_decl->val.typedef_id);
                break;
        default:
                assert(0);  /* Invalid enum value. */
        }
        if (var_decl->array_specifier) {
                size = var_decl->array_specifier->constant->val.ival;
                if (size < 0) {
                        type_error(var_decl->pos, "'%s' declared as an array with negative size");
                }
                return create_symbol_array(this, size);
        } else {
                return this;
        }
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
        default:
                assert(0);  /* Invalid enum value. */
        }
}

struct symbol *translate_assign_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *assignee, *assignment;
        assignee = translate_var(global, local, this->val.assign.assignee);
        assignment = translate_expr(global, local, this->val.assign.assignment);
        if (!symbol_equivalent(assignee, assignment)) {
                type_error(this->pos, "assignment from incompatible type");
        }
        if (assignee->type != SYMBOL_BASIC) {
                type_error(this->val.assign.assignee->pos, "assignment not supported for type");
        }
        return assignee;
}

struct symbol *translate_logical_or_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC || primary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression (requires int)");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_logical_and_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC || primary->val.basic_type != INT_TYPE) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression (requires int)");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_equality_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_relational_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.relation.primary);
        secondary = translate_expr(global, local, this->val.relation.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.relation.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC) {
                type_error(this->val.relation.primary->pos, "invalid operand to expression");
        }
        symbol = create_symbol_basic(INT_TYPE);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_additive_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.binary_op.primary);
        secondary = translate_expr(global, local, this->val.binary_op.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.binary_op.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.primary->pos, "invalid operand to expression");
        }
        symbol = create_symbol_basic(primary->val.basic_type);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_multiplicative_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *primary, *secondary, *symbol;
        primary = translate_expr(global, local, this->val.binary_op.primary);
        secondary = translate_expr(global, local, this->val.binary_op.secondary);
        if (!symbol_equivalent(primary, secondary)) {
                type_error(this->val.binary_op.primary->pos, "comparison of incompatible types");
        }
        if (primary->type != SYMBOL_BASIC) {
                type_error(this->val.binary_op.primary->pos, "invalid operand to expression");
        }
        if (primary->val.basic_type == FLOAT_TYPE && this->subtype.multiplicative_expr_subtype == MULTIPLICATIVE_EXPR_MODULO) {
                type_error(this->val.binary_op.primary->pos, "invalid operand to expression");
        }
        symbol = create_symbol_basic(primary->val.basic_type);
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_unary_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *symbol;
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
        default:
                assert(0);  /* Invalid enum value. */
        }
        add_symbol_temp(local, symbol);
        return symbol;
}

struct symbol *translate_postfix_expr
(struct symbol_table *global, struct symbol_table *local, struct expr *this)
{
        struct symbol *symbol;
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
        default:
                assert(0);  /* Invalid enum value. */
        }
        return symbol;
}

struct symbol *translate_function_call
(struct symbol_table *global, struct symbol_table *local, struct function_call *this)
{
        struct symbol *symbol, *parent;
        parent = get_symbol(global, this->id);
        if (!parent) {
                type_error(this->pos, "use of undeclared function '%s'", this->id);
        }
        type_check_function_arg_list(global, local, parent->scoped_table, this->function_arg_list, parent->val.function_def->function_param_list); 
        if (parent->val.function_def->type == VOID_FUNCTION) {
                return NULL;
        } else {
                symbol = create_symbol_basic(parent->val.function_def->type_specifier);
                add_symbol_temp(global, symbol);
                return symbol;
        }
}

struct symbol *translate_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
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
        default:
                assert(0);  /* Invalid enum value. */
        }
}

struct symbol *translate_identifier_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *symbol;
        symbol = get_symbol(local, this->val.id);
        symbol = symbol ? symbol : get_symbol(global, this->val.id);
        if (!symbol) {
                type_error(this->pos, "use of undeclared identifier '%s'", this->val.id);
        }
        return symbol;
}

struct symbol *translate_field_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *symbol, *parent;
        parent = translate_var(global, local, this->val.field.var);
        if (!parent || parent->type != SYMBOL_STRUCT) {
                type_error(this->pos, "member reference is not a structure");
        }
        symbol = get_symbol(parent->scoped_table, this->val.field.id);
        if (!symbol) {
                type_error(this->pos, "no member '%s' in struct '%s'", this->val.field.id, parent->val.struct_type->id);
        }
        return symbol;
}

struct symbol *translate_subscript_var
(struct symbol_table *global, struct symbol_table *local, struct var *this)
{
        struct symbol *subscript, *parent;
        subscript = translate_expr(global, local, this->val.subscript.expr);
        parent = translate_var(global, local, this->val.subscript.var);
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
        symbol = create_symbol_basic(this->type);
        add_symbol_temp(local, symbol);
        return symbol;
}

int symbol_equivalent
(struct symbol *s1, struct symbol *s2)
{
        if (!s1 || !s2) {
                return 0;
        }

        if (s2->type == SYMBOL_NAMED) {
                return symbol_equivalent(s1, s2->val.symbol);
        }

        switch (s1->type) {
        case SYMBOL_BASIC:
                return (s1->type == s2->type && 
                        s1->val.basic_type == s2->val.basic_type); 
        case SYMBOL_NAMED:
                return symbol_equivalent(s1->val.symbol, s2);
        case SYMBOL_ARRAY:
                return (s1->type == s2->type && 
                        s1->val.array.size == s2->val.array.size && 
                        symbol_equivalent(s1->val.array.symbol, s2->val.array.symbol));
        case SYMBOL_STRUCT:
                return (s1->type == s2->type &&
                        strcmp(s1->val.struct_type->id, s2->val.struct_type->id) == 0);
        case SYMBOL_FUNCTION:
                return (s1->type == s2->type &&
                        strcmp(s1->val.function_def->id, s2->val.function_def->id) == 0);
        default:
                assert(0);  /* Invalid enum value. */
        }
}
