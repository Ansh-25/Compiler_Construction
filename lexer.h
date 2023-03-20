/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/

#ifndef lexer_H
#define lexer_H

#include "lexerDef.h"
#include "stdio.h"

extern FILE *ptr;
extern int bufferSize, begin , forward , line ;
extern char* buffer1;
extern char* buffer2;

FILE *getStream(FILE *fp);

FILE* initLexer(FILE* ptr, int buffSize);

struct Token Tokenize(int begin, int forward, tokentype type, int lineNo);

struct Token* getNextToken();

void removeComments(char *testcaseFile, char *cleanFile);

void printToken(struct Token *tk);

#endif