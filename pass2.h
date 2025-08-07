#ifndef PASS2_H
#define PASS2_H

#include <stdint.h>
#include "section.h"
#include "ir.h"
#include "symtab.h"

void pass2_initialize(void);
void pass2_run(const char* output_filename);
int32_t encode_instruction(const ir_entry_t* entry, sym_t* symtab, int* error_flag);

#endif