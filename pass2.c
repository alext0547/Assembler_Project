#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>

#include "pass.h"
#include "ir.h"
#include "symtab.h"
#include "pass2.h"

typedef struct {
  uint8_t opcode;
  uint8_t funct3;
  uint8_t funct7;
} encoding_info_t;

static const encoding_info_t encoding_table[] = {
  [OP_ADD] = { 0b0110011, 0, 0 },
  [OP_SUB] = { 0b0110011, 0, 0b0100000 },
  [OP_SLL] = { 0b0110011, 0b001, 0 },
  [OP_SLT] = { 0b0110011, 0b010, 0 },
  [OP_SLTU] = { 0b0110011, 0b011, 0 },
  [OP_XOR] = { 0b0110011, 0b100, 0 },
  [OP_SRL] = { 0b0110011, 0b101, 0 },
  [OP_SRA] = { 0b0110011, 0b101, 0b0100000 },
  [OP_OR] = { 0b0110011, 0b110, 0 },
  [OP_AND] = { 0b0110011, 0b111, 0 },
  [OP_ADDI] = { 0b0010011, 0, 0 },
  [OP_SLLI] = { 0b0010011, 0b001, 0b0000000 }, 
  [OP_LW] = { 0b0000011, 0b010, 0 }, 
  [OP_SLTI] = { 0b0010011, 0b010, 0 }, 
  [OP_SLTIU] = { 0b0010011, 0b011, 0 }, 
  [OP_XORI] = { 0b0010011, 0b100, 0}, 
  [OP_ORI] = { 0b0010011, 0b110, 0}, 
  [OP_ANDI] = { 0b0010011, 0b111, 0 }, 
  [OP_SRLI] = { 0b0010011, 0b101, 0b0000000 }, 
  [OP_SRAI] = { 0b0010011, 0b101, 0b0100000 }, 
  [OP_JALR] = { 0b1100111, 0b000, 0 }, 
  [OP_LB] = { 0b0000011, 0b000, 0 }, 
  [OP_LH] = { 0b0000011, 0b001, 0 }, 
  [OP_LBU] = { 0b0000011, 0b100, 0 }, 
  [OP_LHU] = { 0b0000011, 0b101, 0 },
  [OP_ECALL] = { 0b1110011, 0b000, 0 },
  [OP_EBREAK] = { 0b1110011, 0b000, 0 },
  [OP_AUIPC] = { 0b0010111, 0, 0 },
  [OP_LUI] = { 0b0110111, 0, 0 },
  [OP_SW] = { 0b0100011, 0b010, 0 },  
  [OP_SH] = { 0b0100011, 0b001, 0 },
  [OP_SB] = { 0b0100011, 0b000, 0 },
  [OP_BEQ] = { 0b1100011, 0b000, 0 },  
  [OP_BNE] = { 0b1100011, 0b001, 0 }, 
  [OP_BLT] = { 0b1100011, 0b100, 0 }, 
  [OP_BGE] = { 0b1100011, 0b101, 0 }, 
  [OP_BLTU] = { 0b1100011, 0b110, 0 }, 
  [OP_BGEU] = { 0b1100011, 0b111, 0 },
  [OP_JAL] = { 0b1101111, 0, 0}
};

static FILE* outfile;
static int had_error;

// Initializes internal state and prepares for the second pass
void pass2_initialize(void) {
  pass_num = 2;
  had_error = 0;
  outfile = NULL;
}

// Registers the output file where the final binary data will be written
void pass2_set_output(FILE* f) {
  outfile = f;
}

// Finalizes pass 2 by freeing any data and checking for errors
bool pass2_finalize(void) {
  free_symtab(symtab);
  ir_clear();
  return had_error == 0;
}

// Looks up a symbol and computes the relocation value
static int32_t resolve_label(const char* label, reloc_kind_t reloc, uint32_t addr, int lineno) {
  int target = search_sym(symtab, label);
  if (target < 0) {
    fprintf(stderr, "Error: (line %d): Undefined label '%s'\n", lineno, label);
    had_error = 1;
    return 0;
  }

  int64_t t = (int64_t)(int32_t)target;
  int64_t pc = (int64_t)addr;
  int64_t pc_offset = t - pc;

  switch (reloc) {
    case RELOC_HI20: {
      int64_t hi = (pc_offset + 0x800) >> 12;
      return (int32_t)(hi << 12);
    }
    case RELOC_LO12: {
      int64_t hi = (pc_offset + 0x800) >> 12;
      int64_t lo = pc_offset - (hi << 12);
      return (int32_t)lo;
    }
    case RELOC_BOFF:
    case RELOC_JTGT:
      return pc_offset;
    default:
      return (int32_t)t;
  }
}

// Dispatches to the correct RV32I encoder for the given format
static bool encode_instruction(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int64_t imm, 
                                   int lineno, uint32_t* out) {
  switch (fmt) {
    case IF_R:
      return encode_rtype(op, rd, rs1, rs2, out);
    case IF_I:
      return encode_itype(op, rd, rs1, imm, lineno, out);
    case IF_S:
      return encode_stype(op, rs1, rs2, imm, lineno, out);
    case IF_B:
      return encode_btype(op, rs1, rs2, imm, lineno, out);
    case IF_J: 
      return encode_jtype(op, rd, imm, lineno, out);
    case IF_UI:
      return encode_uitype(op, rd, imm, lineno, out);
    default:
      fprintf(stderr, "Error (line %d): Unknown format\n", lineno);
      had_error = 1;
      return false;
  }
}

// Packs an R-type instruction into a 32 bit word
static bool encode_rtype(opcode_t op, int rd, int rs1, int rs2, uint32_t* out) {
  const encoding_info_t *info = &encoding_table[op];

  *out = ((uint32_t)(info->funct7 & 0x7f) << 25) |
         ((uint32_t)(rs2 & 0x1F) << 20) |
         ((uint32_t)(rs1 & 0x1F) << 15) |
         ((uint32_t)(info->funct3 & 0x07) << 12) |
         ((uint32_t)(rd & 0x1F) << 7) |
         (uint32_t)(info->opcode & 0x7F);
  return true;
}

// Packs an I-type instruction into a 32 bit word
static bool encode_itype(opcode_t op, int rd, int rs1, int64_t imm, int lineno, uint32_t* out) {
  const encoding_info_t *info = &encoding_table[op];

  if (op == OP_SLLI || op == OP_SRLI || op == OP_SRAI) {
    if (imm < 0 || imm > 31) {
      fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " entered\n", lineno, imm);
      had_error = 1;
      return false;
    }
    uint32_t shamt = (uint32_t)imm & 0x1F;
    *out = ((uint32_t)info->funct7 << 25) |
           (shamt << 20) |
           ((uint32_t)(rs1 & 0x1F) << 15) |
           ((uint32_t)(info->funct3 & 0x07) << 12) |
           ((uint32_t)(rd & 0x1F) << 7) |
           (uint32_t)(info->opcode & 0x7F);
    return true;
  }
  else if (op == OP_EBREAK || op == OP_ECALL) {
    imm = (op == OP_EBREAK) ? 1 : 0;
    rs1 = 0;
    rd = 0;
  }

  if (imm < -2048 || imm > 2047) {
    fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " entered\n", lineno, imm);
    had_error = 1;
    return false;
  }

  uint32_t uimm = (uint32_t)(imm & 0xFFF);
  *out = (uimm << 20) |
         ((uint32_t)(rs1 & 0x1F) << 15) |
         ((uint32_t)(info->funct3 & 0x07) << 12) |
         ((uint32_t)(rd & 0x1F) << 7) |
         (uint32_t)(info->opcode & 0x7F);

  return true;
}

// Packs an S-type instruction into a 32 bit word
static bool encode_stype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint32_t* out) {
  if (imm < -2048 || imm > 2047) {
    fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " entered\n", lineno, imm);
    had_error = 1;
    return false;
  }

  const encoding_info_t *info = &encoding_table[op];
  uint32_t uimm = (uint32_t)(imm & 0xFFF);

  *out = (((uimm >> 5) & 0x7F) << 25) |
         ((uint32_t)(rs2 & 0x1F) << 20) |
         ((uint32_t)(rs1 & 0x1F) << 15) |
         ((uint32_t)(info->funct3 & 0x07) << 12) |
         ((uimm & 0x1F) << 7) |
         (uint32_t)(info->opcode & 0x7F);

  return true;
}

// Packs an B-type instruction into a 32 bit word
static bool encode_btype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint32_t* out) {
  if (imm & 1) {
    fprintf(stderr, "Error: (line %d): branch offset %" PRId64 " not 2-byte aligned\n", lineno, imm);
    had_error = 1;
    return false;
  }
  
  if (imm < -4096 || imm > 4094) {
    fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " entered\n", lineno, imm);
    had_error = 1;
    return false;
  }

  const encoding_info_t *info = &encoding_table[op];
  uint32_t uimm = (uint32_t)((imm >> 1) & 0xFFF);

  *out = (((uimm >> 11) & 0x1) << 31) |
         (((uimm >> 4) & 0x3F) << 25) |
         ((uint32_t)(rs2 & 0x1F) << 20) |
         ((uint32_t)(rs1 & 0x1F) << 15) |
         ((uint32_t)(info->funct3 & 0x07) << 12) |
         ((uimm & 0xF) << 8) |
         (((uimm >> 10) & 0x1) << 7) |
         (uint32_t)(info->opcode & 0x7F);

  return true;
}

// Packs an J-type instruction into a 32 bit word
static bool encode_jtype(opcode_t op, int rd, int64_t imm, int lineno, uint32_t* out) {
  if (imm & 1) {
    fprintf(stderr, "Error (line %d): JAL target not 2-byte aligned (offset %" PRId64 ")\n", lineno, imm);
    had_error = 1;
    return false;
  }

  if (imm < -1048576 || imm > 1048574) {
    fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " entered\n", lineno, imm);
    had_error = 1;
    return false;
  }

  const encoding_info_t *info = &encoding_table[op];
  uint32_t uimm = (uint32_t)((imm >> 1) & 0xFFFFF);

  *out = (((uimm >> 19) & 0x1) << 31) |
         ((uimm & 0x3FF) << 21) |
         (((uimm >> 10) & 0x1) << 20) |
         (((uimm >> 11) & 0xFF) << 12) |
         ((uint32_t)(rd & 0x1F) << 7) |
         (uint32_t)(info->opcode & 0x7F);

  return true;
}

// Packs an UI-type instruction into a 32 bit word
static bool encode_uitype(opcode_t op, int rd, int64_t imm, int lineno, uint32_t* out) {
  if ((imm & 0xFFF) != 0) {
    fprintf(stderr, "Error: (line %d): UI-type immediate %" PRId64 " must be a multiple of 4096\n", lineno, imm);
    had_error = 1;
    return false;
  }

  if (imm < INT32_MIN || imm > INT32_MAX) {
    fprintf(stderr, "Error: (line %d): UI-type immediate %" PRId64 " out of 32-bit range\n", lineno, imm);
    had_error = 1;
    return false;
  }

  const encoding_info_t *info = &encoding_table[op];
  uint32_t upper = (uint32_t)imm & 0xFFFFF000;

  *out = upper |
         ((uint32_t)(rd & 0x1F) << 7) |
         (uint32_t)(info->opcode & 0x7F);

  return true;
}

// Resolves any symbolic relocation, encodes the isntruction, and writes the resulting 32-bit word to the output stream
bool pass2_emit_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2,
                      int64_t imm, const char* label, reloc_kind_t reloc,
                      uint32_t addr, int lineno) {
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set before emitting instruction (line %d)\n", lineno);
    had_error = 1;
    return false;
  }

  int64_t val = imm;

  if (label && reloc != RELOC_NONE) {
    val = resolve_label(label, reloc, addr, lineno);
    if (had_error) return false;
  }
  else if (label && reloc == RELOC_NONE) {
    fprintf(stderr, "Error: (line %d): label provided without relocation kind\n", lineno);
    had_error = 1;
    return false;
  }

  uint32_t word = 0;
  if (!encode_instruction(op, fmt, rd, rs1, rs2, val, lineno, &word)) {
    // encoder already prints error and sets had_error
    return false;
  }

  if (fwrite(&word, sizeof(word), 1, outfile) != 1) {
    fprintf(stderr, "Error: (line %d): failed to write instruction\n", lineno);
    had_error = 1;
    return false;
  }
  return true;
}