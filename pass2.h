#ifndef PASS2_H
#define PASS2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ir.h"
#include "pass.h"

void pass2_initialize(void);
void pass2_set_output(FILE* f);
bool pass2_finalize(void);
bool pass2_emit_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2,
                      int64_t imm, const char* label, reloc_kind_t reloc,
                      uint32_t addr, int lineno);
bool pass2_emit_section(section_t sect);
bool pass2_emit_data(int64_t value, uint32_t size);
bool pass2_emit_align(uint32_t pad_bytes);
void pass2_run(void);

#endif