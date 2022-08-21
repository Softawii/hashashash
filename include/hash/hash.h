#ifndef HASH_HASH_H
#define HASH_HASH_H
#define DEBUG

#include <stdbool.h>
#include "../core/core.h"

// Typedef for hash function, we will not expose this type to the user.
typedef struct hash_table hash_table;

// Typedef to abstract the hash function.
typedef int (hash_function)(var a, int size);

typedef struct hash_element hash_element;

struct hash_element {
    var element;
    struct hash_element * next;
    int flag;
};

struct hash_table
{
    FILE *file;
    hash_function * h;
    comparator * comp;
    from_stream * from;
    to_stream * to;
};

// Initializes a hash table.
hash_table* hash_init(char * file_name, size_t size, size_t element_size, hash_function hash_function, comparator comparator) {

    hash_table * table = (hash_table *) malloc(sizeof(hash_table));
    table->file = fopen(file_name, "wb+");
    table->h = hash_function;
    table->comp = comparator;

    int temp = 0;
    for (int i = 0; i < size; i++) {
        fwrite(&temp, element_size, 1, table->file);
        fwrite(&temp, sizeof(int), 1, table->file);
        fwrite(&temp, sizeof(int), 1, table->file);
    }

    return table;
}

void hash_free(hash_table * table) {
    free(table->file); 
    free(table);
}

// CRUD
void hash_insert(hash_table* table, var element) {

}


int hash_search (hash_table* table, var element){

}


void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif