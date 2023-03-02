#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parse.h"
#include <time.h>

int main(int argc, char *argv[]){
    int choice; 
    char test_file[32],parse_out_file[32],size_buff[32];

    strcpy(test_file,argv[1]);
    strcpy(parse_out_file,argv[2]);
    strcpy(size_buff,argv[3]);

    // converting string buffer size to integer
    int size_of_buffer = 0, mul = 1;
    for (int i = strlen(argv[3])-1; i >= 0; --i)
    {
        size_of_buffer += mul * (size_buff[i] - '0');
        mul *= 10;
    }

    // printing menu
    do{
        printf("IMPLEMENTATION STATUS \n\n");
        printf("\t\t MAIN MENU \n");
        printf("Press '0' For exiting the program\n");
        printf("Press '1' For removal of comments\n");
        printf("Press '2' For printing the token list generated by the lexer\n");
        printf("Press '3' For parsing the source code and printing the parse tree\n");
        printf("Press '4' For printing the total time taken by the lexer and parser\n");
        scanf(" %d",&choice);
        switch (choice)
        {
        case 0:
            exit(0);
            break;
        case 1:
            removeComments(test_file,"commentFreeFile.txt");
            FILE* commentFilePtr = fopen("commentFreeFile.txt","r");
            char comment_buff[50];
            if(commentFilePtr==NULL) printf("ERROR: comment free file cannot be opened\n");
            else{
                for(int i=0;i<50;++i) comment_buff[i]='\0';
                while(!feof(commentFilePtr)){
                    fread(comment_buff, sizeof(comment_buff) - 1 , 1, commentFilePtr);
                    printf("%s",comment_buff);
                    for(int i=0;i<50;++i) comment_buff[i]='\0';
                }
                printf("\n\n");
            }
            fclose(commentFilePtr);
            break;
        case 2:
            ptr = fopen(argv[1],"r");
            ptr = initLexer(ptr, size_of_buffer);
            struct Token *tk;
            do{
                tk = getNextToken();
                printToken(tk);
            }while(tk!=NULL);
            fclose(ptr);
            break;
        case 3:
            ptr = fopen(argv[1],"r");
            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("GrammarForParser");
            computefirstandfollow();
            createParseTable();
            TreeNode* root = parse();
            printTree(root,fopen(argv[2],"w"));
            printf("\n\n");
            fclose(ptr);
            break;
        case 4:
            ptr = fopen(argv[1],"r");
            clock_t start_time, end_time;

            start_time = clock();

            ptr = initLexer(ptr, size_of_buffer);
            loadgrammar("GrammarForParser");
            computefirstandfollow();
            createParseTable();
            parse();

            end_time = clock();

            fclose(ptr);

            double total_CPU_time = (double) (end_time - start_time);
            double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("\ntotal_CPU_time := %lf \ntotal_CPU_time_in_sec := %lf\n\n",total_CPU_time,total_CPU_time_in_seconds);
            
            break;
        default:
            printf("ERROR: INVALID OPERATION CODE\n");
            break;
        }
    }while(1);
}