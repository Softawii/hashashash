#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/hash/hash.h"
#include "include/client/cliente.h"

int main(int argc, char **argv) {
    int table_size = 7;

    for(int i = 0; i < table_size; i++) {
        if(strcmp(argv[i], "-n") == 0) {
           table_size = atoi(argv[i+1]);     
        }
    }

    cliente * client = (cliente *) malloc(sizeof(cliente));
    client->id = 1;
    strcpy(client->nome, "Yancf");
    strcpy(client->nascimento, "05/07/2022");
    printf("%d\n", cliente_hash(client, table_size)); 


    hash_table * table = hash_init("hash.txt", table_size, cliente_hash);
    hash_insert(table, client);
}
