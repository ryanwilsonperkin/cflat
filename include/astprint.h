#ifndef CFLAT_ASTPRINT_H
#define CFLAT_ASTPRINT_H

#include <stdio.h>
#include "ast.h"

void print_ast(FILE *, struct program *);
void print_basic_type(FILE *, enum basic_type, int);
void print_program(FILE *, struct program *, int);
void print_type_decl_list(FILE *, struct type_decl_list *, int);
void print_type_decl(FILE *, struct type_decl *, int);
void print_var_decl_stmt_list(FILE *, struct var_decl_stmt_list *, int);
void print_var_decl(FILE *, struct var_decl *, int);
void print_struct_type(FILE *, struct struct_type *, int);
void print_array_specifier(FILE *, struct array_specifier *, int);
void print_function_def_list(FILE *, struct function_def_list *, int);
void print_function_def(FILE *, struct function_def *, int);
void print_function_param_list(FILE *, struct function_param_list *, int);
void print_function_body(FILE *, struct function_body *, int);
void print_function_call(FILE *, struct function_call *, int);
void print_function_arg_list(FILE *, struct function_arg_list *, int);
void print_stmt_list(FILE *, struct stmt_list *, int);
void print_stmt(FILE *, struct stmt *, int);
void print_expr_stmt(FILE *, struct expr_stmt *, int);
void print_compound_stmt(FILE *, struct compound_stmt *, int);
void print_select_stmt(FILE *, struct select_stmt *, int);
void print_iter_stmt(FILE *, struct iter_stmt *, int);
void print_return_stmt(FILE *, struct return_stmt *, int);
void print_expr(FILE *, struct expr *, int);
void print_var(FILE *, struct var *, int);
void print_constant(FILE *, struct constant *, int);

#endif  /* CFLAT_ASTPRINT_H */
