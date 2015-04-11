#ifndef CFLAT_INTERMEDIATEPRINT_H
#define CFLAT_INTERMEDIATEPRINT_H

#include "intermediate.h"

void print_intermediate(FILE *, struct instructions *);
void print_quad(FILE *, struct quad *);
void print_quad_binary_assign(FILE *, struct quad *);
void print_quad_unary_assign(FILE *, struct quad *);
void print_quad_copy(FILE *, struct quad *);
void print_quad_copy_from_addr(FILE *, struct quad *);
void print_quad_copy_to_addr(FILE *, struct quad *);
void print_quad_label(FILE *, struct quad *);
void print_quad_unconditional_jump(FILE *, struct quad *);
void print_quad_conditional_jump(FILE *, struct quad *);
void print_quad_relational_jump(FILE *, struct quad *);
void print_quad_procedure_param(FILE *, struct quad *);
void print_quad_procedure_call(FILE *, struct quad *);
void print_quad_procedure_return(FILE *, struct quad *);
void print_quad_op_binary(FILE *, struct quad_address *, struct quad_address *, enum quad_op);
void print_quad_op_unary(FILE *o, struct quad_address *, enum quad_op);
void print_quad_address(FILE *, struct quad_address *);

#endif  /* CFLAT_INTERMEDIATEPRINT_H */
