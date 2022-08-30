#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/hash.h"
#include "include/cliente.h"
#define DEBUG

int main(int argc, char **argv) {
    int table_size = 7;

    // for(int i = 0; i < table_size; i++) {
    //     if(strcmp(argv[i], "-n") == 0) {
    //        table_size = atoi(argv[i+1]);     
    //     }
    // }

    cliente * client = (cliente *) malloc(sizeof(cliente));
    client->id = 3;
    strcpy(client->nome, "Yancf");
    strcpy(client->nascimento, "05/07/2022");
    printf("%d\n", cliente_hash(client, table_size)); 

    hash_table * table = hash_init("hash.dat", table_size, get_cliente_size(), cliente_hash, id_comparator, cliente_from_stream, cliente_to_stream);

    hash_insert(table, client);

    hash_read(table, 3);


    int t;
    int a = hash_search(table, client, &t);

    printf("a = %d, end = %d\n", a, t);
    //hash_read(table, 3);

    hash_free(table);
}
