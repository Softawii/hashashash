#ifndef HASH_HASH_H
#define HASH_HASH_H

#include <stdbool.h>
#include "../core/core.h"

// Typedef for hash function, we will not expose this type to the user.
typedef struct hash_table hash_table;

// Typedef to abstract the hash function.
typedef int (hash_function)(var a, int size);

struct hash_element {
    var element;
    var * next;
    bool is_free;
};

struct hash_table {
    int size;
    int count;
    struct hash_element * table;
    hash_function * h;
};

// Initializes a hash table.
hash_table* hash_init(char * file_name, size_t size, hash_function hash_function) {
    hash_table * table = (hash_table *) malloc(sizeof(hash_table));
    table->size = size;
    table->count = 0;
    table->table = (struct hash_element *) malloc(sizeof(struct hash_element) * size);
    table->h = hash_function;
    for(int i = 0; i < size; i++) {
        table->table[i].is_free = true;
    }

    #ifdef DEBUG
    printf("Hash table initialized with size %d\n", size);
    #endif

    return table;
}

void hash_free(hash_table * table) {
    for(int i = 0; i < table->size; i++) if(table->table[i].is_free == false) free(table->table[i].element);
    free(table->table);
    free(table);   
}

// CRUD
void hash_insert(hash_table* table, var element) {
    int index = table->h(element, table->size);

    #ifdef DEBUG
    printf("[DEBUG] Index %d.\n", index);
    #endif

    if(table->table[index].is_free == true) {
        table->table[index].element = element;
        table->table[index].is_free = false;
        table->count++;
    } else {
        // TODO: implement
        #ifdef DEBUG
        printf("[DEBUG] Collision detected at index %d.\n", index);
        #endif
    }

}
void hash_get   (hash_table* table, int key);     // Read
void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif