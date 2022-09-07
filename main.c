#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/hash.h"
#include "include/cliente.h"

int main(int argc, char **argv) {
    int table_size = 7;

    // for(int i = 0; i < table_size; i++) {
    //     if(strcmp(argv[i], "-n") == 0) {
    //        table_size = atoi(argv[i+1]);     
    //     }
    // }

    cliente * client1 = (cliente *) malloc(sizeof(cliente));
    client1->id = 3;
    strcpy(client1->nome, "Yancf");
    strcpy(client1->nascimento, "05/07/2022");

    cliente * client2 = (cliente *) malloc(sizeof(cliente));
    client2->id = 14;
    strcpy(client2->nome, "Sérgio Felipe");
    strcpy(client2->nascimento, "20/02/2001");

    cliente * client3 = (cliente *) malloc(sizeof(cliente));
    client3->id = 10;
    strcpy(client3->nome, "Nicolas Magalhães");
    strcpy(client3->nascimento, "10/10/2010");

    hash_table * table = hash_init("hash.dat", table_size, get_cliente_size(), cliente_hash, id_comparator, cliente_from_stream, cliente_to_stream, cliente_show);

    hash_insert(table, client1);
    hash_insert(table, client2);
    hash_insert(table, client3);

    hash_remove(table, client1);

    int t;
    int a = hash_search(table, client1, &t);


    hash_show(table);

    hash_free(table);

    return 0;
}
