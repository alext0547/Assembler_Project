#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

sym_t* create_symtab() {
  sym_t* table = malloc(sizeof(sym_t));
  if (table == NULL) {
    return NULL;
  }

  for (int i = 0; i < SIZE; i++) {
    table->hash_table[i] = NULL;
  }

  table->dummy = malloc(sizeof(kvp_t));

  if (table->dummy != NULL) {
    table->dummy->key = strdup("DUMMY");
    table->dummy->value = -1;
  }

  table->count = 0;
  return table;
}