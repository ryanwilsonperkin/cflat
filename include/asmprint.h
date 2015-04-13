#ifndef CFLAT_ASMPRINT_H
#define CFLAT_ASMPRINT_H

void print_assembly(FILE *, struct symbol_table *, struct assembly *);
void print_assembly_symbols(FILE *, struct symbol_table *);
void print_reg(FILE *, enum reg);
void print_line_address(FILE *, struct line_address *, int);
void print_line_address_name(FILE *, struct line_address *);
void print_line_address_constant(FILE *, struct line_address *);
void print_line_address_offset(FILE *, struct line_address *);
void print_line_address_reg(FILE *, struct line_address *, int);
void print_line(FILE *, struct line *);
void print_line_load(FILE *, struct line *);
void print_line_load_address(FILE *, struct line *);
void print_line_store(FILE *, struct line *);
void print_line_label(FILE *, struct line *);
void print_line_add(FILE *, struct line *);
void print_line_sub(FILE *, struct line *);
void print_line_or(FILE *, struct line *);
void print_line_and(FILE *, struct line *);
void print_line_seq(FILE *, struct line *);
void print_line_sne(FILE *, struct line *);
void print_line_slt(FILE *, struct line *);
void print_line_sle(FILE *, struct line *);
void print_line_sgt(FILE *, struct line *);
void print_line_sge(FILE *, struct line *);
void print_line_mul(FILE *, struct line *);
void print_line_div(FILE *, struct line *);
void print_line_rem(FILE *, struct line *);
void print_line_not(FILE *, struct line *);
void print_line_jump(FILE *, struct line *);
void print_line_beqz(FILE *, struct line *);
void print_line_bnez(FILE *, struct line *);
void print_line_beq(FILE *, struct line *);
void print_line_bne(FILE *, struct line *);
void print_line_blt(FILE *, struct line *);
void print_line_ble(FILE *, struct line *);
void print_line_bgt(FILE *, struct line *);
void print_line_bge(FILE *, struct line *);

#endif  /* CFLAT_ASMPRINT_H */
