#include <stdio.h>
#include <stdlib.h>

int test=0;
char* a[2];

void g() {
    if(test==0){
        test++;
        a[0] = (char*)malloc(sizeof(char));
        a[1] = (char*)malloc(sizeof(char));
        a[2] = (char*)malloc(sizeof(char));
        a[0] = "ab";
        a[1] = "cd";
        a[2] = "ef";
    }
}

void abc(int k) {
    g();
    printf("%s\n",a[k]);
}