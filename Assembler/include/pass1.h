#ifndef PASS1_H
#define PASS1_H

#include <stdint.h>
#include "ir.h"
#include "section.h"

void pass1_initialize(void);
void pass1_finalize(void);
void pass1_emit_instruction(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int64_t imm,
                            const char* label, reloc_kind_t reloc, int lineno);
void pass1_emit_label(char* label);
void pass1_emit_align(uint32_t pad_bytes, int lineno);
void pass1_emit_data(int64_t word, uint32_t size, int lineno);
void pass1_emit_space(uint64_t num_bytes, int lineno);

#endif