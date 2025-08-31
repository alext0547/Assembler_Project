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
#include "c_rules.h"
#include "out.h"

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
  [OP_C_SLLI] = { IF_CI, 0x2, 0x0, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LI] = { IF_CI, 0x1, 0x2, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LW]     = { IF_CL, 0x0, 0x2, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_LD]     = { IF_CL, 0x0, 0x3, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_SW]     = { IF_CS, 0x0, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SD]     = { IF_CS, 0x0, 0x7, C_ANY, C_ANY, C_ANY, C_RV_64, 1 },
  [OP_C_BEQZ]   = { IF_CB, 0x1, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_BNEZ]   = { IF_CB, 0x1, 0x7, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_JAL]    = { IF_CJ, 0x1, 0x1, C_ANY, C_ANY, C_ANY, C_RV_32, 1 },
  [OP_C_J]      = { IF_CJ, 0x1, 0x5, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SWSP]   = { IF_CSS, 0x2, 0x6, C_ANY, C_ANY, C_ANY, C_RV_ANY, 1 },
  [OP_C_SDSP]   = { IF_CSS, 0x2, 0x7, C_ANY, C_ANY, C_ANY, C_RV_64, 1 }
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

// Current chosen size for instruction entries
static inline uint32_t pass2_node_size_now(const ir_entry_t* e) {
  if (ir_is_explicit_c(e)) return 2;
  if (e->forced_size == IR_SIZE_2) return 2;
  if (e->forced_size == IR_SIZE_4) return 4;
  if (e->has_c_choice) return 2;
  return 4;
}

// Return true if a compressed choice's displacement fits the encoding's limits
static bool pass2_choice_range_ok(const struct c_choice* ch, int32_t disp) {
  if (!ch) return false;
  switch (ch->op) {
    case OP_C_BEQZ:
    case OP_C_BNEZ:
      return (((disp & 1) == 0) && (disp >= -256) && (disp <= 254));
    case OP_C_J:
    case OP_C_JAL:
      return ((disp & 1) == 0) && (disp >= -2048) && (disp <= 2046);
    default:
      return true;
  }
}

// Compute signed byte displacement for a branch/jump given from/to addresses
static inline int32_t pass2_compute_disp(uint32_t from_addr, uint32_t to_addr) {
  return (int32_t)to_addr - (int32_t)from_addr;
}

// Look up absolute address of a label without emitting
static bool pass2_peek_label_addr(const char* label, uint32_t* out_addr) {
  if (!label || !out_addr) return false;
  int addr = search_sym(symtab, label);
  if (addr < 0) return false;

  *out_addr = (uint32_t)addr;
  return true;
}

// Validate all chosen C forms that depend on final displacements
static bool pass2_validate_compressed_ranges(section_t sect) {
  if (!pass_has_C() || !pass_has_auto_compress()) return false;

  bool changed = false;
  const size_t n = ir_count();
  for (size_t i = 0; i < n; i++) {
    ir_entry_t* e = ir_get(i);
    if (!e || e->sect != sect) continue;
    if (!e->has_c_choice) continue;

    const struct c_choice* ch = &e->choice;

    bool needs_pc_rel = false;
    switch (ch->op) {
      case OP_C_BEQZ:
      case OP_C_BNEZ:
      case OP_C_J:
      case OP_C_JAL:
        needs_pc_rel = true;
        break;
      default:
        break;
    }
    if (!needs_pc_rel) continue;

    uint32_t to_addr = 0;
    bool have_target = false;

    if (ch->label && ch->label[0]) {
      have_target = pass2_peek_label_addr(ch->label, &to_addr);
    }
    else if (ch->reloc == RELOC_BOFF || ch->reloc == RELOC_JTGT) {
      to_addr = (uint32_t)((int32_t)e->addr + (int32_t)ch->imm);
      have_target = true;
    }
    else {
      continue;
    }

    if (!have_target) {
      e->has_c_choice = false;
      changed = true;
      continue;
    }

    const int32_t disp = pass2_compute_disp(e->addr, to_addr);
    if (!pass2_choice_range_ok(ch, disp)) {
      e->has_c_choice = false;
      changed = true;
    }
    else {
      ((struct c_choice*)&e->choice)->imm = disp;
    }
  }
  return changed;
}

// Assign sequential addresses in a section using current node sizes
static void pass2_assign_addresses(section_t sect) {
  uint32_t pc = 0;
  const size_t n = ir_count();
  for (size_t i = 0; i < n; i++) {
    ir_entry_t* e = ir_get(i);
    if (!e || e->sect != sect) continue;

    switch (e->kind) {
      case IR_DATA:
        break;
      case IR_ALIGN: {
        uint32_t align = (uint32_t)e->imm;
        if (align > 1) {
          uint32_t aligned = (pc + (align - 1)) & ~(align - 1);
          e->size = aligned - pc;
          pc = aligned;
        }
        else {
          e->size = 0;
        }
        e->addr = pc;
        continue;
      }
      default:
        e->size = pass2_node_size_now(e);
        break;
    }
    e->addr = pc;
    pc += e->size;
  }
}

// Decide whether a node is eligible to attempt mapping
static bool pass2_node_can_attempt(const ir_entry_t* e) {
  if (!e) return false;
  if (!pass_has_C()) return false;
  if (!pass_has_auto_compress()) return false;
  if (e->kind == IR_DATA || e->kind == IR_ALIGN) return false;
  if (ir_is_explicit_c(e)) return false;
  if (ir_get_forced_size(e) == IR_SIZE_4) return false;
  if (!e->can_compress) return false;
  if (e->reloc != RELOC_NONE && e->reloc != RELOC_BOFF && e->reloc != RELOC_JTGT) return false;
  if (e->sect != SEC_TEXT) return false;
  return true;
}

// Scan IR and attach one compressed candidate per eligible instruction
static size_t pass2_plan_choices(ir_entry_t* list, size_t n) {
  if (!pass_has_C() || !pass_has_auto_compress()) return 0;
  size_t count = 0;
  for (size_t i = 0; i < n; i++) {
    ir_entry_t* e = &list[i];
    if (!pass2_node_can_attempt(e)) continue;
    
    struct c_choice ch = (struct c_choice){0};
    if (c_try_map(e, &ch)) {
      ir_set_c_choice(e, &ch);
      ++count;
    }
    else {
      e->has_c_choice = false;
    }
  }
  return count;
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

// Recompute addresses and validate chosen compressed forms to a fixed point
bool pass2_relayout_and_validate(section_t sect, unsigned max_iters) {
  if (max_iters == 0) max_iters = 1; 

  for (unsigned i = 0; i < max_iters; ++i) {
    pass2_assign_addresses(sect);
    bool changed = pass2_validate_compressed_ranges(sect);
    if (!changed) return true;
  }
  return false;
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

// Returns 0-7 for x8-x15 registers for compressed instructions
static inline int c_regp(int x) {
  if (x < 8 || x > 15) return -1;
  return x - 8;
}

// Returns true if the instruction format is of compressed type
static inline bool is_c_fmt(ir_fmt_t fmt) {
  if (fmt == IF_CR || fmt == IF_CI || fmt == IF_CL || fmt == IF_CS || fmt == IF_CSS || fmt == IF_CJ
      || fmt == IF_CB || fmt == IF_CA) return true;
  return false;
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

// Ensures the C extension is enabled and the compressed op is valid for this xlen
static bool require_c_for_op(opcode_t op, int lineno) {
  if (!require_C(lineno)) return false;

  const c_encoding_info_t* ci = &c_encoding_table[op];
  if (!ci->needs_c) {
    fprintf(stderr, "Internal error (line %d): opcode %d has no c-encoding table entry\n", lineno, (int)op);
    return false;
  }

  int xlen = pass_get_xlen();
  c_rv_mask_t have = (xlen == 64) ? C_RV_64 : C_RV_32;
  if ((ci->rv_req & have) == 0) {
    fprintf(stderr, "Error (line %d): compressed instruction not available for RV%d\n", lineno, xlen);
    return false;
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

// Set bits [hi:lo] of w to v
static inline uint16_t put(uint16_t w, int hi, int lo, uint32_t v) {
  if (hi < lo || hi > 15 || lo < 0) return w;
  uint32_t width = (uint32_t)(hi - lo + 1);
  uint32_t mask = ((1u << width) - 1u) << lo;
  uint32_t vbits = (v & ((1u << width) - 1u)) << lo;
  return (uint16_t)(((uint32_t)w & ~mask) | vbits);
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

// Pack a CR-type instruction into a 16 bit halfword
static bool encode_crtype(opcode_t op, int rd, int rs1, int rs2, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  if (op == OP_C_MV && (rd == 0 || rs2 == 0 || rs1 != 0)) {
    fprintf(stderr, "Error: (line %d): Invalid register value(s) for rd (x%d), rs1 (x%d), rs2 (x%d)\n",
            lineno, rd, rs1, rs2);
    had_error = 1; return false;
  }
  else if (op == OP_C_ADD && (rd == 0 || rs2 == 0 || rs1 != rd)) {
    fprintf(stderr, "Error: (line %d): Invalid register value(s) for rd (x%d), rs1 (x%d), rs2 (x%d)\n",
            lineno, rd, rs1, rs2);
    had_error = 1; return false;
  }
  else if (op == OP_C_JR) {
    if (rs1 == 0 || rs2 != 0) {
      fprintf(stderr, "Error: (line %d): Invalid register value(s) for rd (x%d), rs1 (x%d), rs2 (x%d)\n",
              lineno, rd, rs1, rs2);
      had_error = 1; return false;
    }
    rd = rs1;
  }
  else if (op == OP_C_JALR) {
    if (rs1 == 0 || rs2 != 0) {
      fprintf(stderr, "Error: (line %d): Invalid register value(s) for rd (x%d), rs1 (x%d), rs2 (x%d)\n",
              lineno, rd, rs1, rs2);
      had_error = 1; return false;
    }
    rd = rs1;
  }
  else if (op == OP_C_EBREAK && (rd != 0 || rs1 != 0 || rs2 != 0)) {
    fprintf(stderr, "Error: (line %d): Invalid register value(s) for rd (x%d), rs1 (x%d), rs2 (x%d)\n",
            lineno, rd, rs1, rs2);
    had_error = 1; return false;
  }

  if (op == OP_C_JR || op == OP_C_JALR) rd = rs1;

  const c_encoding_info_t* info = &c_encoding_table[op];
  *out = ((uint16_t)(info->funct3 & 0x7) << 13) |
         ((uint16_t)(info->op12 & 0x1)  << 12) |
         ((uint16_t)(rd & 0x1F)         << 7)  |
         ((uint16_t)(rs2 & 0x1F)        << 2)  |
         ((uint16_t)(info->quadrant & 0x3));
  return true;
}

// Pack a CA-type instruction into a 16 bit halfword
static bool encode_catype(opcode_t op, int rd, int rs1, int rs2, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  rd = c_regp(rd);
  rs1 = c_regp(rs1);
  rs2 = c_regp(rs2);
  if (rd == -1 || rs1 == -1 || rs2 == -1 || rd != rs1) {
    fprintf(stderr, "Error: (line %d): Invalid register(s) rd (x%d), rs1 (x%d), rs2 (x%d)", 
            lineno, rd, rs1, rs2);
    had_error = 1;
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];

  *out = ((uint16_t)(info->funct3 & 0x7) << 13) |
         ((uint16_t)(info->op12 & 0x1) << 12) |
         ((uint16_t)(info->op11_10 & 0x3) << 10) |
         ((uint16_t)(rd & 0x7) << 7) |
         ((uint16_t)(info->op6_5 & 0x3) << 5) |
         ((uint16_t)(rs2 & 0x7) << 2) |
         ((uint16_t)(info->quadrant & 0x3));
  return true;
}

// CI imm helper
static inline void put_imm5_0_ci(uint16_t* w, uint32_t u6) {
  *w = put(*w, 12, 12, (u6 >> 5) & 0x1);
  *w = put(*w, 6, 2, u6 & 0x1F);
}

// Pack a CI-type instruction into a 16 bit halfword
static bool encode_citype(opcode_t op, int rd, int rs1, int64_t imm, int lineno, uint16_t* out) {
  (void)rs1;

  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  const c_encoding_info_t* info = &c_encoding_table[op];
  const int xlen = pass_get_xlen();
  uint16_t w = c_build_tags(info);

  switch (op) {
    case OP_C_ADDI: {
      if (rd == 0 && imm == 0) {
        w = put(w, 11, 7, 0u);
        put_imm5_0_ci(&w, 0u);
        *out = w;
        return true;
      }
      if (rd == 0) {
        fprintf(stderr, "Error (line %d): c.addi rd cannot be x0 unless imm==0 (c.nop).\n", lineno);
        had_error = 1; return false;
      }
      if (imm < -32 || imm > 31) {
        fprintf(stderr, "Error (line %d): c.addi imm out of range (-32..31): %" PRId64 "\n", lineno, imm);
        had_error = 1; return false;
      }
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      put_imm5_0_ci(&w, (uint32_t)(imm & 0x3F));
      *out = w; return true;
    }

    case OP_C_ADDIW: {
      if (xlen != 64) { fprintf(stderr, "Error (line %d): c.addiw requires RV64.\n", lineno); had_error = 1; return false; }
      if (rd == 0) { fprintf(stderr, "Error (line %d): c.addiw rd cannot be x0.\n", lineno); had_error = 1; return false; }
      if (imm < -32 || imm > 31) { fprintf(stderr, "Error (line %d): c.addiw imm out of range (-32..31): %" PRId64 "\n", lineno, imm); had_error = 1; return false; }
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      put_imm5_0_ci(&w, (uint32_t)(imm & 0x3F));
      *out = w; return true;
    }

    case OP_C_LUI: {
      if (rd == 0 || rd == 2) { fprintf(stderr, "Error (line %d): c.lui rd cannot be x0 or x2.\n", lineno); had_error = 1; return false; }
      if (imm == 0 || imm < -32 || imm > 31) { fprintf(stderr, "Error (line %d): c.lui imm must be in [-32..-1] or [1..31].\n", lineno); had_error = 1; return false; }
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      put_imm5_0_ci(&w, (uint32_t)(imm & 0x3F));
      *out = w; return true;
    }

    case OP_C_SLLI: {
      if (rd == 0) { fprintf(stderr, "Error (line %d): c.slli rd cannot be x0.\n", lineno); had_error = 1; return false; }
      const uint32_t max_sh = (xlen == 64) ? 63u : 31u;
      if (imm < 0 || (uint64_t)imm > max_sh) {
        fprintf(stderr, "Error (line %d): c.slli shamt out of range (0..%u): %" PRId64 "\n", lineno, max_sh, imm);
        had_error = 1; return false;
      }
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      put_imm5_0_ci(&w, (uint32_t)imm & 0x3Fu);
      *out = w; return true;
    }

    case OP_C_SRLI:
    case OP_C_SRAI: {
      const int rp = c_regp(rd);
      if (rp < 0) { fprintf(stderr, "Error (line %d): %s requires rd in x8..x15.\n", lineno, (op == OP_C_SRLI) ? "c.srli" : "c.srai"); had_error = 1; return false; }
      const uint32_t max_sh = (xlen == 64) ? 63u : 31u;
      if (imm < 0 || (uint64_t)imm > max_sh) {
        fprintf(stderr, "Error (line %d): %s shamt out of range (0..%u): %" PRId64 "\n",
                lineno, (op == OP_C_SRLI) ? "c.srli" : "c.srai", max_sh, imm);
        had_error = 1; 
        return false;
      }
      w = put(w, 9, 7, (uint32_t)(rp & 0x7));
      put_imm5_0_ci(&w, (uint32_t)imm & 0x3Fu);
      *out = w; 
      return true;
    }

    case OP_C_ANDI: {
      const int rp = c_regp(rd);
      if (rp < 0) { fprintf(stderr, "Error (line %d): c.andi requires rd in x8..x15.\n", lineno); had_error = 1; return false; }
      if (imm < -32 || imm > 31) { fprintf(stderr, "Error (line %d): c.andi imm out of range (-32..31): %" PRId64 "\n", lineno, imm); had_error = 1; return false; }
      w = put(w, 9, 7, (uint32_t)(rp & 0x7));
      put_imm5_0_ci(&w, (uint32_t)(imm & 0x3F));
      *out = w; 
      return true;
    }

    case OP_C_LWSP: {
      if (rs1 != 2) { 
        fprintf(stderr, "Error (line %d): c.lwsp base must be x2 (sp), got x%d\n", lineno, rs1);
        had_error = 1; return false;
      }
      if (rd == 0) {
        fprintf(stderr, "Error (line %d): c.lwsp rd cannot be x0\n", lineno);
        had_error = 1; return false;
      }
      if (imm < 0 || (imm & 0x3) != 0 || imm > 252) {
        fprintf(stderr, "Error (line %d): c.lwsp offset must be 0..252 and multiple of 4 (got %" PRId64 ")\n",
                lineno, imm);
        had_error = 1; return false;
      }

      uint32_t u = (uint32_t)imm;
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      w = put(w, 12, 12, (u >> 5) & 0x1);
      w = put(w, 6,  4,  (u >> 2) & 0x7); 
      w = put(w, 3,  2,  (u >> 6) & 0x3);
      *out = w;
      return true;
    }

    case OP_C_LDSP: {
      // RV64 only, base must be sp (x2), rd != x0, uimm 0..504 and multiple of 8
      if (pass_get_xlen() != 64) {
        fprintf(stderr, "Error (line %d): c.ldsp requires RV64\n", lineno);
        had_error = 1; return false;
      }
      if (rs1 != 2) { 
        fprintf(stderr, "Error (line %d): c.ldsp base must be x2 (sp), got x%d\n", lineno, rs1);
        had_error = 1; return false;
      }
      if (rd == 0) {
        fprintf(stderr, "Error (line %d): c.ldsp rd cannot be x0\n", lineno);
        had_error = 1; return false;
      }
      if (imm < 0 || (imm & 0x7) != 0 || imm > 504) {
        fprintf(stderr, "Error (line %d): c.ldsp offset must be 0..504 and multiple of 8 (got %" PRId64 ")\n",
                lineno, imm);
        had_error = 1; return false;
      }
      uint32_t u = (uint32_t)imm;
      w = put(w, 12, 12, (u >> 5) & 0x1);
      w = put(w,  6,  5, (u >> 3) & 0x3);
      w = put(w,  4,  2, (u >> 6) & 0x7);
      *out = w;
      return true;
    }

    case OP_C_LI: {
      if (rd == 0) {
        fprintf(stderr, "Error (line %d): c.li rd cannot be x0.\n", lineno);
        had_error = 1; return false;
      }
      if (imm < -32 || imm > 31 || imm == 0) {
        fprintf(stderr, "Error (line %d): c.li imm must be in [-32..-1] or [1..31], got %" PRId64 "\n", lineno, imm);
        had_error = 1; return false;
      }
      w = put(w, 11, 7, (uint32_t)(rd & 0x1F));
      put_imm5_0_ci(&w, (uint32_t)(imm & 0x3F));
      *out = w;
      return true;
    }

    default:
      fprintf(stderr, "Error (line %d): encode_citype(): unhandled CI op.\n", lineno);
      had_error = 1; 
      return false;
  }
}

// Pack a CL-type instruction into a 16 bit halfword
static bool encode_cltype(opcode_t op, int rd, int rs1, int64_t imm, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  int rd_orig = rd, rs1_orig = rs1;
  int rdp = c_regp(rd), r1p = c_regp(rs1);
  if (rdp < 0 || r1p < 0) {
    fprintf(stderr, "Error (line %d): CL requires rd, rs1 in x8..x15 (got x%d, x%d)\n",
            lineno, rd_orig, rs1_orig);
    had_error = 1; 
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];
  uint16_t w = c_build_tags(info);
  uint32_t u = (uint32_t)imm;

  if (op == OP_C_LW) {
    if (imm < 0 || (u & 0x3) != 0 || u > 124) {
      fprintf(stderr, "Error (line %d): c.lw offset must be 0..124 and multiple of 4 (got %" PRId64 ")\n",
              lineno, imm);
      had_error = 1; 
      return false;
    }
    w = put(w, 12, 10, (u >> 3) & 0x7);
    w = put(w, 6,  6,  (u >> 2) & 0x1);
    w = put(w, 5,  5,  (u >> 6) & 0x1);
  } else if (op == OP_C_LD) {
    if (imm < 0 || (u & 0x7) != 0 || u > 248) {
      fprintf(stderr, "Error (line %d): c.ld offset must be 0..248 and multiple of 8 (got %" PRId64 ")\n",
              lineno, imm);
      had_error = 1; 
      return false;
    }
    w = put(w, 12, 10, (u >> 3) & 0x7);
    w = put(w, 6,  5,  (u >> 6) & 0x3);
  } else {
    fprintf(stderr, "Error (line %d): encode_cltype only handles C.LW/C.LD\n", lineno);
    had_error = 1; 
    return false;
  }

  w = put(w, 9, 7, (uint32_t)r1p & 0x7);
  w = put(w, 4, 2, (uint32_t)rdp & 0x7);

  *out = w;
  return true;
}

// Pack a CS-type instruction into a 16 bit halfword
static bool encode_cstype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  int rs1_orig = rs1, rs2_orig = rs2;
  int r2p = c_regp(rs2), r1p = c_regp(rs1);
  if (r1p < 0 || r2p < 0) {
    fprintf(stderr, "Error (line %d): CS requires rs1, rs2 in x8..x15 (got x%d, x%d)\n",
            lineno, rs1_orig, rs2_orig);
    had_error = 1; 
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];
  uint16_t w = c_build_tags(info);
  uint32_t u = (uint32_t)imm;

  if (op == OP_C_SW) {
    if (imm < 0 || (u & 0x3) != 0 || u > 124) {
      fprintf(stderr, "Error (line %d): c.sw offset must be 0..124 and multiple of 4 (got %" PRId64 ")\n",
              lineno, imm);
      had_error = 1; 
      return false;
    }
    w = put(w, 12, 10, (u >> 3) & 0x7);
    w = put(w, 6,  6,  (u >> 2) & 0x1);
    w = put(w, 5,  5,  (u >> 6) & 0x1);
  } else if (op == OP_C_SD) {
    if (imm < 0 || (u & 0x7) != 0 || u > 248) {
      fprintf(stderr, "Error (line %d): c.sd offset must be 0..248 and multiple of 8 (got %" PRId64 ")\n",
              lineno, imm);
      had_error = 1; 
      return false;
    }
    w = put(w, 12, 10, (u >> 3) & 0x7);
    w = put(w, 6,  5,  (u >> 6) & 0x3);
  } else {
    fprintf(stderr, "Error (line %d): encode_cstype only handles C.SW/C.SD\n", lineno);
    had_error = 1; 
    return false;
  }

  w = put(w, 9, 7, (uint32_t)r1p & 0x7);
  w = put(w, 4, 2, (uint32_t)r2p & 0x7);

  *out = w;
  return true;
}

// Pack a CSS-type instruction into a 16 bit halfword
static bool encode_csstype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }
  if (rs1 != 2) {
    fprintf(stderr, "Error (line %d): CSS base must be x2 (sp), got x%d\n", lineno, rs1);
    had_error = 1; 
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];
  uint16_t w = c_build_tags(info);
  uint32_t u = (uint32_t)imm;

  if (op == OP_C_SWSP) {
    if (imm < 0 || (u & 0x3) != 0 || u > 252) {
      fprintf(stderr, "Error (line %d): c.swsp offset must be 0..252 and multiple of 4 (got %" PRId64 ")\n", lineno, imm);
      had_error = 1; return false;
    }
    w = put(w, 12, 12, (u >> 7) & 0x1);
    w = put(w,  6,  2, (u >> 2) & 0x1F);
    w = put(w, 11,  7, (uint32_t)(rs2 & 0x1F));
  } else if (op == OP_C_SDSP) {
    if (imm < 0 || (u & 0x7) != 0 || u > 504) {
      fprintf(stderr, "Error (line %d): c.sdsp offset must be 0..504 and multiple of 8 (got %" PRId64 ")\n", lineno, imm);
      had_error = 1; return false;
    }
    w = put(w, 12, 10, (u >> 6) & 0x7);
    w = put(w,  6,  2, (u >> 3) & 0x1F);
    w = put(w, 11,  7, (uint32_t)(rs2 & 0x1F));
  } else {
    fprintf(stderr, "Error (line %d): encode_csstype only handles C.SWSP/C.SDSP\n", lineno);
    had_error = 1; return false;
  }

  *out = w;
  return true;
}

// Pack a CB-type instruction into a 16 bit halfword
static bool encode_cbtype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  (void)rs2;
  int r1p = c_regp(rs1);
  if (r1p < 0) {
    fprintf(stderr, "Error (line %d): %s requires rs1 in x8..x15 (got x%d)\n",
            lineno, (op == OP_C_BEQZ ? "c.beqz" : "c.bnez"), rs1);
    had_error = 1; 
    return false;
  }

  if ((imm & 1) != 0 || imm < -256 || imm > 254) {
    fprintf(stderr, "Error (line %d): CB offset must be even and in [-256, 254], got %" PRId64 "\n",
            lineno, imm);
    had_error = 1; 
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];
  uint16_t w = c_build_tags(info);
  int32_t off = (int32_t)imm >> 1;
  uint32_t u = (uint32_t)(off & 0xFF);

  w = put(w, 12, 12, (u >> 7) & 0x1);
  w = put(w, 11, 10, (u >> 5) & 0x3);
  w = put(w, 9, 7, (uint32_t)r1p & 0x7);
  w = put(w, 6, 3, u & 0xF);
  w = put(w, 2, 2, (u >> 4) & 0x1);

  *out = w;
  return true;
}

// Pack a CJ-type instruction into a 16 bit halfword
static bool encode_cjtype(opcode_t op, int rs1, int rs2, int64_t imm, int lineno, uint16_t* out) {
  if (!require_c_for_op(op, lineno)) { had_error = 1; return false; }

  (void)rs1; (void)rs2;
  if ((imm & 1) != 0 || imm < -2048 || imm > 2046) {
    fprintf(stderr, "Error (line %d): CJ offset must be even and in [-2048, 2046], got %" PRId64 "\n",
            lineno, imm);
    had_error = 1; 
    return false;
  }

  const c_encoding_info_t* info = &c_encoding_table[op];
  uint16_t w = c_build_tags(info);
  uint32_t off = (int32_t)imm >> 1;
  uint32_t u = (uint32_t)(off & 0x7FF);

  w = put(w, 12, 12, (u >> 10) & 0x1);
  w = put(w, 11, 11, (u >>  3) & 0x1);
  w = put(w, 10,  9, (u >>  8) & 0x3);
  w = put(w,  8,  8, (u >>  9) & 0x1);
  w = put(w,  7,  7, (u >>  6) & 0x1);
  w = put(w,  6,  6, (u >>  7) & 0x1);
  w = put(w,  5,  3, (u >>  1) & 0x7);
  w = put(w,  2,  2, (u >>  5) & 0x1);

  *out = w;
  return true;
}

// Dispatches to the correct RV16I encoder for the given format
static bool c_encode_instruction(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int64_t imm, 
                                   int lineno, uint16_t* out) {
  switch (fmt) {
    case IF_CA:
      return encode_catype(op, rd, rs1, rs2, lineno, out);
    case IF_CI:
      return encode_citype(op, rd, rs1, imm, lineno, out);
    case IF_CR:
      return encode_crtype(op, rd, rs1, rs2, lineno, out);
    case IF_CS:
      return encode_cstype(op, rs1, rs2, imm, lineno, out);
    case IF_CSS: 
      return encode_csstype(op, rs1, rs2, imm, lineno, out);
    case IF_CB:
      return encode_cbtype(op, rs1, rs2, imm, lineno, out);
    case IF_CJ:
      return encode_cjtype(op, rs1, rs2, imm, lineno, out);
    case IF_CL:
      return encode_cltype(op, rd, rs1, imm, lineno, out);
    default:
      fprintf(stderr, "Error (line %d): Unknown C format\n", lineno);
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

// Build the opcode/operands to emit, preferring the compressed choice when present
static bool c_materialize(const ir_entry_t* e, opcode_t* op, ir_fmt_t* fmt, int* rd,
                          int* rs1, int* rs2, int64_t* imm, const char** label, reloc_kind_t* reloc) {
  if (!e || !op || !fmt || !rd || !rs1 || !rs2 || !imm || !label || !reloc) return false;

  if (e->has_c_choice) {
    const struct c_choice* ch = &e->choice;
    *op = ch->op;
    *fmt = ch->fmt;
    *rd = ch->rd;
    *rs1 = ch->rs1;
    *rs2 = ch->rs2;
    *imm = ch->imm;
    *label = ch->label;
    *reloc = ch->reloc;

    if (ch->op == OP_C_BEQZ || ch->op == OP_C_BNEZ || ch->op == OP_C_J || ch->op == OP_C_JAL) {
      *label = NULL;
      *reloc = RELOC_NONE;
    }
    return true;
  }

  *op = e->op;
  *fmt = e->fmt;
  *rd = e->rd;
  *rs1 = e->rs1;
  *rs2 = e->rs2;
  *imm = e->imm;
  *label = e->label;
  *reloc = e->reloc;
  return true;
}

// Resolves any symbolic relocation, encodes the instruction, and writes the resulting 32-bit word to the output stream
bool pass2_emit_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2,
                      int64_t imm, const char* label, reloc_kind_t reloc,
                      uint32_t addr, int lineno) {
  if (had_error) return false;
  if (!outfile) {
    fprintf(stderr, "Error: no output file set before emitting instruction (line %d)\n", lineno);
    had_error = 1;
    return false;
  }

  if (is_c_fmt(fmt)) {
    if (!require_c_for_op(op, lineno)) return false;

    int64_t use_imm = imm;
    if (label && reloc != RELOC_NONE) {
      use_imm = resolve_label(label, reloc, addr, lineno);
      if (had_error) return false;
    }
    else if (label && reloc == RELOC_NONE) {
      fprintf(stderr, "Error: (line %d): label provided without relocation kind\n", lineno);
      had_error = 1;
      return false;
    }
    
    uint16_t half = 0;
    if (!c_encode_instruction(op, fmt, rd, rs1, rs2, use_imm, lineno, &half)) return false;
    return write_half_le(half);
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

// Emit one IR node, honoring explicit C or an auto compressed form
bool pass2_emit_node(const ir_entry_t* e) {
  if (!e) { return false; }

  opcode_t op; ir_fmt_t fmt;
  int rd, rs1, rs2;
  int64_t imm; const char* label;
  reloc_kind_t reloc;

  if (!c_materialize(e, &op, &fmt, &rd, &rs1, &rs2, &imm, &label, &reloc)) return false;

  int64_t use_imm = label ? 0 : imm;
  return pass2_emit_instr(op, fmt, rd, rs1, rs2, use_imm, label, reloc, e->addr, e->line);
}

// Switches to the requested output section while preventing returns to .data after .text
bool pass2_emit_section(section_t sect) {
  if (sect != SEC_TEXT && sect != SEC_DATA) {
    fprintf(stderr, "Error: pass 2 attempted to emit an invalid section\n");
    had_error = 1;
    return false;
  }

  out_begin_section(sect);
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

  pass2_assign_addresses(SEC_DATA);

  if (pass_has_C() && pass_has_auto_compress()) {
    (void)pass2_plan_choices(ir_get(0), ir_count());
    (void)pass2_relayout_and_validate(SEC_TEXT, 8);
  }
  else {
    pass2_assign_addresses(SEC_TEXT);
  }

  if (ir_count() > 0) {
    const ir_entry_t* first = ir_get(0);
    if (!pass2_emit_section(first->sect)) return false;
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
        ok = pass2_emit_node(e);
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