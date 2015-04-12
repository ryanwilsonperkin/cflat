#ifndef CFLAT_ASMPRINT_H
#define CFLAT_ASMPRINT_H

void print_assembly(FILE *, struct assembly *);
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

#endif  /* CFLAT_ASMPRINT_H */
