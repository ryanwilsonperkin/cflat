#ifndef CFLAT_TYPECHECK_H
#define CFLAT_TYPECHECK_H

#include "ast.h"
#include "symbol.h"

void type_check_program(struct symbol_table *, struct program *);
void type_check_function_def_list(struct symbol_table *, struct function_def_list *);
void type_check_function_def(struct symbol_table *, struct function_def *);
void type_check_function_body(struct symbol_table *, struct symbol_table *, struct function_body *);
void type_check_function_body(struct symbol_table *, struct symbol_table *, struct function_body *);
void type_check_stmt_list(struct symbol_table *, struct symbol_table *, struct stmt_list *);
void type_check_stmt(struct symbol_table *, struct symbol_table *, struct stmt *);
void type_check_expr_stmt(struct symbol_table *, struct symbol_table *, struct expr_stmt *);
void type_check_compound_stmt(struct symbol_table *, struct symbol_table *, struct compound_stmt *);
void type_check_select_stmt(struct symbol_table *, struct symbol_table *, struct select_stmt *);
void type_check_iter_stmt(struct symbol_table *, struct symbol_table *, struct iter_stmt *);
void type_check_return_stmt(struct symbol_table *, struct symbol_table *, struct return_stmt *);
struct symbol *translate_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_assign_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_logical_or_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_logical_and_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_equality_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_relational_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_additive_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_multiplicative_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_unary_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_postfix_expr(struct symbol_table *, struct symbol_table *, struct expr *);
struct symbol *translate_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_identifier_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_field_var(struct symbol_table *, struct symbol_table *, struct var *);
struct symbol *translate_subscript_var(struct symbol_table *, struct symbol_table *, struct var *);

#endif  /* CFLAT_TYPECHECK_H */
