#ifndef HASH_HASH_H
#define HASH_HASH_H
#define DEBUG
#define OCUPADO 1;
#define LIBERADO 0;
#include <stdbool.h>
#include "../core/core.h"

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

int _get_table_index(hash_table *table, int index) {
    return index*(table->element_size + sizeof(int)*2);
}

// Initializes a hash table.
hash_table* hash_init(char * file_name, int size, int element_size, hash_function hash_function, comparator comparator, from_stream from, to_stream to) {

    hash_table * table = (hash_table *) malloc(sizeof(hash_table));
    table->file = fopen(file_name, "wb+");
    table->h = hash_function;
    table->comp = comparator;
    table->to = to;
    table->from = from;
    table->element_size = element_size;
    table->size = size;

    char charNull = 0;
    int varNull = LIBERADO;
    int nextNull = -1;
    for (int i = 0; i < size; i++) {
        fwrite(&charNull, sizeof(char), element_size, table->file);
        fwrite(&nextNull, sizeof(int), 1, table->file);
        fwrite(&varNull, sizeof(int), 1, table->file);
        #ifdef DEBUG
            printf("Escreveu %d de tamanho %d\n", i, element_size);
        #endif
    }

    #ifdef DEBUG
        printf("Hash Init End\n");
    #endif

    return table;
}

void hash_free(hash_table * table) {
    fclose(table->file);
    free(table);
}

void hash_insert(hash_table* table, var element) {
    int index;
    index = table->h(element, table->size);
    fseek(table->file, _get_table_index(table, index), SEEK_SET);
    table->to(table->file, element);
    
    int ocupied = 1, nextNull = -1;
    fwrite(&nextNull, sizeof(int), 1, table->file);
    fwrite(&ocupied, sizeof(int), 1, table->file);
}


int hash_search (hash_table* table, var element){

}

hash_element * hash_read (hash_table* table, int index) {
    fseek(table->file, _get_table_index(table, index), SEEK_SET);
    hash_element * element = (hash_element *)malloc(sizeof(hash_element) *  1);
    element->element = table->from(table->file);
    fread(&element->next, sizeof(int), 1, table->file);
    fread(&element->flag, sizeof(int), 1, table->file);

    #ifdef DEBUG
        printf("Next: %d | Flag: %d\n", element->next, element->flag);
    #endif

    return element;
}


void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif