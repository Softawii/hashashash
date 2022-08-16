#ifndef HASH_HASH_H
#define HASH_HASH_H

#include "../core/core.h"

// Typedef for hash function, we will not expose this type to the user.
typedef struct hash_table hash_table;

// Typedef to abstract the hash function.
typedef int (hash_function)(var a, size_t size);

// Initializes a hash table.
hash_table* hash_init(char * file_name, size_t size, hash_function hash_function);
void        hash_free(hash_table * table);
// CRUD
void hash_insert(hash_table* table, var element); // Create
void hash_get   (hash_table* table, int key);     // Read
void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif