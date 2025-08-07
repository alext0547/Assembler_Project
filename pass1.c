#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "pass.h"
#include "symtab.h"
#include "ir.h"
#include "pass1.h"

sym_t* symtab;

// Initializes global state for pass1
void pass1_initialize(void) {
  pass_reset();
  symtab = create_symtab();
  ir_init();
}

// Emits an instruction to the IR with its full metadata
void pass1_emit_instruction(opcode_t op, ir_fmt_t fmt, int rd, int rs1, int rs2, int64_t imm, 
  const char* label, reloc_kind_t reloc, int lineno) {
  section_t sect = cur_section;
  uint32_t addr = pass_current_pc();
  uint32_t size = 4;

  ir_append_instr(op, fmt, rd, rs1, rs2, imm, label, reloc, sect, addr, size, lineno);
}

// Emits an alignment directive and advances the program counter by the padding
void pass1_emit_align(uint32_t pad_bytes, int lineno) {
  section_t sect = cur_section;
  uint32_t addr = pass_current_pc();

  ir_append_align(sect, addr, pad_bytes, lineno);
  pass_advance_pc(pad_bytes);
}

// Emits a 64-bit data word to the IR and advances program counter
void pass1_emit_data(int64_t word, uint32_t size, int lineno) {
  section_t sect = cur_section;
  uint32_t addr = pass_current_pc();

  ir_append_data(sect, addr, word, size, lineno);
  pass_advance_pc(size);
}

// Inserts a label into the symbol table with the current program counter as its address
void pass1_emit_label(char* label) {
  if (!insert_sym(symtab, label, (int)pass_current_pc())) {
    // insert_sym already produces an error for duplicates
  }
  free(label);
}

// Finalizes pass1 by printing a warning if the .text wasn't found and dumps the IR summary
void pass1_finalize(void) {
  int text = pass_was_text_entered();
  if (text != 1) {
    fprintf(stderr, "Warning: no .text section encountered - no instructions will be processed\n");
  }
  ir_dump_summary(100);
}

// Emits N zero bytes to reserve uninitialized space in the current section
void pass1_emit_space(uint64_t num_bytes, int lineno) {
  section_t sect = cur_section;
  uint32_t addr = pass_current_pc();

  ir_append_data(sect, addr, 0, num_bytes, lineno);
  pass_advance_pc(num_bytes);
}