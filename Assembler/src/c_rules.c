#include <stdbool.h>
#include "ir.h"
#include <stdint.h>
#include "pass.h"

// addi rd, rd, imm -> c.addi rd, imm
static bool c_map_addi_self(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_ADDI) return false;
  if (n->rd != n->rs1) return false;
  if (n->imm < -32 || n->imm > 31) return false;
  if (n->rd == 0) return false;

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = 0;
  out->imm = n->imm;
  out->op = OP_C_ADDI;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// addi rd, x0, imm -> c.li rd, imm (via c.addi)
static bool c_map_li_via_addi(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rs1 != 0 || n->rd == 0) return false;
  if (n->imm < -32 || n->imm > 31) return false;
  if (n->op != OP_ADDI) return false;

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = 0;
  out->rs2 = 0;
  out->imm = n->imm;
  out->op = OP_C_LI;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// addi x0, x0, 0 -> c.nop
static bool c_map_nop(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd != 0 || n->rs1 != 0 || n->imm != 0) return false;
  if (n->op != OP_ADDI) return false;
  
  out->fmt = IF_CI;
  out->rd = 0;
  out->rs1 = 0;
  out->rs2 = 0;
  out->imm = 0;
  out->op = OP_C_ADDI;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// slli rd, rd, sh -> c.slli
static bool c_map_slli(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd == 0) return false;
  if (n->rd != n->rs1) return false;
  if (n->op != OP_SLLI) return false;
  uint32_t max_sh = (pass_get_xlen() == 64) ? 63u : 31u;
  if (n->imm < 0 || (uint64_t)n->imm > max_sh) return false;

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = 0;
  out->imm = n->imm;
  out->op = OP_C_SLLI;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// srli/srai rd, rd, sh -> c.srli/c.srai (prime regs)
static bool c_map_srxi_prime(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd != n->rs1) return false;
  if (n->op != OP_SRAI && n->op != OP_SRLI) return false;
  if (n->rd < 8 || n->rd > 15) return false;
  uint32_t max_sh = (pass_get_xlen() == 64) ? 63u : 31u;
  if (n->imm < 0 || (uint64_t)n->imm > max_sh) return false;

  if (n->op == OP_SRAI) out->op = OP_C_SRAI;
  else out->op = OP_C_SRLI;

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = 0;
  out->imm = n->imm;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// andi rd, rd, imm -> c.andi (prime regs)
static bool c_map_andi_prime(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd != n->rs1) return false;
  if (n->rd < 8 || n->rd > 15) return false;
  if (n->imm < -32 || n->imm > 31) return false;
  if (n->op != OP_ANDI) return false;

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = 0;
  out->imm = n->imm;
  out->op = OP_C_ANDI;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// add rd, x0, rs2 -> c.mv rd, rs2
static bool c_map_mv(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd == 0 || n->rs2 == 0 || n->rs1 != 0) return false;
  if (n->op != OP_ADD) return false;

  out->fmt = IF_CR;
  out->rd = n->rd;
  out->rs1 = 0;
  out->rs2 = n->rs2;
  out->imm = 0;
  out->op = OP_C_MV;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// add rd, rd, rs2 -> c.add rd, rs2
static bool c_map_add_cr(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_ADD) return false;
  if (n->rd != n->rs1) return false;
  if (n->rd == 0 || n->rs2 == 0) return false;

  out->fmt = IF_CR;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = n->rs2;
  out->imm = 0;
  out->op = OP_C_ADD;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// jalr x0, rs1, 0 -> c.jr rs1
static bool c_map_jr(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd != 0 || n->imm != 0 || n->rs1 == 0) return false;
  if (n->op != OP_JALR) return false;

  out->fmt = IF_CR;
  out->rd = 0;
  out->rs1 = n->rs1;
  out->rs2 = 0;
  out->imm = 0;
  out->op = OP_C_JR;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// jalr x1, rs1, 0 -> c.jalr rs1
static bool c_map_jalr(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->rd != 1 || n->rs1 == 0 || n->imm != 0) return false;
  if (n->op != OP_JALR) return false;

  out->fmt = IF_CR;
  out->rd = 1;
  out->rs1 = n->rs1;
  out->rs2 = 0;
  out->imm = 0;
  out->op = OP_C_JALR;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// ebreak -> c.ebreak
static bool c_map_ebreak(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_EBREAK) return false;

  out->fmt = IF_CR;
  out->rd = 0;
  out->rs1 = 0;
  out->rs2 = 0;
  out->imm = 0;
  out->op = OP_C_EBREAK;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// sub/xor/or/and rd’, rd’, rs2’ -> CA ALU (prime regs)
static bool c_map_ca_alu(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_SUB && n->op != OP_XOR && n->op != OP_OR && n->op != OP_AND) return false;
  if (n->rd != n->rs1) return false;
  if (n->rs2 < 8 || n->rs2 > 15 || n->rd < 8 || n->rd > 15) return false;
  if (n->op == OP_SUB) out->op = OP_C_SUB;
  else if (n->op == OP_XOR) out->op = OP_C_XOR;
  else if (n->op == OP_OR) out->op = OP_C_OR;
  else out->op = OP_C_AND;

  out->fmt = IF_CA;
  out->rd = n->rd;
  out->rs1 = n->rs1;
  out->rs2 = n->rs2;
  out->imm = 0;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// addw/subw rd’, rd’, rs2’ -> CAW ALU (RV64C)
static bool c_map_caw_alu64(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (pass_get_xlen() != 64) return false;
  if (n->op != OP_SUBW && n->op != OP_ADDW) return false;
  if (n->rd != n->rs1) return false;
  if (n->rd < 8 || n->rd > 15 || n->rs2 < 8 || n->rs2 > 15) return false;
  if (n->op == OP_SUBW) out->op = OP_C_SUBW;
  else out->op = OP_C_ADDW;

  out->fmt = IF_CA;
  out->rd = n->rd;
  out->rs1 = n->rd;
  out->rs2 = n->rs2;
  out->imm = 0;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// lw/ld rd’, u(xrs1’) -> c.lw/c.ld
static bool c_map_cl_load(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_LW && n->op != OP_LD) return false;
  if (n->rd < 8 || n->rd > 15 || n->rs1 < 8 || n->rs1 > 15) return false;
  if (n->imm < 0) return false;
  if (n->op == OP_LW) {
    if ((n->imm & 0x3) != 0 || n->imm > 124) return false;
    out->op = OP_C_LW;
  }
  else {
    if (pass_get_xlen() != 64) return false;
    if ((n->imm & 0x7) != 0 || n->imm > 248) return false;
    out->op = OP_C_LD;
  }

  out->fmt = IF_CL;
  out->rd = n->rd;
  out->rs1 = n->rs1;
  out->rs2 = 0;
  out->imm = n->imm;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// sw/sd rs2’, u(xrs1’) -> c.sw/c.sd
static bool c_map_cs_store(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_SW && n->op != OP_SD) return false;
  if (n->rs2 < 8 || n->rs2 > 15 || n->rs1 < 8 || n->rs1 > 15) return false;
  if (n->imm < 0) return false;
  if (n->op == OP_SW) {
    if ((n->imm & 0x3) != 0 || n->imm > 124) return false;
    out->op = OP_C_SW;
  }
  else {
    if ((n->imm & 0x7) != 0 || n->imm > 248) return false;
    out->op = OP_C_SD;
  }

  out->fmt = IF_CS;
  out->rd = 0;
  out->rs1 = n->rs1;
  out->rs2 = n->rs2;
  out->imm = n->imm;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// lw/ld rd, u(sp) -> c.lwsp/c.ldsp
static bool c_map_ci_sp_load(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_LW && n->op != OP_LD) return false;
  if (n->rd == 0 || n->rs1 != 2) return false;
  if (n->imm < 0) return false;
  if (n->op == OP_LW) {
    if ((n->imm & 0x3) != 0 || n->imm > 252) return false;
    out->op = OP_C_LWSP;
  }
  else {
    if (pass_get_xlen() != 64) return false;
    if ((n->imm & 0x7) != 0 || n->imm > 504) return false;
    out->op = OP_C_LDSP;
  }

  out->fmt = IF_CI;
  out->rd = n->rd;
  out->rs1 = 2;
  out->rs2 = 0;
  out->imm = n->imm;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// sw/sd rs2, u(sp) -> c.swsp/c.sdsp
static bool c_map_css_sp_store(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_SW && n->op != OP_SD) return false;
  if (n->rs2 == 0 || n->rs1 != 2) return false;
  if (n->imm < 0) return false;
  if (n->op == OP_SW) {
    if ((n->imm & 0x3) != 0 || n->imm > 252) return false;
    out->op = OP_C_SWSP;
  }
  else {
    if ((n->imm & 0x7) != 0 || n->imm > 504) return false;
    out->op = OP_C_SDSP;
  }

  out->fmt = IF_CSS;
  out->rd = 0;
  out->rs1 = 2;
  out->rs2 = n->rs2;
  out->imm = n->imm;
  out->label = NULL;
  out->reloc = RELOC_NONE;
  return true;
}

// beq/bne rs1’, x0, off -> c.beqz/c.bnez
static bool c_map_cb_branchz(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_BEQ && n->op != OP_BNE) return false;
  if (n->rs2 != 0) return false;
  if (n->rs1 < 8 || n->rs1 > 15) return false;
  if ((n->imm & 1) != 0) return false;
  if (n->imm < -256 || n->imm > 254) return false;
  if (n->op == OP_BEQ) out->op = OP_C_BEQZ;
  else out->op = OP_C_BNEZ;

  out->fmt = IF_CB;
  out->rd = 0;
  out->rs1 = n->rs1;
  out->rs2 = 0;
  out->imm = n->imm;
  out->label = n->label;
  out->reloc = n->reloc;
  return true;
}

// jal x0/off -> c.j ; jal x1/off -> c.jal (RV32C)
static bool c_map_cj_jump(const ir_entry_t* n, struct c_choice* out) {
  if (!n || !out) return false;
  if (n->op != OP_JAL) return false;
  if (n->rd != 0 && n->rd != 1) return false;
  if (n->rd == 1 && pass_get_xlen() != 32) return false;
  if ((n->imm & 1) != 0) return false;
  if (n->imm < -2048 || n->imm > 2046) return false;
  if (n->rd == 0) out->op = OP_C_J;
  else out->op = OP_C_JAL;

  out->fmt = IF_CJ;
  out->rd = n->rd;
  out->rs1 = 0;
  out->rs2 = 0;
  out->imm = n->imm;
  out->label = n->label;
  out->reloc = n->reloc;
  return true;
}

// Try to build a compressed replacement for a 32-bit IR node; returns true on success.
bool c_try_map(const ir_entry_t* n, struct c_choice* out) {
  return c_map_addi_self(n, out) || c_map_li_via_addi(n, out) || c_map_nop(n, out) || 
         c_map_slli(n, out) || c_map_srxi_prime(n, out) || c_map_andi_prime(n, out) ||
         c_map_mv(n, out) || c_map_add_cr(n, out) || c_map_jr(n, out) || c_map_jalr(n, out) ||
         c_map_ebreak(n, out) || c_map_ca_alu(n, out) || c_map_caw_alu64(n, out) ||
         c_map_cl_load(n, out) || c_map_cs_store(n, out) || c_map_ci_sp_load(n, out) ||
         c_map_css_sp_store(n, out) || c_map_cb_branchz(n, out) || c_map_cj_jump(n, out);
}