#ifndef CFLAT_ASTPRINT_H
#define CFLAT_ASTPRINT_H

#include <stdio.h>
#include "ast.h"

void print_ast(FILE *, struct program_t *);
void print_basic_type(FILE *, enum basic_type, int);
void print_program(FILE *, struct program_t *, int);
void print_type_decl_list(FILE *, struct type_decl_list_t *, int);
void print_type_decl(FILE *, struct type_decl_t *, int);
void print_var_decl_stmt_list(FILE *, struct var_decl_stmt_list_t *, int);
void print_var_decl(FILE *, struct var_decl_t *, int);
void print_struct_type(FILE *, struct struct_type_t *, int);
void print_array_specifier(FILE *, struct array_specifier_t *, int);
void print_function_def_list(FILE *, struct function_def_list_t *, int);
void print_function_def(FILE *, struct function_def_t *, int);
void print_function_param_list(FILE *, struct function_param_list_t *, int);
void print_function_body(FILE *, struct function_body_t *, int);
void print_function_call(FILE *, struct function_call_t *, int);
void print_function_arg_list(FILE *, struct function_arg_list_t *, int);
void print_stmt_list(FILE *, struct stmt_list_t *, int);
void print_stmt(FILE *, struct stmt_t *, int);
void print_expr_stmt(FILE *, struct expr_stmt_t *, int);
void print_compound_stmt(FILE *, struct compound_stmt_t *, int);
void print_select_stmt(FILE *, struct select_stmt_t *, int);
void print_iter_stmt(FILE *, struct iter_stmt_t *, int);
void print_return_stmt(FILE *, struct return_stmt_t *, int);
void print_expr(FILE *, struct expr_t *, int);
void print_var(FILE *, struct var_t *, int);
void print_constant(FILE *, struct constant_t *, int);

#endif  /* CFLAT_ASTPRINT_H */
