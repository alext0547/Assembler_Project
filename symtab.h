#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SIZE 17
#define LOAD_FACTOR 0.5

typedef struct kvp{
  char *key;
  int value;
} kvp_t;

typedef struct sym{
  kvp_t *hash_table[SIZE];
  kvp_t *dummy;
  int count;
} sym_t;

sym_t *create_symtab();
void insert_sym(sym_t *symtab, const char *key, int value);
int search_sym(sym_t *symtab, const char *key);
void remove_sym(sym_t *table, const char *key);
void remove_all_sym(sym_t *table);
void free_symtab(sym_t *table);

#endif