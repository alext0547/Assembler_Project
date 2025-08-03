#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#define SIZE 53
#define LOAD_FACTOR 0.5

typedef struct kvp{
  char* key;
  int value;
  struct kvp* next;
} kvp_t;

typedef struct sym{
  kvp_t** hash_table;
  int count;
  int size;
} sym_t;

sym_t* create_symtab();
int hash(const char* key, int table_size);
bool insert_sym(sym_t* symtab, const char* key, int value);
int search_sym(sym_t* symtab, const char* key);
bool remove_sym(sym_t* symtab, const char* key);
bool remove_all_sym(sym_t* symtab);
void free_symtab(sym_t* symtab);

#endif