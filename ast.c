#include <stdlib.h>
#include "ast.h"

struct program_t *create_program
(struct type_decl_list_t *type_decl_list, struct var_decl_list_t *var_decl_list, struct function_def_list_t *function_def_list)
{ return NULL; }

struct type_decl_list_t *create_type_decl_list
(struct type_decl_t *type_decl, struct type_decl_list_t *type_decl_list)
{ return NULL; }

struct type_decl_t *create_type_decl
(enum basic_type type, char *id)
{ return NULL; }

struct var_decl_list_t *create_var_decl_list
(struct var_decl_t *var_decl, struct var_decl_list_t *var_decl_list)
{ return NULL; }

struct var_decl_t *create_var_decl_basic
(enum basic_type type, char *id, struct array_specifier_t *array_specifier)
{ return NULL; } 

struct var_decl_t *create_var_decl_struct
(struct struct_type_t *struct_type, char *id, struct array_specifier_t *array_specifier)
{ return NULL; } 

struct struct_type_t *create_struct_type
(struct var_decl_list_t *var_decl_list)
{ return NULL; }

struct array_specifier_t *create_array_specifier
(cflat_int size)
{ return NULL; }

struct function_def_list_t *create_function_def_list
(struct function_def_t *function_def, struct function_def_list_t *function_def_list)
{ return NULL; }

struct function_def_t *create_basic_function_def
(enum basic_type type, char *id, struct function_params_t *function_params, struct function_body_t *function_body)
{ return NULL; }

struct function_def_t *create_void_function_def
(char *id, struct function_params_t *function_params, struct function_body_t *function_body)
{ return NULL; }

struct function_params_t *create_function_params
(struct var_decl_list_t *var_decl_list)
{ return NULL; }

struct function_body_t *create_function_body
(struct var_decl_list_t *var_decl_list, struct stmt_list_t *stmt_list, struct return_stmt_t *return_stmt)
{ return NULL; }

struct function_call_t *create_function_call
(char *id, struct arg_list_t *arg_list)
{ return NULL; }

struct arg_list_t *create_arg_list
(struct expr_t *expr, struct arg_list_t *arg_list)
{ return NULL; }

struct stmt_list_t *create_stmt_list
(struct stmt_t *stmt, struct stmt_list_t *stmt_list)
{ return NULL; }

struct stmt_t *create_stmt
(enum stmt_type type, void *stmt)
{ return NULL; }

struct expr_stmt_t *create_expr_stmt
(struct expr_t *expr)
{ return NULL; }

struct compound_stmt_t *create_compound_stmt
(struct stmt_list_t *stmt_list)
{ return NULL; }

struct select_stmt_t *create_select_stmt
(struct expr_t *expr, struct stmt_t *if_true_stmt, struct stmt_t *if_false_stmt)
{ return NULL; }

struct iter_stmt_t *create_iter_stmt
(struct expr_t *init, struct expr_t *cond, struct expr_t *after, struct stmt_t *body)
{ return NULL; }

struct return_stmt_t *create_return_stmt
(struct expr_t *expr)
{ return NULL; }

struct expr_t *create_expr_t
(struct assign_expr_t *assign_expr)
{ return NULL; }

struct assign_expr_t *create_assign_expr
(struct var_t *var, struct assign_expr_t *assign_expr)
{ return NULL; }

struct assign_expr_t *wrap_logical_or_expr
(struct logical_or_expr_t *logical_or_expr)
{ return NULL; }

struct logical_or_expr_t *create_logical_or_expr
(struct logical_or_expr_t *logical_or_expr, struct logical_and_expr_t *logical_and_expr)
{ return NULL; }

struct logical_or_expr_t *wrap_logical_and_expr
(struct logical_and_expr_t *logical_and_expr)
{ return NULL; }

struct logical_and_expr_t *create_logical_and_expr
(struct logical_and_expr_t *logical_and_expr, struct equality_expr_t *equality_expr)
{ return NULL; }

struct logical_and_expr_t *wrap_equality_expr
(struct equality_expr_t *equality_expr)
{ return NULL; }

struct equality_expr_t *create_equality_expr
(enum equality_expr_type type, struct equality_expr_t *equality_expr, struct relational_expr_t *relational_expr)
{ return NULL; }

struct equality_expr_t *wrap_relational_expr
(struct relational_expr_t *relational_expr)
{ return NULL; }

struct relational_expr_t *create_relational_expr
(enum relational_expr_type type, struct relational_expr_t *relational_expr, struct additive_expr_t *additive_expr)
{ return NULL; }

struct relational_expr_t *wrap_additive_expr
(struct additive_expr_t *additive_expr)
{ return NULL; }

struct additive_expr_t *create_additive_expr
(enum additive_expr_type type, struct additive_expr_t *additive_expr, struct multiplicative_expr_t *multiplicative_expr)
{ return NULL; }

struct additive_expr_t *wrap_multiplicative_expr
(struct multiplicative_expr_t *multiplicative_expr)
{ return NULL; }

struct multiplicative_expr_t *create_multiplicative_expr
(enum multiplicative_expr_type type, struct multiplicative_expr_t *multiplicative_expr, struct unary_expr_t *unary_expr)
{ return NULL; }

struct multiplicative_expr_t *wrap_unary_expr
(struct unary_expr_t *unary_expr)
{ return NULL; }

struct unary_expr_t *create_unary_expr_sizeof_basic
(enum basic_type type)
{ return NULL; }

struct unary_expr_t *create_unary_expr
(enum unary_expr_type type, struct unary_expr_t *unary_expr)
{ return NULL; }

struct unary_expr_t *wrap_postfix_expr
(struct postfix_expr_t *postfix_expr)
{ return NULL; }

struct postfix_expr_t *create_postfix_expr
(enum postfix_expr_type type, struct postfix_expr_t *postfix_expr)
{ return NULL; }

struct postfix_expr_t *wrap_var
(struct var_t *var)
{ return NULL; }

struct postfix_expr_t *wrap_constant
(struct constant_t *constant)
{ return NULL; }

struct postfix_expr_t *wrap_enclosed
(struct expr_t *expr)
{ return NULL; }

struct postfix_expr_t *wrap_function_call
(struct function_call_t *function_call)
{ return NULL; }

struct var_t *create_var_identifier
(char *id)
{ return NULL; }

struct var_t *create_var_field
(struct var_t *var, char *id)
{ return NULL; }

struct var_t *create_var_subscript
(struct var_t *var, struct expr_t *expr)
{ return NULL; }

struct constant_t *create_constant_char
(cflat_char val)
{ return NULL; }

struct constant_t *create_constant_float
(cflat_float val)
{ return NULL; }

struct constant_t *create_constant_int
(cflat_int val)
{ return NULL; }

enum basic_type recognize_basic_type
(char *type_name)
{ return CHAR_TYPE; }
