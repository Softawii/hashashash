#ifndef HASH_HASH_H
#define HASH_HASH_H
#define DEBUG
#define OCUPADO 1
#define LIBERADO 0
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

int hash_search (hash_table* table, var element, int *end){
    #ifdef DEBUG
        printf("hash_search: start\n");
    #endif

    int temp = table->h(element, table->size);
    *end = temp;

    int a = 0; 
    int j = -1;

    int foundFirstFreePosition = 0;
    while(a == 0) {
        #ifdef DEBUG
            printf("hash_serch: ler no index=%d\n", *end);
        #endif
        hash_element* tableElement = hash_read(table, *end);

        if(tableElement->flag == LIBERADO && !foundFirstFreePosition) {
            j = *end;
            foundFirstFreePosition = 1;
        }

        if(table->comp(element, tableElement->element) == 0 && tableElement->flag == OCUPADO) {
            a = 1;
        }
        else {
            if(tableElement->next == -1) {
                a = 2;
                *end = j;
            } else {
                *end = tableElement->next;
            }
        }
    }

    #ifdef DEBUG
        printf("hash_search: final\n\n");
    #endif

    return a;
}

void hash_insert(hash_table* table, var element) {
    #ifdef DEBUG
        printf("hash_insert: start\n");
    #endif

    int index;
    int j, i=1;
    int hx = table->h(element, table->size);

    int a = hash_search(table, element, &index);

    hash_element* tableElement;

    if(a != 1) {
        if(index != -1) {
            j = index;
        }
        else {
            j = hx;
            while(i <= table->size) {
                tableElement = hash_read(table, j);
                if(tableElement->flag == OCUPADO) {
                    j = (j + 1) % table->size; 
                    i++;
                }
                else {
                    i = table->size + 2;
                }
            }
            if(i == table->size + 1) {
                printf("DEU MERDA AI DENOVO, INSERIU ERRADO\n");
                return;
            }
            tableElement = hash_read(table, hx);
            int temp = tableElement->next;

            fseek(table->file, _get_table_index(table, hx) + table->element_size, SEEK_SET);
            fwrite(&j, sizeof(int), 1, table->file);

            tableElement = hash_read(table, j);
            fseek(table->file, _get_table_index(table, j) + table->element_size, SEEK_SET);
            fwrite(&temp, sizeof(int), 1, table->file);
        }
        tableElement = hash_read(table, j);
        fseek(table->file, _get_table_index(table, j), SEEK_SET);
        table->to(table->file, element);

        fseek(table->file, _get_table_index(table, j) + table->element_size + sizeof(int), SEEK_SET);
        int ocupied = OCUPADO;
        fwrite(&ocupied, sizeof(int), 1, table->file);
    }
    else {
        printf("DEU ERRO AI BIXO\n");
    }

    #ifdef DEBUG
        printf("hash_insert: final\n\n");
    #endif
}


void hash_update(hash_table* table, var element); // Update
void hash_remove(hash_table* table, int key);     // Delete

#endif