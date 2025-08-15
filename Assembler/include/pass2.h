#ifndef PASS2_H
#define PASS2_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ir.h"
#include "pass.h"

typedef enum {
  IF_CR,
  IF_CI,
  IF_CIW,
  IF_CL,
  IF_CS,
  IF_CSS,
  IF_CB,
  IF_CJ,
  IF_CA
} c_fmt_t;

typedef enum {
  C_RV_32 = 1,
  C_RV_64 = 2,
  C_RV_ANY = C_RV_32 | C_RV_64
} c_rv_mask_t;

typedef struct {
  c_fmt_t fmt;
  uint8_t quadrant;
  uint8_t funct3;
  uint8_t op12;
  uint8_t op11_10;
  uint8_t op6_5;
  c_rv_mask_t rv_req;
  uint8_t needs_c;
} c_encoding_info_t;

typedef struct {
  uint8_t opcode;
  uint8_t funct3;
  uint8_t funct7;
} encoding_info_t;

void pass2_initialize(void);
void pass2_set_output(FILE* f);
bool pass2_finalize(void);
bool pass2_emit_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2,
                      int64_t imm, const char* label, reloc_kind_t reloc,
                      uint32_t addr, int lineno);
bool pass2_emit_section(section_t sect);
bool pass2_emit_data(int64_t value, uint32_t size);
bool pass2_emit_align(uint32_t pad_bytes);
bool pass2_run(void);
void pass2_report_error(void);

#endif