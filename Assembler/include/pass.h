#ifndef PASS_H
#define PASS_H

#include <stdint.h>
#include "symtab.h"
#include "section.h"

extern int pass_num;
extern section_t cur_section;
extern uint32_t pc_text, pc_data;
extern sym_t* symtab;
extern int xlen;
extern bool extM;

uint32_t pass_current_pc(void);
void pass_advance_pc(uint32_t bytes);
void pass_set_section(section_t s);
section_t pass_get_section(void);
uint32_t pass_align_current_pc(uint32_t pow2);
int pass_was_text_entered(void);

// Computes the next address aligned to 2^pow2
// Returns the newly aligned address
static inline uint32_t pass_align_to_pow2(uint32_t addr, uint32_t pow2) {
  uint32_t a = 1u << pow2;
  return (addr + (a - 1u)) & ~(a - 1u);
}

// Calculates how many bytes of padding are needed to align addr to 2^pow2
// Returns the number of bytes of padding required
static inline uint32_t pass_align_pad(uint32_t addr, uint32_t pow2) {
  uint32_t aligned = pass_align_to_pow2(addr, pow2);
  return aligned - addr;
}

#endif