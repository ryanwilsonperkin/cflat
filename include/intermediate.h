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
        QUAD_LABEL,
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
                } label; 
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

struct instructions {
        struct quad **quads;
        unsigned int n_quads, n_labels, n_temps;
};

struct instructions *create_instructions();
struct instruction *create_instruction(char *, struct quad *);
struct quad *create_quad_binary_assign(struct quad_address *, struct quad_address *, struct quad_address *, enum quad_op);
struct quad *create_quad_unary_assign(struct quad_address *, struct quad_address *, enum quad_op);
struct quad *create_quad_copy(struct quad_address *, struct quad_address *);
struct quad *create_quad_label(char *);
struct quad *create_quad_unconditional_jump(char *);
struct quad *create_quad_conditional_jump(struct quad_address *, char *);
struct quad *create_quad_relational_jump(struct quad_address *, struct quad_address *, enum quad_op, char *);
struct quad *create_quad_procedure_param(struct quad_address *);
struct quad *create_quad_procedure_call(char *, unsigned int);
struct quad *create_quad_procedure_return(struct quad_address *);
struct quad_address *create_quad_address_name(char *);
struct quad_address *create_quad_address_constant(struct constant *);
struct quad_address *create_quad_address_temp(unsigned int);

void add_instruction(struct instructions *, struct quad *);

struct instructions *parse_instructions(struct program *);
void parse_instructions_program(struct instructions *, struct program *);
void parse_instructions_function_def_list(struct instructions *, struct function_def_list *);
void parse_instructions_function_def(struct instructions *, struct function_def *);
void parse_instructions_function_body(struct instructions *, struct function_body *);
void parse_instructions_stmt_list(struct instructions *, struct stmt_list *);
void parse_instructions_stmt(struct instructions *, struct stmt *);
void parse_instructions_expr_stmt(struct instructions *, struct expr_stmt *);
void parse_instructions_compound_stmt(struct instructions *, struct compound_stmt *);
void parse_instructions_select_stmt(struct instructions *, struct select_stmt *);
void parse_instructions_iter_stmt(struct instructions *, struct iter_stmt *);
void parse_instructions_return_stmt(struct instructions *, struct return_stmt *);

#endif  /* CFLAT_INTERMEDIATE_H */
