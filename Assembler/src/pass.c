#include <stdint.h>
#include "pass.h"
#include <stdbool.h>

static int text_entered = 0;
int pass_num;
section_t cur_section = SEC_TEXT;
uint32_t pc_text = 0, pc_data = 0;
static int xlen = 32;
static bool extM = false;

// Returns the current value of the program counter depending on which section the assembler is processing
uint32_t pass_current_pc(void) {
  if (cur_section == SEC_TEXT) { return pc_text; }

  return pc_data;
}

// Increases the program counter by a specified number of bytes in the section the assembler is processing
void pass_advance_pc(uint32_t bytes) {
  if (cur_section == SEC_TEXT) { pc_text += bytes; }
  else { pc_data += bytes; }
}

// Updates the section the assembler is processing
void pass_set_section(section_t s) {
  cur_section = s;
  if (s == SEC_TEXT) {
    text_entered = 1;
  }
}

// Returns the current section being processed by the assembler
section_t pass_get_section(void) {
  return cur_section;
}

// Adjusts current program counter (depending on section) so that it is aligned to the next multiple of 2^pow2
uint32_t pass_align_current_pc(uint32_t pow2) {
  if (cur_section == SEC_TEXT) {
    uint32_t offset = pass_align_pad(pc_text, pow2);
    pc_text += offset;
    return pc_text;
  }
  else {
    uint32_t offset = pass_align_pad(pc_data, pow2);
    pc_data += offset;
    return pc_data;
  }
}

// Returns 1 if text section was entered, otherwise 0
int pass_was_text_entered(void) {
  return text_entered;
}

// Sets the target ISA width and toggles the M extension
void pass_set_arch(int len, bool hasM) {
  if (len != 32 && len != 64) len = 32;
  xlen = len;
  extM = hasM;
}

// Returns the currently selected XLEN
int pass_get_xlen(void) {
  return xlen;
}

// Reports whether the M extension is enabled
bool pass_has_M(void) {
  return extM;
}