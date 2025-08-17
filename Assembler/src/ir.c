#include <stdint.h>
#include "ir.h"
#include "pass.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

static const char* const SECTION_NAMES[] = { ".text", ".data" };
static const char* const KIND_NAMES[] = { "INSTR", "DATA", "ALIGN" };
static const char* const FMT_NAMES[] = { "R", "I", "S", "B", "J", "UI" };
static const char* const RELOC_NAMES[] = { "NONE", "HI20", "LO12", "BOFF", "JTGT" };
static const char* const OPCODE_NAMES[] = { 
  "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", "MUL", "MULH", "MULHSU", "MULHU", "DIV",
  "DIVU", "REM", "REMU", "ADDW", "SUBW", "SLLW", "SRLW", "SRAW", "MULW", "DIVW", "DIVUW", "REM", "REMW",
  "ADDI", "SLLI", "LW", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SRLI", "SRAI", "JALR", "LB", "LH", "LBU", "LHU",
  "ECALL","EBREAK", "ADDIW", "SLLIW", "SRLIW", "SRAIW", "LWU", "LD",
  "AUIPC", "LUI",
  "SW", "SH","SB", "SD",
  "BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU",
  "JAL",
  "C_ADD", "C_SUB", "C_XOR", "C_OR", "C_AND", "C_SUBW", "C_ADDW", "C_JALR",
  "C_ADDI", "C_ADDW", "C_SRLI", "C_SRAI", "C_LW", "C_LD",
  "C_BEQZ", "C_BNEZ",
  "C_JAL",
  "C_SD", "C_SW",
  "C_LUI" };

// Bounds check an enum/index into a name table and return "?" if it's out of range
static inline const char* safe(const char* const* a, size_t n, size_t idx) { return (idx < n) ? a[idx] : "?"; }

// Maps either a section_t, ir_kind_t, ir_fmt_t, reloc_kind_t, opcode_t value to its printable name
const char* sect_name(section_t s) { return safe(SECTION_NAMES, sizeof SECTION_NAMES/sizeof *SECTION_NAMES, (size_t)s); }
const char* kind_name(ir_kind_t k) { return safe(KIND_NAMES, sizeof KIND_NAMES/sizeof *KIND_NAMES, (size_t)k); }
const char* fmt_name(ir_fmt_t f) { return safe(FMT_NAMES, sizeof FMT_NAMES/sizeof *FMT_NAMES, (size_t)f); }
const char* reloc_name(reloc_kind_t r) { return safe(RELOC_NAMES, sizeof RELOC_NAMES/sizeof *RELOC_NAMES, (size_t)r); }
const char* op_name(opcode_t o) { return safe(OPCODE_NAMES, sizeof OPCODE_NAMES/sizeof *OPCODE_NAMES, (size_t)o); }

static ir_entry_t *buf = NULL;
static size_t len = 0, cap = 0;

// Initializes the IR buffer to an empty state
void ir_init(void) {
  ir_clear();
  buf = NULL;
  len = 0;
  cap = 0;
}

// Grows the IR buffer (using doubling strategy) so that at least 'need' more entries fit
// Returns 1 if successful, otherwise 0
static int ensure_capacity(size_t need) {
  if (len + need <= cap) return 1;
  size_t new_cap = cap ? cap : 128;
  while (len + need > new_cap) new_cap *= 2;
  void *p = realloc(buf, new_cap * sizeof *buf);
  if (!p) return 0;
  buf = (ir_entry_t*)p;
  cap = new_cap;
  return 1;
}

// Append an instruction IR entry (duplicates label string if provided)
void ir_append_instr(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int64_t imm, const char* label, 
          reloc_kind_t rk, section_t sect, uint32_t addr, uint32_t size, int line) {
          if (!ensure_capacity(1)) {
            fprintf(stderr, "Error: out of memory growing buffer (ir.c, line 57)\n");
            return;
          }

          ir_entry_t e = (ir_entry_t){0};
          e.kind = IR_INSTR;
          e.op = op;
          e.fmt = fmt;
          e.rd = rd;
          e.rs1 = rs1;
          e.rs2 = rs2;
          e.imm = imm;
          e.reloc = rk;
          e.sect = sect;
          e.addr = addr;
          e.size = size;
          e.line = line;

          if (label) {
            char* label_copy = strdup(label);
            if (!label_copy) {
              fprintf(stderr, "Error: label failed to duplicate (ir.c, line 77)\n");
              return;
            }
            e.label = label_copy;
            if (e.reloc == RELOC_NONE) {
              fprintf(stderr, "Warning: label present but reloc == RELOC_NONE (ir.c, line 83)\n");
            }
          }
          else {
            e.label = NULL;
            e.reloc = RELOC_NONE;
          }

          const bool is_c = (fmt == IF_CR || fmt == IF_CA || fmt == IF_CI || fmt == IF_CL ||
                             fmt == IF_CS || fmt == IF_CSS || fmt == IF_CB || fmt == IF_CJ);
          if (is_c) {
            e.forced_size = IR_SIZE_2;
            e.can_compress = false;
            e.has_c_choice = false;
          }
          else {
            e.forced_size = IR_SIZE_AUTO;
            e.can_compress = true;
            e.has_c_choice = false;
          }
          e.choice = (struct c_choice){0};

          buf[len++] = e;
}

// Append an alignment/padding IR entry for the current section
void ir_append_align(section_t sect, uint32_t addr_before, uint32_t pad_bytes, int line) {
  if (pad_bytes == 0) { return; }

  if (!ensure_capacity(1)) {
    fprintf(stderr, "Error: out of memory growing buffer (ir.c, line 99)\n");
    return;
  }

  ir_entry_t e = (ir_entry_t){0};
  e.kind = IR_ALIGN;
  e.label = NULL;
  e.reloc = RELOC_NONE;
  e.sect = sect;
  e.addr = addr_before;
  e.size = pad_bytes;
  e.line = line;

  buf[len++] = e;
}

// Appends a data entry to the IR buffer for the given section and address
void ir_append_data(section_t sect, uint32_t addr, int64_t word, uint32_t size, int line) {
  if (!ensure_capacity(1)) {
    fprintf(stderr, "Error: out of memory growing buffer (ir.c, line 118)\n");
    return;
  }

  ir_entry_t e = (ir_entry_t){0};
  e.kind = IR_DATA;
  e.sect = sect;
  e.addr = addr;
  e.size = size;
  e.line = line;
  e.imm = word;
  e.reloc = RELOC_NONE;
  e.forced_size = IR_SIZE_AUTO;
  e.can_compress = false;
  e.has_c_choice = false;

  buf[len++] = e;
}

// Returns the number of IR entries currently buffered
size_t ir_count(void) {
  return len;
}

// Gets a pointer to IR entry i or NULL if out of range
// Returns pointer to IR entry or NULL
ir_entry_t* ir_get(size_t i) {
  if (i >= len) {
    return NULL;
  }
  return &buf[i];
}

// Prints a brief, human-readable table of up to max_rows IR entries
void ir_dump_summary(size_t max_rows) {
  size_t n = (len < max_rows) ? len : max_rows;

  for (size_t i = 0; i < n; i++) {
    const ir_entry_t* e = &buf[i];
    const char* opstr = (e->kind == IR_INSTR) ? op_name(e->op) : "-";
    printf("Index: %zu, Section: %s, Address: 0x%08x, Size: %u,"
           " Kind: %s, Op Name: %s, Format: %s, rd: %d, "
           "rs1: %d, rs2: %d, imm: %" PRId64 ", Label: %s, Reloc: %s, Line: %d\n", 
    i, 
    sect_name(e->sect), 
    (unsigned)e->addr, 
    (unsigned)e->size, 
    kind_name(e->kind), 
    opstr, 
    fmt_name(e->fmt), 
    e->rd, e->rs1, e->rs2, 
    (int64_t)e->imm, 
    e->label ? e->label : "-", 
    reloc_name(e->reloc), 
    e->line);
  }
  if (len > max_rows) {
    printf("+%u more...\n", (unsigned)(len - max_rows));
  }
}

// Frees all IR labels and the underlying buffer
void ir_clear(void) {
  for (size_t i = 0; i < len; i++) {
    free(buf[i].label);
    buf[i].label = NULL;
  }
  free(buf);
  buf = NULL;
  len = cap = 0;
}

// Mark a node as explicitly compressed
void ir_mark_explicit_compressed(ir_entry_t* instr) {
  if (!instr) return;
  instr->forced_size = IR_SIZE_2;
  instr->can_compress = false;
  instr->has_c_choice = false;
}

// Hint that a node must remain 32-bit
void ir_force_32(ir_entry_t* instr) {
  if (!instr) return;
  instr->forced_size = IR_SIZE_4;
  instr->can_compress = false;
  instr->has_c_choice = false;
}

// Allow the node to be auto sized
void ir_force_auto(ir_entry_t* instr) {
  if (!instr) return;
  instr->forced_size = IR_SIZE_AUTO;
  instr->can_compress = true;
}

// Set a selected compressed replacement choice
void ir_set_c_choice(ir_entry_t* instr, const struct c_choice* ch) {
  if (!instr || !ch) return;
  instr->choice = *ch;
  instr->has_c_choice = true;
}

// Returns the currently chosen size for layout
uint8_t ir_get_forced_size(const ir_entry_t* instr) {
  if (!instr) return IR_SIZE_AUTO;
  return (uint8_t)instr->forced_size;
}

// Returns true if this node originated from a c. mnemonic
bool ir_is_explicit_c(const ir_entry_t* instr) {
  if (!instr) return false;
  return (instr->forced_size == IR_SIZE_2) && !instr->can_compress && !instr->has_c_choice;
}

// Clear any previously selected compressed replacement choice
void ir_clear_c_choice(ir_entry_t* instr) {
  if (!instr) return;
  instr->has_c_choice = false;
}