/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/
#ifndef hash_H
#define hash_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"

struct hashentry {
    char str[20];
    tokentype tk;
};

int sumchars(char* s);

void insert_hash(char* str, tokentype tk);

int search_hash(char* str);

void init_hash();

#endif