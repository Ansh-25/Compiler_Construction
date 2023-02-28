#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeComments(char *testcaseFile, char *cleanFile){
    FILE* f1 = fopen(testcaseFile,"r");
    FILE* f2 = fopen(cleanFile,"w");
    if (f1 == NULL) {
        printf("file can't be opened \n");
        return;
    }
    char ch;
    while (!feof(f1)) {
        ch = fgetc(f1);
        if(ch == '*' && !feof(f1)){
            ch = fgetc(f1);
            if(ch == '*' && !feof(f1)){
                ch = fgetc(f1);
                while(!feof(f1)){   
                    if(ch=='\n'){
                        fputc(ch,f2);
                    } 
                    if(ch=='*' && !feof(f1)){
                        ch=fgetc(f1);
                        if(ch=='*')
                            break;
                        else
                            fseek(f1,-1,SEEK_CUR);
                    }                
                    ch = fgetc(f1);
                }
                continue;
            }
            fputc('*',f2);
            fseek(f1,-1,SEEK_CUR);
            continue;
        }
        fputc(ch,f2);
    }
    fseek(f2,-1,SEEK_END);
    fputc(' ',f2);
    fclose(f1);
    fclose(f2);
}

int main(){
    char* filename = "test.txt";
    char* newfile = "temp.txt";
    removeComments(filename,newfile);
}