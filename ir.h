#ifndef IR_H
#define IR_H

#include <stddef.h>
#include <stdint.h>
#include "pass.h"

// The kind of IR entry this is
typedef enum {
  IR_INSTR,
  IR_DATA,
  IR_ALIGN
} ir_kind_t;

// Instruction formats
typedef enum {
  IF_R,
  IF_I,
  IF_S,
  IF_B,
  IF_J,
  IF_UI
} ir_fmt_t;

// Relocation, resolution kind for label based operands
typedef enum {
  RELOC_NONE,
  RELOC_HI20,
  RELOC_LO12,
  RELOC_BOFF, 
  RELOC_JTGT
} reloc_kind_t;

// Opcodes needed for pass 2
typedef enum {
  OP_ADD,
  OP_SUB, 
  OP_SLL, 
  OP_SLT, 
  OP_SLTU, 
  OP_XOR, 
  OP_SRL, 
  OP_SRA, 
  OP_OR, 
  OP_AND, 
  OP_ADDI, 
  OP_SLLI, 
  OP_LW, 
  OP_SLTI, 
  OP_SLTIU, 
  OP_XORI, 
  OP_ORI, 
  OP_ANDI, 
  OP_SRLI, 
  OP_SRAI, 
  OP_JALR, 
  OP_LB, 
  OP_LH, 
  OP_LBU, 
  OP_LHU,
  OP_ECALL,
  OP_EBREAK,
  OP_AUIPC,
  OP_LUI,
  OP_SW, 
  OP_SH,
  OP_SB,
  OP_BEQ, 
  OP_BNE, 
  OP_BLT, 
  OP_BGE, 
  OP_BLTU, 
  OP_BGEU,
  OP_JAL
} opcode_t;

typedef struct {
  ir_kind_t kind;
  opcode_t op;
  ir_fmt_t fmt;
  int rd, rs1, rs2;
  int64_t imm;
  char* label;
  reloc_kind_t reloc;
  section_t sect;
  uint32_t addr;
  uint32_t size;
  int line;
} ir_entry_t;

void ir_init(void);
void ir_append_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int32_t imm, const char* label, 
          reloc_kind_t rk, section_t sect, uint32_t addr, uint32_t size, int line);
void ir_append_data(section_t sect, uint32_t addr, uint32_t word, uint32_t size, int line);
void ir_append_align(section_t sect, uint32_t addr_before, uint32_t pad_bytes, int line);
size_t ir_count(void);
const ir_entry_t* ir_get(size_t i);
void ir_dump_summary(size_t max_rows);
void ir_clear(void);

#endif