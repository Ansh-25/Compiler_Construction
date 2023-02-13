#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer1[32],buffer2[32];
FILE* ptr;

union TokenVal{
    int integer;
    float decimal;
    char* identifier;
};

struct Token{
    char* type;
    int lineNo;
    union TokenVal val;
};

int readBuffer(){
    if(!ptr){
        ptr = fopen("./Program.txt","r");
        if(!ptr){
            fprintf(stderr,"File cannot be opened\n");
            exit(1);
        }
    }
    if(!feof(ptr)) fread(buffer2,sizeof(buffer2)-1,1,ptr);
    else return -1;
    return 0;
}

void Tokenize(int begin,int forward,char* tokenType,int lineNo){
    char s[64];
    int size=0;
    struct Token tk;
    // assuming id size < 32
    if(begin<forward){
        strncpy(s,buffer1+begin,forward-begin+1);
        size = forward-begin;
    }else{
        strncpy(s,buffer1+begin,32-begin);
        for (int i = 0; i < forward; i++){
            s[32-begin+1+i] = buffer2[i];
        }
        s[32-begin+forward+1] = '\0';
        strcpy(buffer1,buffer2);
        readBuffer();
        size = 32-begin+forward;
    }
    tk.type = tokenType;
    tk.lineNo = lineNo;
    if(tokenType=="TK_NUM"){
        int r=0,k=1;
        for(int i=size;i>=0;--i){
            r+=k*(s[i]-'0');
            k*=10;
        }
        tk.val.integer = r;
    } else if(tokenType=="TK_RNUM"){
        // do something
    } else{
        tk.val.identifier = s; 
    }
    // call parser with token tk
    // parser(tk);
}

int isNum(char ch){
    return (ch>='0' && ch<='9');
}

void Lexer(){
    int begin=0,forward=0,line=0;
    char ch = buffer1[forward];
    int state=0; 
    while(state!=-1){
        switch(state){
            // start state
            case 0: 
                if((ch>='a' && ch<='z' ) || (ch>='A' && ch<='Z') || ch=='_') state = 1;
                else if(ch=='+') state = 2;
                else if(ch=='-') state = 3;
                else if(isNum(ch)) state = 4;
                else if(ch=='=') state = 11;
                else if(ch=='*') state = 13;
                else if(ch=='<') state = 17;
                else if(ch==',') state = 21;
                else if(ch==';') state = 22;
                else if(ch==':') state = 23;
                else if(ch=='(') state = 25;
                else if(ch==')') state = 26;
                else if(ch=='>') state = 28;
                else if(ch=='[') state = 31;
                else if(ch==']') state = 32;
                else if(ch=='!') state = 33;
                else if(ch=='/') state = 35;
                else if(ch=='.') state = 36;
                else if(ch=='\n') state = 38;
                else if(ch=='\b' || ch=='\t') state = 39;
                forward++;
                break;

            // ID/keyword final state
            case 1:
                if ((ch>='a' && ch<='z' ) || (ch>='A' && ch<='Z') || ch=='_' || isNum(ch)) {
                    state = 1;
                    forward++;
                } 
                // if whitespace encountered
                else{
                    tokenize(begin,forward,"TK_ID",line);
                    begin = forward;
                }
                break;

            // tokenize TK_PLUS
            case 2:
                tokenize(begin,forward,"TK_PLUS",line);
                forward++;
                begin = forward;
                break;

            default:
                printf("ERROR: State does not exist\n");
                exit(1);
                state = -1;
                break;
        }
        // character update
        if(forward<32) ch = buffer1[forward];
        else{
            // case when forward in buf2 nd begin in buf1
            forward %= 32;
            ch = buffer2[forward];
        }
    }
}


int main(){
    readBuffer();
    strcpy(buffer1,buffer2);
    readBuffer();
    // lexer();

    printf("%s\n",buffer1);
    printf("%s\n",buffer2);

    return 0;
}