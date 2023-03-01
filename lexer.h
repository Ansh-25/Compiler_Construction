#ifndef lexer_H
#define lexer_H

#include "lexerDef.h"

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