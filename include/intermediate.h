#ifndef CFLAT_INTERMEDIATE_H
#define CFLAT_INTERMEDIATE_H

#include "ast.h"

enum address_type {
        ADDRESS_NAME,
        ADDRESS_CONSTANT,
        ADDRESS_TEMP
};

struct quad_address {
        enum address_type type;
        union {
                char *id;
                struct constant *constant;
                unsigned int temp;
        } val;
};

enum quad_type {
        QUAD_BINARY_ASSIGN,
        QUAD_UNARY_ASSIGN,
        QUAD_COPY,
        QUAD_UNCONDITIONAL_JUMP,
        QUAD_CONDITIONAL_JUMP,
        QUAD_RELATIONAL_JUMP,
        QUAD_PROCEDURE_PARAM,
        QUAD_PROCEDURE_CALL,
        QUAD_PROCEDURE_RETURN
        /* QUAD_INDEXED_COPY purposely not implemented */
        /* QUAD_POINTER_ASSIGN purposely not implemented */
};

enum quad_op {
        QUAD_OP_EQUAL,
        QUAD_OP_NOT_EQUAL,
        QUAD_OP_LESS_THAN,
        QUAD_OP_LESS_THAN_OR_EQUAL,
        QUAD_OP_GREATER_THAN,
        QUAD_OP_GREATER_THAN_OR_EQUAL,
        QUAD_OP_ADD,
        QUAD_OP_SUBTRACT,
        QUAD_OP_MULTIPLY,
        QUAD_OP_DIVIDE,
        QUAD_OP_MODULO,
        QUAD_OP_NOT_UNARY,
        QUAD_OP_POSITIVE,
        QUAD_OP_NEGATIVE,
        QUAD_OP_PRE_INCREMENT,
        QUAD_OP_PRE_DECREMENT,
        QUAD_OP_POST_INCREMENT,
        QUAD_OP_POST_DECREMENT
};

struct quad {
        enum quad_type type;
        union {
                struct {
                        struct quad_address *arg1, *arg2, *result;
                        enum quad_op op;
                } binary_assign;
                struct {
                        struct quad_address *arg, *result;
                        enum quad_op op;
                } unary_assign;
                struct {
                        struct quad_address *arg, *result;
                } copy;
                struct {
                        char *label;
                } unconditional_jump;
                struct {
                        struct quad_address *arg;
                        char *label;
                } conditional_jump;
                struct {
                        struct quad_address *arg1, *arg2;
                        enum quad_op op;
                        char *label;
                } relational_jump;
                struct {
                        struct quad_address *arg;
                } procedure_param;
                struct {
                        char *label;
                        unsigned int n_params;
                } procedure_call;
                struct {
                        struct quad_address *result;
                } procedure_return;
        } val;
};

struct instruction {
        char *label;
        struct quad *quad;
};

struct instruction_list {
        struct instruction **instructions;
        unsigned int n_instructions;
        unsigned int n_labels;
};

struct instruction_list *create_instruction_list();
struct instruction *create_instruction(char *, struct quad *);
struct quad *create_quad_binary_assign(struct quad_address *, struct quad_address *, struct quad_address *, enum quad_op);
struct quad *create_quad_unary_assign(struct quad_address *, struct quad_address *, enum quad_op);
struct quad *create_quad_copy(struct quad_address *, struct quad_address *);
struct quad *create_quad_unconditional_jump(char *);
struct quad *create_quad_conditional_jump(struct quad_address *, char *);
struct quad *create_quad_relational_jump(struct quad_address *, struct quad_address *, enum quad_op, char *);
struct quad *create_quad_procedure_param(struct quad_address *);
struct quad *create_quad_procedure_call(char *, unsigned int);
struct quad *create_quad_procedure_return(struct quad_address *);
struct quad_address *create_quad_address_name(char *);
struct quad_address *create_quad_address_constant(struct constant *);
struct quad_address *create_quad_address_temp();

void add_instruction(struct instruction_list *, struct quad *);
void add_labeled_instruction(struct instruction_list *, char *, struct quad *);

struct instruction_list *parse_instructions(struct program *);
void parse_instructions_program(struct instruction_list *, struct program *);
void parse_instructions_type_decl_list(struct instruction_list *, struct type_decl_list *);
void parse_instructions_type_decl(struct instruction_list *, struct type_decl *);
void parse_instructions_var_decl_stmt_list(struct instruction_list *, struct var_decl_stmt_list *);
void parse_instructions_var_decl(struct instruction_list *, struct var_decl *);
void parse_instructions_struct_type(struct instruction_list *, struct struct_type *);
void parse_instructions_function_def_list(struct instruction_list *, struct function_def_list *);
void parse_instructions_function_def(struct instruction_list *, struct function_def *);
void parse_instructions_function_param_list(struct instruction_list *, struct function_param_list *);
void parse_instructions_function_body(struct instruction_list *, struct function_body *);

#endif  /* CFLAT_INTERMEDIATE_H */
