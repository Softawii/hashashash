#include <stdio.h>
#include <string.h>
#include "include/hash/hash.h"

int main(int argc, char **argv) {
    int table_size = 7;

    for(int i = 0; i < table_size; i++) {
        if(strcmp(argv[i], "-n") == 0) {
           table_size = atoi(argv[i+1]);     
        }
    }
    
    hash_table * table = hash_create(table_size);
}
