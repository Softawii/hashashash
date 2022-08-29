//
// Created by yancf on 26/06/2022.
//

#ifndef HASH_CLIENTE_H
#define HASH_CLIENTE_H

#include "../core/core.h"
#include <stdio.h>

typedef struct{
    int id;
    char nome[50];
    char nascimento[11];
}cliente;


var cliente_from_stream(FILE * file) {

    cliente *temp = (cliente *) malloc(sizeof(cliente));

    fread(&temp->id, sizeof(temp->id), 1, file);
    fread(temp->nome, sizeof(temp->nome), 1, file);
    size_t read = fread(temp->nascimento, sizeof(temp->nascimento), 1, file);

    if(read == 0) return NULL;

    #ifdef DEBUG
        printf("ID: %d | Nome: %s | Nascimento: %s\n", temp->id, temp->nome, temp->nascimento);
    #endif

    // Return
    return temp;
}

void cliente_to_stream(FILE * file, var c) {
    cliente *tempCliente = (cliente*) c;
    fwrite(&tempCliente->id, sizeof(tempCliente->id), 1, file);
    fwrite(tempCliente->nome, sizeof(tempCliente->nome), 1, file);
    fwrite(tempCliente->nascimento, sizeof(tempCliente->nascimento), 1, file);
}

void cliente_show(cliente * c) {
    printf("\tID: %7d, Name: %10s, Birth: %s\n", c->id, c->nome, c->nascimento);
}

int cliente_hash(var a, int divisor) {
    int p1 = ((cliente*) a)->id;
    return p1 % divisor;
}

int id_comparator (var  a, var  b) {
    int p1 = ((cliente*) a)->id;
    int p2 = ((cliente*) b)->id;

    if(p1 > p2)
        return 1;
    else if(p1 == p2)
        return 0;
    else
        return -1;
}

int get_cliente_size () {
    cliente cliente;
    return sizeof(cliente.nome) + sizeof(cliente.nascimento) + sizeof(cliente.id);
}

#endif //HASH_CLIENTE_H


