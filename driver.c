/*
    ID: 2020A7PS0035P | Name: Shreekar Puranik
    ID: 2020A7PS1209P | Name: Sriram Ramanathan
    ID: 2020A7PS1205P | Name: Nikhil Pradhan
    ID: 2020A7PS0146P | Name: Toshit Jain
    ID: 2020A7PS0116P | Name: Ansh Gupta
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "typeChecker.h"

int main(int argc, char *argv[]){
    int choice,size_of_buffer = 32; 
    clock_t start_time, end_time;
    char test_file[32],asm_file[32];
    
    ParseNode* parserRoot=NULL;
    strcpy(test_file,argv[1]);
    strcpy(asm_file,argv[2]);

    printf("\nIMPLEMENTATION STATUS\nLEVEL 4: Symbol table/ AST/ Semantic Rules work and handled static and dynamic arrays in type checking \n\n");
    do{
        printf("\t\t MAIN MENU \n");
        printf("Press '0' EXIT: For exiting the program\n");
        printf("Press '1' Lexer: For printing the token list generated by the lexer \n");
        printf("Press '2' Parser: For parsing the source code and printing the parse tree \n");
        printf("Press '3' AST: For printing the abstract syntax tree in preorder traversal\n");
        printf("Press '4' Memory: For displaying the amount of allocated memory\n");
        printf("Press '5' Symbol Table: For printing the Symbol Table\n");
        printf("Press '6' Activation record size: For printing the total memory requirement for each function\n");
        printf("Press '7' Static and dynamic arrays: For printing the type expressions and width of array variables\n");
        printf("Press '8' Errors reporting and total compiling time: For printing Syntactic and semantic error along with compile time\n");
        scanf(" %d",&choice);
        switch (choice)
        {
        case 0:
            break;
        case 1:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            struct Token *tk;
            do{
                tk = getNextToken();
                printToken(tk);
            }while(tk!=NULL);
            if(ptr != NULL) fclose(ptr);
            break;
        case 2:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            printTree(parserRoot);
            printf("\n\n");
            if(ptr != NULL) fclose(ptr);
            break;
        case 3:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            makeAST(parserRoot);
            printAST(astroot);
            if(ptr != NULL) fclose(ptr);
            break;
        case 4:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            makeAST(parserRoot);
            printAST(astroot);
            typeChecker(astroot);
            printSymbolTable();
            if(ptr != NULL) fclose(ptr);
            break;
        case 5:
            start_time = clock();

            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parse();

            end_time = clock();

            if(ptr != NULL)
                fclose(ptr);

            double total_CPU_time = (double) (end_time - start_time);
            double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("\ntotal_CPU_time := %lf ticks \ntotal_CPU_time := %lf s\n\n",total_CPU_time,total_CPU_time_in_seconds);
            
            break;
        default:
            printf("\nERROR: INVALID OPERATION CODE\n");
            break;
        }
    }while(choice);
}
