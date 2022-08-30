#include <stdbool.h>
#include "core.h"

#ifndef HASH_HASH_H
#define HASH_HASH_H

#define OCUPADO 1
#define LIBERADO 0

// Typedef for hash function, we will not expose this type to the user.
typedef struct hash_table hash_table;

// Typedef to abstract the hash function.
typedef int (hash_function)(var a, int size);

typedef struct hash_element hash_element;

/**
 * @brief Struct that represents an element in the hash table.
 */
struct hash_element {
    var element;
    int next;
    int flag;
};

/**
 * @brief Struct that represents a hash table.
 */
struct hash_table
{
    FILE *file;
    int size;
    int element_size;
    hash_function * h;
    comparator * comp;
    from_stream * from;
    to_stream * to;
    show * show;
};

// Hash functions

hash_table* hash_init (char *file_name, int size, int element_size, hash_function hash_function, comparator comparator, from_stream from, to_stream to, show show);
void hash_free (hash_table *table);
int hash_search (hash_table *table, var element, int *end);
void hash_insert (hash_table *table, var element);
void hash_remove (hash_table *table, var element);
void hash_update (hash_table *table, var oldElement, var newElement);
void hash_show (hash_table *table);

#endif