#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hash[50];
int is_init=0;

int sumchars(char* s){
    int res=0;
    for(int i=0;s[i]!='\0';++i){
        res = (res+s[i]-'a');
    }
    return res;
}

void insert_hash(char* str){
    int ind = sumchars(str);
    while(hash[ind]!=NULL) ind++;
    hash[ind] = str;
}

int search_hash(char* str){
    int ind = sumchars(str);
    while(hash[ind]!=NULL){
        printf("%s == %s\n",str,hash[ind]);
        if(strcmp(str,hash[ind])==0) return 1;
        ind++;
    }
    return 0;
}

void init_hash(){   
    if(is_init==0){
        char *keywords[] = {"integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use", "with", "parameters", "takes", "input", "returns", "for", "in", "switch", "case", "break", "default", "while"};
        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) insert_hash(keywords[i]);
        is_init = 1;
    }
}

int main(){
    // char* keywords[] = {"integer","real","boolean","of","array","start","end","declare","module","driver","program","get_value","print","use","with","parameters","takes","input","returns","for","in","switch","case","break","default","while"};
    // for(int i=0;i<sizeof(keywords)/sizeof(keywords[0]);++i){
    //     insert(keywords[i]);
    //     // printf("%s - %d\n",keywords[i],sumchars(keywords[i]));
    // }
    init_hash();
    // printf("%d\n",search("stars"));
    printf("%d\n",search_hash("booleal"));
    return 0;
}