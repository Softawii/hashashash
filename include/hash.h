#ifndef HASH_HASH_H
#define HASH_HASH_H
#define DEBUG
#define OCUPADO 1
#define LIBERADO 0
#include <stdbool.h>
#include "core.h"

// Typedef for hash function, we will not expose this type to the user.
typedef struct hash_table hash_table;

// Typedef to abstract the hash function.
typedef int (hash_function)(var a, int size);

typedef struct hash_element hash_element;

struct hash_element {
    var element;
    int next;
    int flag;
};

struct hash_table
{
    FILE *file;
    hash_function * h;
    comparator * comp;
    from_stream * from;
    to_stream * to;
    int element_size;
    int size;
};

int _get_table_index(hash_table *table, int index);

hash_element * hash_read (hash_table* table, int index);

// Initializes a hash table.
hash_table* hash_init(char * file_name, int size, int element_size, hash_function hash_function, comparator comparator, from_stream from, to_stream to);

void hash_free(hash_table * table);

int hash_search (hash_table* table, var element, int *end);

void hash_insert(hash_table* table, var element);
void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif