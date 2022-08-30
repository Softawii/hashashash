#include <stdlib.h>
#include "../include/hash.h"
#include "../include/core.h"

int _get_table_index(hash_table *table, int index) {
    return index*(table->element_size + sizeof(int)*2);
}

hash_element * _get_hash_element (hash_table *table, int index) {
    fseek(table->file, _get_table_index(table, index), SEEK_SET);

    hash_element * element = (hash_element *)malloc(sizeof(hash_element) *  1);
    element->element = table->from(table->file);

    if(!fread(&element->next, sizeof(int), 1, table->file)) {
        printf("ERRO: Leitura de elemento falhou!\n");
    }

    if(!fread(&element->flag, sizeof(int), 1, table->file)) {
        printf("ERRO: Leitura de elemento falhou!\n");
    }

    return element;
}

void _set_hash_element (hash_table *table, int index, hash_element * element) {
    fseek(table->file, _get_table_index(table, index), SEEK_SET);
    table->to(table->file, element->element);
    fwrite(&element->next, sizeof(int), 1, table->file);
    fwrite(&element->flag, sizeof(int), 1, table->file);
}

void _set_hash_element_element (hash_table *table, int index, var element) {
    fseek(table->file, _get_table_index(table, index), SEEK_SET);
    table->to(table->file, element);
}

void _set_hash_element_next (hash_table *table, int index, int next) {
    fseek(table->file, _get_table_index(table, index) + table->element_size, SEEK_SET);
    fwrite(&next, sizeof(int), 1, table->file);
}

void _set_hash_element_flag  (hash_table *table, int index, int flag) {
    fseek(table->file, _get_table_index(table, index) + table->element_size + sizeof(int), SEEK_SET);
    fwrite(&flag, sizeof(int), 1, table->file);
}

/**
 * @brief Initizalize the hash table for the type required.
 * 
 * @param file_name Name of the file to be used.
 * @param size Size of the hash table
 * @param element_size Size (in bytes) of the element to be stored.
 * @param hash_function Hash function to be used.
 * @param comparator Function to comparate your data strutcture.
 * @param from Function to get your data strtucture from a stream.
 * @param to Function to get your data strtucture to a stream.
 * @param show Function to print your data strtucture.
 * @return hash_table* Table initialized.
 */
hash_table* hash_init(char *file_name, int size, int element_size, hash_function hash_function, comparator comparator, from_stream from, to_stream to, show show) {
    #ifdef HASH_DEBUG
        printf("\nhash_init: start\n");
    #endif
    
    hash_table *table = (hash_table *) malloc(sizeof(hash_table));

    table->file = fopen(file_name, "wb+");
    table->size = size;
    table->element_size = element_size;
    table->h = hash_function;
    table->comp = comparator;
    table->from = from;
    table->to = to;
    table->show = show;

    char elementNull = 0;
    int nextNull = -1;
    int flagNull = LIBERADO;
    for (int i = 0; i < size; i++) {
        fwrite(&elementNull, sizeof(char), element_size, table->file);
        fwrite(&nextNull, sizeof(int), 1, table->file);
        fwrite(&flagNull, sizeof(int), 1, table->file);
    }

    #ifdef HASH_DEBUG
        printf("hash_init: initialized table with size=%d\n", size);
        printf("hash_init: end\n\n");
    #endif

    return table;
}

/**
 * @brief Finish the hash table and free the memory.
 * 
 * @param table Table to be freed.
 */
void hash_free(hash_table *table) {
    fclose(table->file);
    free(table);
}

/**
 * @brief Search for an element in the hash table.
 * 
 * @param table The table to be searched.
 * @param element The element to be searched.
 * @param end Return the index of the element found. If not found, return the index where it could be inserted or -1 if there are no indexes left.
 * @return int The status of the search. 1 if found, 2 if not found.
 */
int hash_search (hash_table *table, var element, int *end){
    #ifdef HASH_DEBUG
        printf("\nhash_search: start\n");
    #endif

    *end = table->h(element, table->size);

    int status = 0; // 0 = searching, 1 = found, 2 = not found
    int firstFreePosition = -1; // -1 = Null
    int foundFirstFreePosition = 0; // 0 = false, 1 = true

    while(status == 0) {
        #ifdef HASH_DEBUG
            printf("hash_serch: searching in index=%d\n", *end);
        #endif

        hash_element* tableElement = _get_hash_element(table, *end);

        if(tableElement->flag == LIBERADO && !foundFirstFreePosition) {
            firstFreePosition = *end;
            foundFirstFreePosition = 1;  // Found the first free position
            #ifdef HASH_DEBUG
                printf("hash_serch: founded free position=%d\n", firstFreePosition);
            #endif
        }

        if(table->comp(element, tableElement->element) == 0 && tableElement->flag == OCUPADO) {
            status = 1; // Found
        }
        else {
            if(tableElement->next == -1) {
                status = 2; // Not Found
                *end = firstFreePosition;
            } else {
                *end = tableElement->next; // Continue searching
            }
        }
    }

    #ifdef HASH_DEBUG
        printf("hash_serch: element %s\n", status == 1 ? "found" : "not found");
        printf("hash_search: end\n\n");
    #endif

    return status;
}

/**
 * @brief Insert an element in the hash table.
 * 
 * @param table The table where the elemente will be inserted
 * @param element The element to be inserted.
 */
void hash_insert(hash_table *table, var element) {
    #ifdef HASH_DEBUG
        printf("\nhash_insert: start\n");
    #endif

    int index;
    int freePosition;

    int hx = table->h(element, table->size);
    
    #ifdef HASH_DEBUG
        printf("hash_insert: hash function=%d\n", hx);
    #endif

    int status = hash_search(table, element, &index);

    hash_element* tableElement;

    if(status != 1) {
        if(index != -1) {
            freePosition = index;
        }
        else {
            freePosition = hx;

            int foundFreePosition = 0;
            
            for (int i = 1; i <= table->size; i++) {
                tableElement = _get_hash_element(table, freePosition);

                if(tableElement->flag == OCUPADO) {
                    freePosition = (freePosition + 1) % table->size; 
                }
                else {
                    foundFreePosition = 1;
                    break;
                }
            }

            if(!foundFreePosition) {
                printf("ERROR: There is no room to insert the element!\n");
                return;
            }

            tableElement = _get_hash_element(table, hx);
            int temp = tableElement->next;

            _set_hash_element_next(table, hx, freePosition);

            _set_hash_element_next(table, freePosition, temp);
        }
        #ifdef HASH_DEBUG
            printf("hash_insert: inserting in index=%d\n", freePosition);
        #endif

        _set_hash_element_element(table, freePosition, element);

        _set_hash_element_flag(table, freePosition, OCUPADO);
    }
    else {
        printf("ERROR: The element is already in the hash table!\n");
        return;
    }

    #ifdef HASH_DEBUG
        printf("hash_insert: end\n\n");
    #endif
}

/**
 * @brief Remove an element from the hash table.
 * 
 * @param table The table where the element will be removed.
 * @param element The element to be removed.
 */
void hash_remove (hash_table *table, var element) {
    #ifdef HASH_DEBUG
        printf("\nhash_remove: start\n");
    #endif
    
    int index;
    int status = hash_search(table, element, &index);

    if(status == 1) {
        _set_hash_element_flag(table, index, LIBERADO);
    }
    else {
        printf("ERROR: The element is not in the hash table!\n");
        return;
    }
    #ifdef HASH_DEBUG
        printf("hash_remove: removing element of index=%d\n", index);
        printf("hash_remove: end\n\n");
    #endif
}

/**
 * @brief Update an element from an index.
 * 
 * @param table The table where the element will be updated.
 * @param oldElement The old element.
 * @param newElement The new element.
 */
void hash_update (hash_table *table, var oldElement, var newElement) {
    int index;
    int status = hash_search(table, oldElement, &index);
    
    if(status == 1) {
        _set_hash_element_element(table, index, newElement);
    }
    else {
        printf("ERROR: The element is not in the hash table!\n");
    }
}

/**
 * @brief Print the hash table.
 * 
 * @param table The table to be printed.
 */
void hash_show(hash_table *table) {
    for (int i = 0; i < table->size; i++) {
        hash_element* tableElement = _get_hash_element(table, i);
        if(tableElement->flag == OCUPADO) {
            printf("%d: ", i);
            printf("Element: ");
            table->show(tableElement->element);
            printf(" | Next: %d | Flag: %s\n", tableElement->next, tableElement->flag? "Ocupado" : "Liberado");
        }
        else {
            printf("%d: Element: Null | Next: %d | Flag: %s\n", i, tableElement->next, tableElement->flag? "Ocupado" : "Liberado");
        }
    }
}