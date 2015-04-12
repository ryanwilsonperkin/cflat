#include <assert.h>
#include <stdlib.h>

#include "asm.h"

struct assembly *create_assembly
()
{
        int i;
        struct assembly *this = malloc(sizeof(struct assembly));
        this->lines = NULL;
        this->n_lines = 0;
        for (i = 0; i < NUM_IREGS; i++) {
                this->iregs[i] = 0;
        }
        for (i = 0; i < NUM_FREGS; i++) {
                this->fregs[i] = 0;
        }
        return this;
}

struct line_address *create_line_address_name
(enum basic_type basic_type, char *name)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_NAME;
        this->basic_type = basic_type;
        this->val.name = name;
        return this;
}

struct line_address *create_line_address_constant
(enum basic_type basic_type, union value constant)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_CONSTANT;
        this->basic_type = basic_type;
        this->val.constant = constant;
        return this;
}

struct line_address *create_line_address_offset
(enum basic_type basic_type, int offset, int reg)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_OFFSET;
        this->basic_type = basic_type;
        this->val.offset.offset = offset;
        this->val.offset.reg = reg;
        return this;
}

struct line_address *create_line_address_register
(enum basic_type basic_type, int reg)
{
        struct line_address *this = malloc(sizeof(struct line_address));
        this->type = LINE_ADDRESS_REG;
        this->basic_type = basic_type;
        this->val.reg = reg;
        return this;
}

struct line *create_line_load(struct line_address *src, struct line_address *dest)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_LOAD;
        this->val.load.src = src;
        this->val.load.dest = dest;
        return this;
}

struct line *create_line_store(struct line_address *src, struct line_address *dest)
{
        struct line *this = malloc(sizeof(struct line));
        this->type = LINE_STORE;
        this->val.store.src = src;
        this->val.store.dest = dest;
        return this;
}

void add_line
(struct assembly *assembly, struct line *line)
{
        assembly->n_lines++;
        assembly->lines = realloc(assembly->lines, sizeof(struct line *) * assembly->n_lines);
        assembly->lines[assembly->n_lines - 1] = line;
}

int get_ireg
(struct assembly *assembly)
{
        int i;
        for (i = 0; i < NUM_IREGS; i++) {
                if (!assembly->iregs[i]) {
                        assembly->iregs[i] = 1;
                        return i;
                }
        }
        assert(0);  /* Out of registers. */
}

int get_freg
(struct assembly *assembly)
{
        int i;
        for (i = 0; i < NUM_FREGS; i++) {
                if (!assembly->fregs[i]) {
                        assembly->fregs[i] = 1;
                        return i;
                }
        }
        assert(0);  /* Out of registers. */
}

void unget_ireg
(struct assembly *assembly, int reg)
{
        assert(assembly->iregs[reg]);  /* Register must be allocated. */
        assembly->iregs[reg] = 0;
}

void unget_freg
(struct assembly *assembly, int reg)
{
        assert(assembly->fregs[reg]);  /* Register must be allocated. */
        assembly->fregs[reg] = 0;
}

struct assembly *parse_assembly
(struct symbol_table *global, struct instructions *instructions)
{
        int i;
        struct symbol *symbol;
        struct quad *quad;
        struct assembly *this = create_assembly();
        struct symbol_table *local = global;
        for (i = 0; i < instructions->n_quads; i++) {
                quad = instructions->quads[i];
                switch (quad->type) {
                case QUAD_BINARY_ASSIGN:
                        parse_assembly_binary_assign(global, local, this, quad);
                        break;
                case QUAD_UNARY_ASSIGN:
                        parse_assembly_unary_assign(global, local, this, quad);
                        break;
                case QUAD_COPY:
                        parse_assembly_copy(global, local, this, quad);
                        break;
                case QUAD_COPY_ADDR:
                        parse_assembly_copy_addr(global, local, this, quad);
                        break;
                case QUAD_COPY_FROM_ADDR:
                        parse_assembly_copy_from_addr(global, local, this, quad);
                        break;
                case QUAD_COPY_TO_ADDR:
                        parse_assembly_copy_to_addr(global, local, this, quad);
                        break;
                case QUAD_LABEL:
                        symbol = get_symbol(global, quad->val.label.label);
                        if (symbol && symbol->type == SYMBOL_FUNCTION) {
                                local = symbol->scoped_table;
                        }
                        parse_assembly_label(global, local, this, quad);
                        break;
                case QUAD_UNCONDITIONAL_JUMP:
                        parse_assembly_unconditional_jump(global, local, this, quad);
                        break;
                case QUAD_CONDITIONAL_JUMP:
                        parse_assembly_conditional_jump(global, local, this, quad);
                        break;
                case QUAD_RELATIONAL_JUMP:
                        parse_assembly_relational_jump(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_PARAM:
                        parse_assembly_procedure_param(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_CALL:
                        parse_assembly_procedure_call(global, local, this, quad);
                        break;
                case QUAD_PROCEDURE_RETURN:
                        parse_assembly_procedure_return(global, local, this, quad);
                        break;
                default:
                        assert(0);  /* Invalid enum value. */
                }
        }
        return this;
}

void parse_assembly_instruction
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_binary_assign
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_unary_assign
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_from_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_copy_to_addr
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_label
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_unconditional_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_conditional_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_relational_jump
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_param
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_call
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}

void parse_assembly_procedure_return
(struct symbol_table *global, struct symbol_table *local, struct assembly *assembly, struct quad *this)
{
}
