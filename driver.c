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

bool compile_error;

int main(int argc, char *argv[]){
    int choice,size_of_buffer = 32; 
    clock_t start_time, end_time;
    char test_file[32],asm_file[32];
    
    ParseNode* parserRoot=NULL;
    strcpy(test_file,argv[1]);
    strcpy(asm_file,argv[2]);

    printf("\nIMPLEMENTATION STATUS\nLEVEL 4: Symbol table/ AST/ Semantic Rules work /handled static and dynamic arrays in type checking \n\n");
    do{
        printf("\t\t MAIN MENU \n");
        printf("Press '0' EXIT\n");
        printf("Press '1' Lexer\n");
        printf("Press '2' Parser \n");
        printf("Press '3' AST (Preorder) \n");
        printf("Press '4' Memory\n");
        printf("Press '5' Symbol Table\n");
        printf("Press '6' Activation record size\n");
        printf("Press '7' Static and dynamic arrays\n");
        printf("Press '8' Errors reporting and total compiling time\n");
        printf("Press '9' Code generation (Not Implemented)\n");
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
            printf("\n");
            if(ptr != NULL) fclose(ptr);
            break;
        case 4:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            int parse_nodes = countParseNodes(parserRoot);
            int parse_node_size = sizeof(*parserRoot);
            makeAST(parserRoot);
            int ast_node_size = sizeof(*astroot);
            int ast_nodes = countASTNodes(astroot);
            float compression = (parse_node_size*parse_nodes-ast_node_size*ast_nodes)/(1.0*parse_node_size*parse_nodes);
            printf("Parse tree Number of nodes = %d , Memory Allocated = %d Bytes\n",parse_nodes,parse_node_size*parse_nodes);
            printf("AST Number of nodes = %d , Memory Allocated = %d Bytes\n",ast_nodes,ast_node_size*ast_nodes);
            printf("Memory compression = %f %% \n\n", compression*100.0);
            if(ptr != NULL) fclose(ptr);
            break;
        case 5:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            makeAST(parserRoot);
            print_error=false;
            typeChecker(astroot);
            printSymbolTable();
            if(ptr != NULL) fclose(ptr);
            break;
        case 6:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            makeAST(parserRoot);
            print_error=false;
            typeChecker(astroot);
            calcActRecordSize();
            if(ptr != NULL) fclose(ptr);
            break;
        case 7:
            ptr = fopen(test_file,"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            parserRoot = parse();
            makeAST(parserRoot);
            print_error=false;
            typeChecker(astroot);
            printAllArrays();
            if(ptr != NULL) fclose(ptr);
            break;
        case 8:
            ptr = fopen(test_file,"r");
            start_time = clock();
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("grammar.txt");
            computefirstandfollow();
            createParseTable();
            compile_error = false;
            parserRoot = parse();
            if(compile_error==false){
                makeAST(parserRoot);
                print_error=true;
                typeChecker(astroot);
            }
            if(compile_error==false) printf("Program compiled succesfully\n"); 
            end_time = clock();
            if(ptr != NULL) fclose(ptr);
            double total_CPU_time = (double) (end_time - start_time);
            double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("\n\ntotal_CPU_time := %lf ticks \ntotal_CPU_time := %lf s\n\n",total_CPU_time,total_CPU_time_in_seconds);
            break;
        case 9:
            break;
        default:
            printf("\nERROR: INVALID OPERATION CODE\n");
            break;
        }
    }while(choice);
}
