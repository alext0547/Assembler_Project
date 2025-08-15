#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>
#define C_ANY 0xFF

#include "pass.h"
#include "ir.h"
#include "symtab.h"
#include "pass2.h"

static const encoding_info_t encoding_table[] = {
  [OP_ADD] = { 0x33, 0, 0 },
  [OP_SUB] = { 0x33, 0, 0x20 },
  [OP_SLL] = { 0x33, 0x1, 0 },
  [OP_SLT] = { 0x33, 0x2, 0 },
  [OP_SLTU] = { 0x33, 0x3, 0 },
  [OP_XOR] = { 0x33, 0x4, 0 },
  [OP_SRL] = { 0x33, 0x5, 0 },
  [OP_SRA] = { 0x33, 0x5, 0x20 },
  [OP_OR] = { 0x33, 0x6, 0 },
  [OP_AND] = { 0x33, 0x7, 0 },
  [OP_MUL] = { 0x33, 0, 0x1 },
  [OP_MULH] = { 0x33, 0x1, 0x1 },
  [OP_MULHSU] = { 0x33, 0x2, 0x1 },
  [OP_MULHU] = { 0x33, 0x3, 0x1 },
  [OP_DIV] = { 0x33, 0x4, 0x1 },
  [OP_DIVU] = { 0x33, 0x5, 0x1 },
  [OP_REM] = { 0x33, 0x6, 0x1 },
  [OP_REMU] = { 0x33, 0x7, 0x1 },
  [OP_ADDW] = { 0x3B, 0, 0 },
  [OP_SUBW] = { 0x3B, 0, 0x20 },
  [OP_SLLW] = { 0x3B, 0x1, 0 },
  [OP_SRLW] = { 0x3B, 0x5, 0 },
  [OP_SRAW] = { 0x3B, 0x5, 0x20 },
  [OP_MULW] = { 0x3B, 0, 0x1 },
  [OP_DIVW] = { 0x3B, 0x4, 0x1 },
  [OP_DIVUW] = { 0x3B, 0x5, 0x1 },
  [OP_REMW] = { 0x3B, 0x6, 0x1 },
  [OP_REMUW] = { 0x3B, 0x7, 0x1 },
  [OP_ADDI] = { 0x13, 0, 0 },
  [OP_SLLI] = { 0x13, 0x1, 0 }, 
  [OP_LW] = { 0x3, 0x2, 0 }, 
  [OP_SLTI] = { 0x13, 0x2, 0 }, 
  [OP_SLTIU] = { 0x13, 0x3, 0 }, 
  [OP_XORI] = { 0x13, 0x4, 0 }, 
  [OP_ORI] = { 0x13, 0x6, 0 }, 
  [OP_ANDI] = { 0x13, 0x7, 0 }, 
  [OP_SRLI] = { 0x13, 0x5, 0 }, 
  [OP_SRAI] = { 0x13, 0x5, 0x20 }, 
  [OP_JALR] = { 0x67, 0, 0 }, 
  [OP_LB] = { 0x3, 0, 0 }, 
  [OP_LH] = { 0x3, 0x1, 0 }, 
  [OP_LBU] = { 0x3, 0x4, 0 }, 
  [OP_LHU] = { 0x3, 0x5, 0 },
  [OP_ECALL] = { 0x73, 0, 0 },
  [OP_EBREAK] = { 0x73, 0, 0 },
  [OP_ADDIW] = { 0x1B, 0, 0 },
  [OP_SLLIW] = { 0x1B, 0x1, 0 },
  [OP_SRLIW] = { 0x1B, 0x5, 0 },
  [OP_SRAIW] = { 0x1B, 0x5, 0x20 },
  [OP_LWU] = { 0x3, 0x6, 0 },
  [OP_LD] = { 0x3, 0x3, 0 },
  [OP_AUIPC] = { 0x17, 0, 0 },
  [OP_LUI] = { 0x37, 0, 0 },
  [OP_SW] = { 0x23, 0x2, 0 },  
  [OP_SH] = { 0x23, 0x1, 0 },
  [OP_SB] = { 0x23, 0, 0 },
  [OP_SD] = { 0x23, 0x3, 0},
  [OP_BEQ] = { 0x63, 0, 0 },  
  [OP_BNE] = { 0x63, 0x1, 0 }, 
  [OP_BLT] = { 0x63, 0x4, 0 }, 
  [OP_BGE] = { 0x63, 0x5, 0 }, 
  [OP_BLTU] = { 0x63, 0x6, 0 }, 
  [OP_BGEU] = { 0x63, 0x7, 0 },
  [OP_JAL] = { 0x6F, 0, 0 }
};

static const c_encoding_info_t c_encoding_table[] = {
  [OP_C_SUB]  = { IF_CA, 0x1, 0x4, 0x1, 0x3, 0x0, C_RV_ANY, 1 },
  [OP_C_XOR]  = { IF_CA, 0x1, 0x4, 0x1, 0x3, 0x1, C_RV_ANY, 1 },
  [OP_C_OR]   = { IF_CA, 0x1, 0x4, 0x1, 0x3, 0x2, C_RV_ANY, 1 },
  [OP_C_AND]  = { IF_CA, 0x1, 0x4, 0x1, 0x3, 0x3, C_RV_ANY, 1 },
  [OP_C_SUBW] = { IF_CA, 0x1, 0x4, 0x1, 0x1, 0x0, C_RV_64, 1 },
  [OP_C_ADDW] = { IF_CA, 0x1, 0x4, 0x1, 0x1, 0x1, C_RV_64, 1 },
  [OP_C_ADD]    = { IF_CR, 0x2, 0x4, 0x1, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_JALR]   = { IF_CR, 0x2, 0x4, 0x1, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_EBREAK] = { IF_CR, 0x2, 0x4, 0x1, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_JR]     = { IF_CR, 0x2, 0x4, 0x0, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_MV]     = { IF_CR, 0x2, 0x4, 0x0, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_ADDI]   = { IF_CI, 0x1, 0x0, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 }, 
  [OP_C_ADDIW]  = { IF_CI, 0x1, 0x1, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_SRLI]   = { IF_CI, 0x1, 0x4, C_ANY, 0x0, C_ANY, C_RV_ANY, 1 },
  [OP_C_SRAI]   = { IF_CI, 0x1, 0x4, C_ANY, 0x1, C_ANY, C_RV_ANY, 1 },
  [OP_C_ANDI]   = { IF_CI, 0x1, 0x4, C_ANY, 0x2, C_ANY, C_RV_ANY, 1 },
  [OP_C_LUI]    = { IF_CI, 0x1, 0x3, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LWSP]   = { IF_CI,  0x2, 0x2, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LDSP]   = { IF_CI,  0x2, 0x3, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_LW]     = { IF_CL, 0x0, 0x2, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LD]     = { IF_CL, 0x0, 0x3, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_SW]     = { IF_CS, 0x0, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SD]     = { IF_CS, 0x0, 0x7, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_BEQZ]   = { IF_CB, 0x1, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_BNEZ]   = { IF_CB, 0x1, 0x7, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_JAL]    = { IF_CJ, 0x1, 0x1, C_ANY, C_ANY, C_ANY, C_RV_32, 1 },
  [OP_C_J]      = { IF_CJ, 0x1, 0x5, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SWSP]   = { IF_CSS, 0x2, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SDSP]   = { IF_CSS, 0x2, 0x7, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
};

static FILE* outfile;
static int had_error;
static section_t out_section;
static bool seen_text;

// Initializes internal state and prepares for the second pass
void pass2_initialize(void) {
  pass_num = 2;
  had_error = 0;
  outfile = NULL;
  out_section = SEC_TEXT;
  seen_text = false;
}

// Changes had_error to 1 if an error was detected elsewhere (not in pass2.c)
void pass2_report_error(void) {
  had_error = 1;
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

// Builds a 16-bit tag mask for a compressed op from its fixed metadata
static inline uint16_t c_build_tags(const c_encoding_info_t* i) {
  uint16_t w = 0;

  w |= ((uint16_t)(i->funct3 & 0x7)) << 13;
  w |= (uint16_t)(i->quadrant & 0x3);

  if (i->op12 != C_ANY) w |= ((uint16_t)(i->op12 & 0x1)) << 12;
  if (i->op11_10 != C_ANY) w |= ((uint16_t)(i->op11_10 & 0x3)) << 10;
  if (i->op6_5 != C_ANY) w |= ((uint16_t)(i->op6_5 & 0x3)) << 5;

  return w;
}

// Returns true if the given tag fields match the encoding's fixed bits
static inline bool c_tags_match(const c_encoding_info_t* i, uint8_t q, uint8_t f3, uint8_t b12,
                                uint8_t b11_10, uint8_t b6_5) {
  if (i->quadrant != q) return false;
  if (i->funct3 != f3) return false;
  if (i->op12 != C_ANY && i->op12 != (b12 & 1)) return false;
  if (i->op11_10 != C_ANY && i->op11_10 != (b11_10 & 3)) return false;
  if (i->op6_5 != C_ANY && i->op6_5 != (b6_5 & 3)) return false;
  return true;
}

// Error if a compressed instruction is used while C is disabled
static bool require_C(int lineno) {
  if (!pass_has_C()) {
    fprintf(stderr, "Error (line %d): C-extension instruction used but C not enabled\n", lineno);
    return false;
  }
  return true;
}

// Write exactly n bytes to outfile
static bool write_bytes(const void *buf, size_t n) {
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set\n");
    had_error = 1;
    return false;
  }

  const unsigned char *p = (const unsigned char *)buf;
  while (n > 0) {
    size_t wrote = fwrite(p, 1, n, outfile);
    if (wrote == 0) {
      perror("fwrite");
      had_error = 1;
      return false;
    }
    p += wrote;
    n -= wrote;
  }
  return true;
}

// Ensures the incorrect xlen isn't being used
static bool require_xlen(int need, int lineno) {
  if (pass_get_xlen() != need) {
    fprintf(stderr, "Error (line %d): instruction requires RV%d but assembler is RV%d\n", 
            lineno, need, pass_get_xlen());
    return false;
  }
  return true;
}

// Ensures the M extension is enabled when using M extended instructions
static bool require_M(int lineno) {
  if (!pass_has_M()) {
    fprintf(stderr, "Error (line %d): M-extension instruction used but M not enabled\n", lineno);
    return false;
  }
  return true;
}

// Ensures the correct xlen and M extension flag is set based on the instruction opcode
static bool require_for_op(opcode_t op, int lineno) {
  switch (op) {
    case OP_ADDW: case OP_SUBW: case OP_SLLW:
    case OP_SRLW: case OP_SRAW: case OP_ADDIW:
    case OP_SRLIW: case OP_SRAIW: case OP_SLLIW: 
    case OP_LWU: case OP_LD: case OP_SD:
      return require_xlen(64, lineno);
    
    case OP_MUL: case OP_MULH: case OP_MULHSU:
    case OP_MULHU: case OP_DIV: case OP_DIVU:
    case OP_REM: case OP_REMU: 
      return require_M(lineno);
    
    case OP_MULW: case OP_DIVW: case OP_DIVUW:
    case OP_REMW: case OP_REMUW:
      return require_M(lineno) && require_xlen(64, lineno);
    
    default:
      return true;
  }
}

// Emit n zero bytes efficiently for .space and alignment
static bool write_zeros(size_t n) {
  static const unsigned char Z[4096] = {0};
  while (n > 0) {
    size_t chunk = n > sizeof(Z) ? sizeof(Z) : n;
    if (!write_bytes(Z, chunk)) return false;
    n -= chunk;
  }
  return true;
}

// Emit a signed value as size bytes in little endian
static bool write_value_le(int64_t value, uint32_t size) {
  if (size != 1 && size != 2 && size != 4 && size != 8) {
    fprintf(stderr, "Error: Invalid size passed to write_value_le (%u)\n", size);
    had_error = 1;
    return false;
  }
  unsigned char b[8];
  for (uint32_t i = 0; i < size; i++) {
    b[i] = (unsigned char)((uint64_t)value >> (8*i));
  }
  return write_bytes(b, size);
}

// Encode a 32-bit word in little endian and write it
static bool write_word_le(uint32_t word) {
  unsigned char b[4];
  b[0] = (unsigned char)(word & 0xFF);
  b[1] = (unsigned char)((word >> 8) & 0xFF);
  b[2] = (unsigned char)((word >> 16) & 0xFF);
  b[3] = (unsigned char)((word >> 24) & 0xFF);
  return write_bytes(b, 4);
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

  // Shift-immediate group: RV32 (SLLI/SRLI/SRAI), RV64 (same), and RV64W (SLLIW/SRLIW/SRAIW)
  if (op == OP_SLLI || op == OP_SRLI || op == OP_SRAI ||
      op == OP_SLLIW || op == OP_SRLIW || op == OP_SRAIW) {

    bool is_word = (op == OP_SLLIW || op == OP_SRLIW || op == OP_SRAIW);
    uint32_t max_shamt = is_word ? 31u : (pass_get_xlen() == 64 ? 63u : 31u);

    if (imm < 0 || (uint64_t)imm > max_shamt) {
      fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 " (max %u)\n", lineno, imm, max_shamt);
      had_error = 1;
      return false;
    }

    uint32_t shamt = (uint32_t)imm;

    if (!is_word && pass_get_xlen() == 64) {
      // RV64 non-W shifts use funct6 in bits [31:26] and shamt[5:0] in [25:20]
      uint32_t f6 = info->funct7 & 0x3F;
      if (op == OP_SRAI) f6 = 0x10; // RV64 SRAI is funct6=010000b (not 0x20)
      *out = (f6 << 26) |
             ((shamt & 0x3F) << 20) |
             ((uint32_t)(rs1 & 0x1F) << 15) |
             ((uint32_t)(info->funct3 & 0x07) << 12) |
             ((uint32_t)(rd  & 0x1F) << 7) |
             (uint32_t)(info->opcode & 0x7F);
      return true;
    } 
    else {
      // RV32 and RV64W shifts use funct7 in [31:25] and shamt[4:0] in [24:20]
      *out = (((uint32_t)info->funct7 & 0x7F) << 25) |
             ((shamt & 0x1F) << 20) |
             ((uint32_t)(rs1 & 0x1F) << 15) |
             ((uint32_t)(info->funct3 & 0x07) << 12) |
             ((uint32_t)(rd  & 0x1F) << 7) |
             (uint32_t)(info->opcode & 0x7F);
      return true;
    }
  }
  else if (op == OP_EBREAK || op == OP_ECALL) {
    imm = (op == OP_EBREAK) ? 1 : 0;
    rs1 = 0;
    rd  = 0;
  }

  // Regular I-type immediates
  if (imm < -2048 || imm > 2047) {
    fprintf(stderr, "Error: (line %d): Invalid immediate %" PRId64 "\n", lineno, imm);
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

  if (!require_for_op(op, lineno)) {
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

  if (!write_word_le(word)) {
    fprintf(stderr, "Error: (line %d): failed to write instruction\n", lineno);
    // had_error is properly handled in write_word_le()
    return false;
  }
  return true;
}

// Switches to the requested output section while preventing returns to .data after .text
bool pass2_emit_section(section_t sect) {
  if (sect != SEC_TEXT && sect != SEC_DATA) {
    fprintf(stderr, "Error: pass 2 attempted to emit an invalid section\n");
    had_error = 1;
    return false;
  }

  if (seen_text && sect == SEC_DATA) {
    fprintf(stderr, "Error: cannot return to .data section after entering .text section\n");
    had_error = 1;
    return false;
  }

  if (sect == SEC_TEXT) seen_text = true;

  out_section = sect;
  return true;
}

// Writes a signed value to the .data section in little-endian with the specified byte size
bool pass2_emit_data(int64_t value, uint32_t size) {
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set\n");
    had_error = 1;
    return false;
  }

  if (out_section != SEC_DATA) {
    fprintf(stderr, "Error: data emitted outside .data section\n");
    had_error = 1;
    return false;
  }

  return write_value_le(value, size);
}

// Emits pad_bytes zero bytes (for .align/.space) in the current section
bool pass2_emit_align(uint32_t pad_bytes) {
  if (pad_bytes == 0) return true;
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set\n");
    had_error = 1;
    return false;
  }

  if (out_section != SEC_TEXT && out_section != SEC_DATA) {
    fprintf(stderr, "Error: invalid section for alignment/padding\n");
    had_error = 1;
    return false;
  }

  return write_zeros(pad_bytes);
}

// Walks the IR in order, switches sections as needed, and dispatches each entry to the appropriate emitter
bool pass2_run(void) {
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set\n");
    had_error = 1;
    return false;
  }

  size_t n = ir_count();
  for (size_t i = 0; i < n; i++) {
    const ir_entry_t* e = ir_get(i);

    if (!e) {
      fprintf(stderr, "Error: IR out of bounds at index %zu\n", i);
      had_error = 1;
      return false;
    }

    if (e->sect != out_section) {
      if (!pass2_emit_section(e->sect)) return false;
    }
    
    bool ok = false;
    switch (e->kind) {
      case IR_ALIGN:
        ok = pass2_emit_align(e->size);
        break;
      case IR_DATA:
        ok = pass2_emit_data(e->imm, e->size);
        break;
      case IR_INSTR:
        ok = pass2_emit_instr(e->op, e->fmt, e->rd, e->rs1, e->rs2, e->label ? 0 : e->imm,
                              e->label, e->reloc, e->addr, e->line);
        break;
      default:
        fprintf(stderr, "Error: unknown IR kind at index %zu (line %d)\n", i, e->line);
        had_error = 1;
        return false;
    }
    if (!ok) return false;
  }
  return !had_error;
}