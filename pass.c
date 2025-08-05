#include <stdint.h>
#include "pass.h"

int pass_num = 1;
section_t cur_section = SEC_TEXT;
uint32_t pc_text = 0, pc_data = 0;

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

// Resets the pass back to default values, primarily for testing
void pass_reset(void) {
  pass_num = 1;
  cur_section = SEC_TEXT;
  pc_text = 0;
  pc_data = 0;
}