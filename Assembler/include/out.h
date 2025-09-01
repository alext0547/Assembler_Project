#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "section.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  OUT_RAW = 0,
  OUT_ELF = 1
} out_format_t;

void out_begin(out_format_t fmt, int xlen);
void out_set_file(FILE* f);
void out_begin_section(section_t sect);
void out_mark_rvc(void);
bool out_finish(uint64_t entry_addr);
bool write_word_le(uint32_t word);
bool write_half_le(uint16_t half);
bool write_value_le(int64_t value, uint32_t size);
bool write_zeros(uint32_t n);
uint64_t out_text_base(void);
void out_dispose(void);

#ifdef __cplusplus
}
#endif