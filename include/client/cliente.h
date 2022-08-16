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


var  cliente_from_stream(FILE * file);
void cliente_to_stream(FILE * file, cliente * c);
void cliente_show(cliente * c);
int cliente_hash(cliente * a, int divisor) ;


#endif //HASH_CLIENTE_H

