#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// SIZE and LOAD_FACTOR defined in symtab.h

// Creates the initial symbol table with size SIZE
// Returns a pointer to the symbol table
sym_t* create_symtab() {
  sym_t* sym_tab = (sym_t*)malloc(sizeof(sym_t));
  
  if (sym_tab == NULL) {
    fprintf(stderr, "Error: Failed to allocate memory for symbol table (symtab.c, line 13)\n");
    return NULL;
  }

  sym_tab->size = SIZE;
  sym_tab->hash_table = (kvp_t**)calloc(SIZE, sizeof(kvp_t*));
  sym_tab->count = 0;

  return sym_tab;
}

// Uses djb2 algorithm to create an index for the symbol table
// Returns the index after hashing
int hash(const char* key, int table_size) {
  unsigned long hash = 5381;

  for (const char* ptr = key; *ptr != '\0'; ptr++) {
    hash = hash * 33 + *ptr;
  }

  return hash % table_size;
}

// Checks to see if an integer n is a prime number or not
// Returns true if prime, otherwise false
bool is_prime(int n) {
  if (n < 2) {
    return false;
  }
  else if (n <= 3) {
    return true;
  }
  else if (n % 2 == 0) {
    return false;
  }
  
  int limit = (int)sqrt(n);

  for (int i = 3; i <= limit; i+=2) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

// Finds the next prime number from integer n to infinity
int find_next_prime(int n) {
  while (true) {
    if (is_prime(n)) {
      return n;
    }
    n++;
  }
}

// Resizes the symbol table to be the next prime number after doubling the old size
void resize_symtab(sym_t* symtab) {
  kvp_t** old_table = symtab->hash_table;
  int old_size = symtab->size;
  int new_size = find_next_prime(old_size * 2);
  symtab->size = new_size;
  symtab->count = 0;
  symtab->hash_table = (kvp_t**)calloc(new_size, sizeof(kvp_t*));

  if (!symtab->hash_table) {
    fprintf(stderr, "Error: Failed to allocate memory during resizing (symtab.c, line 79)\n");
    symtab->size = old_size;
    symtab->hash_table = old_table;
    return;
  }

  for (int i = 0; i < old_size; i++) {
    kvp_t* node = old_table[i];
    while (node != NULL) {
      kvp_t* next = node->next;
      insert_sym(symtab, node->key, node->value);
      free(node->key);
      free(node);
      node = next;
    }
  }

  free(old_table);
}

// Inserts a kvp into the symbol table
// Returns true if successfully inserted, otherwise false
bool insert_sym(sym_t* symtab, const char* key, int value) {
  if ((float)symtab->count / symtab->size >= LOAD_FACTOR) {
    resize_symtab(symtab);
  }

  int index = hash(key, symtab->size);
  kvp_t* target_bucket = symtab->hash_table[index];

  if (target_bucket == NULL) {
    kvp_t* new_node = (kvp_t*)malloc(sizeof(kvp_t));
    char* key_copy = (char*)malloc(strlen(key) + 1);

    if (new_node == NULL || key_copy == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for node or key copy (symtab.c, lines 113-114)\n");
      return false;
    }

    strcpy(key_copy, key);
    new_node->key = key_copy;
    new_node->value = value;
    new_node->next = NULL;
    symtab->hash_table[index] = new_node;
    symtab->count++;
    return true;
  }
  else {
    kvp_t* prev = NULL;

    while (target_bucket != NULL) {
      if (strcmp(target_bucket->key, key) == 0) {
        fprintf(stderr, "Error: Duplicate Label '%s'\n", key);
        return false;
      }

      prev = target_bucket;
      target_bucket = target_bucket->next;
    }
    kvp_t* new_node = (kvp_t*)malloc(sizeof(kvp_t));
    char* key_copy = (char*)malloc(strlen(key) + 1);

    if (new_node == NULL || key_copy == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for node or key copy (symtab.c, lines 141-142)\n");
      return false;
    }

    strcpy(key_copy, key);
    new_node->key = key_copy;
    new_node->value = value;
    new_node->next = NULL;
    prev->next = new_node;
    symtab->count++;
    return true;
  }
}

// Searches for a value based on a specified key
// Returns value if found, otherwise -1
int search_sym(sym_t* symtab, const char* key) {
  int index = hash(key, symtab->size);
  kvp_t* target_bucket = symtab->hash_table[index];

  while (target_bucket != NULL) {
    if (strcmp(target_bucket->key, key) == 0) {
      return target_bucket->value;
    }
    target_bucket = target_bucket->next;
  }

  return -1;
}

// Removes a value from the symbol table based on the specified key
// Returns true if successfull, otherwise false
bool remove_sym(sym_t* symtab, const char* key) {
  int index = hash(key, symtab->size);
  kvp_t* target_bucket = symtab->hash_table[index];
  kvp_t* prev = NULL;

  if (target_bucket == NULL) {
    fprintf(stderr, "Error: Key could not be removed from symtab as it doesn't exist\n");
    return false;
  }

  while (target_bucket != NULL) {
    if (prev == NULL && strcmp(target_bucket->key, key) == 0) {
      symtab->hash_table[index] = target_bucket->next;
      free(target_bucket->key);
      free(target_bucket);
      symtab->count--;
      return true;
    }
    else if (strcmp(target_bucket->key, key) == 0) {
      prev->next = target_bucket->next;
      free(target_bucket->key);
      free(target_bucket);
      symtab->count--;
      return true;
    }
    prev = target_bucket;
    target_bucket = target_bucket->next;
  }

  fprintf(stderr, "Error: Key could not be removed from symtab as it doesn't exist\n");
  return false;
}

// Removes all values from the symbol table
// Returns true if successful, otherwise false
bool remove_all_sym(sym_t* symtab) {
  if (symtab == NULL) {
    fprintf(stderr, "Error: Failed to remove all from symtab due to symtab being NULL\n");
    return false;
  }

  for (int i = 0; i < symtab->size; i++) {
    kvp_t* node = symtab->hash_table[i];
    while (node != NULL) {
      kvp_t* next = node->next;
      free(node->key);
      free(node);
      node = next;
    }
    symtab->hash_table[i] = NULL;
  }
  symtab->count = 0;
  return true;
}

// Frees the symbol table after it is no longer needed
void free_symtab(sym_t* symtab) {
  if (!symtab) {
    return;
  }

  remove_all_sym(symtab);
  free(symtab->hash_table);
  free(symtab);
}