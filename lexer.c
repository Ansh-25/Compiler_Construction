#include "lexerDef.h"
#include "hash.h"
#include <math.h>

char buffer1[32], buffer2[32];
FILE *ptr;
int begin = 0, forward = 0, line = 1;
int m = 0;

// check fp for eof
FILE *getStream(FILE *fp){
    if (!fp){
        fp = fopen("testprog.txt", "r");
        if (!fp){
            printf("File cannot be opened\n");
            exit(1);
        }
    }
    for(int i=0;i<32;++i) buffer2[i]='\0'; 
    if (!feof(fp)) fread(buffer2, sizeof(buffer2) - 1 , 1, fp);
    else return NULL;
    return fp;
}

struct Token Tokenize(int begin, int forward, char *tokenType, int lineNo){
    // printf("begin:= %d  end:= %d  type:= %s  line:=%d  ",begin,forward,tokenType,lineNo);
    char s[64];
    int size = 0;
    struct Token tk;
    tk.type = tokenType;
    tk.lineNo = lineNo;
    init_hash();
    // assuming id size < 32
    if (begin < forward){
        strncpy(s, buffer1 + begin, forward - begin);
        size = forward - begin;
    }
    else{
        strncpy(s, buffer1 + begin, 31 - begin);
        for (int i = 0; i < forward; i++)
        {
            s[31 - begin + i] = buffer2[i];
        }
        s[31 - begin + forward] = '\0';
        size = 31 - begin + forward;
    }
    s[size]='\0';

    // ID to keyword resolution
    if (tokenType == "TK_ID" && search_hash(s)){
        strcpy(tk.val.identifier,s);
        int i = 0;
        // capitalize s
        char temp[64];
        while (i<size) temp[i++] = s[i] + 'A' - 'a';
        temp[i]='\0';
        tk.type = temp;
    }
    else if (tokenType == "TK_NUM"){
        int r = 0, k = 1;
        for (int i = size-1; i >= 0; --i)
        {
            r += k * (s[i] - '0');
            k *= 10;
        }
        if(m)
            r*=-1;
        tk.val.integer = r;
    }
    else if (tokenType == "TK_RNUM"){
        int i,j;
        for(i=0;i<size;i++){
            if(s[i]=='.')
            break;
        }
        double r = 0, k = 1;
        for (int j = i-1; j >= 0; --j){
            r += k * (s[j] - '0');
            k *= 10;
        }
        k=0.1;
        for(j=i+1;j<size;j++){
            if(s[j]=='e' || s[j]=='E') break;
            r+= k*(s[j]-'0');
            k/=10;
        }
        if(j!=size){
            i=++j;
            if(s[i]=='+'){
                i++;
                int t = 0, k = pow(10,size-i-1);
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                r*=pow(10,t);
                //printf("%f",r);
            }
            else if(s[i]=='-'){
                i++;
                int t = 0, k = pow(10,size-i-1);
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                r*=pow(0.1,t);
                //printf("\n %lf",r);
            }
            else{
                int t = 0, k = pow(10,size-i-1);
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                r*=pow(10,t);
                //printf("%f",r);
            }
        }
        tk.val.decimal = r;
    }
    else
    {
        // printf("hi");
        strcpy(tk.val.identifier,s);
    }
    // printf("type:= %s  val := %s\n",tk.type,s);
    return tk;
}

struct Token* getNextToken()
{
    struct Token* tk = malloc(sizeof(struct Token));
    int flag=0;
    char ch = buffer1[forward];
    int state = 0;
    while (state != -1)
    {
        // printf("state := %d ",state);
        switch (state)
        {
        // start state
        case 0:
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
                state = 1;
            else if (ch == '+')
                state = 2;
            else if (ch == '-')
                state = 3;
            else if (ch >= '0' && ch <= '9')
                state = 4;
            else if (ch == '=')
                state = 11;
            else if (ch == '*')
                state = 13;
            else if (ch == '<')
                state = 17;
            else if (ch == ',')
                state = 21;
            else if (ch == ';')
                state = 22;
            else if (ch == ':')
                state = 23;
            else if (ch == '(')
                state = 25;
            else if (ch == ')')
                state = 26;
            else if (ch == '>')
                state = 28;
            else if (ch == '[')
                state = 31;
            else if (ch == ']')
                state = 32;
            else if (ch == '!')
                state = 33;
            else if (ch == '/')
                state = 35;
            else if (ch == '.')
                state = 36;
            else if (ch == '\n')
                state = 38;
            else if (ch == '\b' || ch == '\t' || ch==32)
                state = 39;
            forward++;
            break;

        // ID/keyword final state
        case 1:
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9'))
                forward++;
            // if whitespace encountered
            else
            {
                *tk = Tokenize(begin, forward, "TK_ID", line);
                if(m)
                    m=0;
                begin = forward;
                state = -1;
            }
            break;

        // tokenize TK_PLUS
        case 2:
            *tk = Tokenize(begin, forward, "TK_PLUS", line);
            forward++;
            begin = forward;
            state = -1;
            break;

        // tokenize TK_MINUS
        case 3:
            *tk = Tokenize(begin, forward, "TK_MINUS", line);
            //forward++;
            m=1;
            begin = forward;
            state = -1;
            break;

        // ----------------------------------------Numbers------------------------------------------

        // TK_NUM final state
        case 4:
            if (ch >= '0' && ch <= '9')
                forward++;
            else if (ch == '.') 
            {
                forward++;
                state = 5;
            }
            else
            {
                *tk = Tokenize(begin, forward, "TK_NUM", line);
                begin = forward;
                state = -1;
            }
            break;

        // dot after number
        case 5:
            if ((ch >= '0' && ch <= '9'))
            {
                state = 7;
                forward++;
            }
            else if (ch == '.')
            {
                *tk = Tokenize(begin, forward - 2, "TK_NUM", line);
                forward -= 1;
                begin = forward;
                state = -1;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }
            break;

        // -------------------------       ELIMINATED CASE 6 ... FUNCTIONALITY PROVIDED IN CASE 5    -------------------------

        // floating point number final state
        case 7:
            if (ch >= '0' && ch <= '9')
                forward++;
            else if (ch == 'E' || ch == 'e')
            {
                forward++;
                state = 8;
            }
            else
            {
                *tk = Tokenize(begin, forward, "TK_RNUM", line);
                begin = forward;
                state = -1;
            }
            break;
        // <num>.<num>E
        case 8:
            if (ch >= '0' && ch <= '9')
            {
                forward++;
                state = 10;
            }
            else if (ch == '+' || ch == '-')
            {
                state = 9;
                forward++;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }
            break;
        // <num>.<num>E(+/-)
        case 9:
            if (ch >= '0' && ch <= '9')
            {
                forward++;
                state = 10;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }
            break;
        // scientific notation final state
        case 10:
            if ((ch >= '0' && ch <= '9'))
                forward++;
            else
            {
                *tk = Tokenize(begin, forward, "TK_RNUM", line);
                begin = forward;
                state = -1;
            }
            break;
        //-----------------------------------------Numbers End---------------------------------------------
        //Shreekar starts
        //tokenise ==
        case 11:
            if(ch=='=') {
                forward++;
                state = 12;
            }
            else {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }
            break;
        
        case 12:
            *tk = Tokenize(begin, forward, "TK_EQ", line);
            forward++;
            begin = forward;
            state = -1;
            break;
        //tokenise comments
        case 13:
            if(ch=='*'){
                forward++;
                state=14;
            }
            else{
                *tk = Tokenize(begin, forward, "TK_MUL", line);
                begin = forward;
                state = -1;
            }
            break;
            //keep moving forward until we reach 2 *s
        case 14:
            if(ch=='*'){
                forward++;
                state=15;
            }
            else {forward++;}
            break;
        case 15:
            if(ch=='*'){
                //discard token
                forward++;
                begin=forward;
                state=0;
            }
            else {
                forward++;
                state=14;
            }
            break;
        //ignoring state 16 coz I did it in state 17
        //tokenise <,<<,<<<,<=
        case 17:
            if(ch=='<'){
                forward++;
                state=19;
            }
            else if(ch=='=') {
                forward++;
                state=18;
            }
            else {
                *tk = Tokenize(begin, forward, "TK_LT", line);
                begin = forward;
                state = -1;
            }
            break;
        case 18:
            *tk = Tokenize(begin, forward, "TK_LE", line);
            begin = forward;
            state = -1;
            break;
        
        case 19:
            if(ch=='<'){
                forward++;
                state=20;
            }
            else {
                *tk = Tokenize(begin, forward, "TK_DEF", line);
                begin = forward;
                state = -1;
            }
            break;
            
        case 20:
            *tk = Tokenize(begin, forward, "TK_DRIVERDEF", line);
            begin = forward;
            state = -1;
            break;
        //Shreekar ends
        // tokenize open sq bracket

        //Sriram Cases: 21 to 30
        //Tokenize Comma and Semicolon
        case 21:
            *tk = Tokenize(begin, forward, "TK_COMMA", line);
            begin = forward;
            state = -1;
            break;

        case 22:
            *tk = Tokenize(begin, forward, "TK_SEMICOLON", line);
            begin = forward;
            state = -1;
            break;

        //Tokenize :, :=, ( and )
        case 23:
            if (ch == '='){
                forward++;
                state = 24;
            }
            else{
                *tk = Tokenize(begin, forward, "TK_COLON", line);
                begin = forward;
                state = -1;
            }
            break;

        case 24:
            *tk = Tokenize(begin, forward, "TK_ASSIGNOP", line);
            begin = forward;
            state = -1;
            break;

        case 25:
            *tk = Tokenize(begin, forward, "TK_BO", line);
            begin = forward;
            state = -1;
            break;

        case 26:
            *tk = Tokenize(begin, forward, "TK_BC", line);
            begin = forward;
            state = -1;
            break;

        //Tokenize >=, >, >>, >>>
        case 28:
            if(ch=='='){
                forward++;
                state=27;
            }
            else if(ch=='>') {
                forward++;
                state=29;
            }
            else {
                *tk = Tokenize(begin, forward, "TK_GT", line);
                begin = forward;
                state = -1;
            }
            break;
        case 27:
            *tk = Tokenize(begin, forward, "TK_GE", line);
            begin = forward;
            state = -1;
            break;
        
        case 29:
            if(ch=='>'){
                forward++;
                state=30;
            }
            else {
                *tk = Tokenize(begin, forward, "TK_ENDDEF", line);
                begin = forward;
                state = -1;
            }
            break;
            
        case 30:
            *tk = Tokenize(begin, forward, "TK_DRIVERENDDEF", line);
            begin = forward;
            state = -1;
            break;

        case 31:
            *tk = Tokenize(begin, forward, "TK_SQBO", line);
            begin = forward;
            state = -1;
            break;

        // tokenize close sq bracket
        case 32:
            *tk = Tokenize(begin, forward - 1, "TK_SQBC", line);
            begin = forward;
            state = -1;
            break;

        // error if not '=' occurs after '!'
        case 33:
            if (ch == '=')
            {
                state = 34;
                break;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }

        // tokenize not equal
        case 34:
            *tk = Tokenize(begin, forward, "TK_NE", line);
            forward++;
            begin = forward;
            state = -1;
            break;

        // tokenize div operator
        case 35:
            *tk = Tokenize(begin, forward - 1, "TK_DIV", line);
            begin = forward;
            state = -1;
            break;

        // error if not '.' occurs after '.'
        case 36:
            if (ch == '.')
            {
                state = 37;
                break;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                exit(1);
            }

        // tokenize range op
        case 37:
            *tk = Tokenize(begin, forward, "TK_RANGEOP", line);
            forward++;
            begin = forward;
            state = -1;
            break;

        // updates line no
        case 38:
            line++;
            state = 0;
            begin = forward;
            break;

        // ignores whitespaces
        case 39:
            if (ch == '\b' || ch == '\t')
            {
                forward++;
                state = 39;
                break;
            }
            else
            {
                begin = forward;
                state = 0;
                break;
            }

        default:
            printf("ERROR: State does not exist\n");
            exit(1);
        }
        // printf("%d,%d\n",begin,forward);
        forward %=31;
        if(forward<begin){
            flag = 1;
            ch = buffer2[forward];
        }else {
            if(flag){
                strcpy(buffer1,buffer2);
                if(ptr==NULL) return NULL;
                ptr = getStream(ptr);
                flag = 0;
            }
            ch = buffer1[forward];
        }
    }
    return tk;
}

int main()
{
    ptr = getStream(ptr);
    strcpy(buffer1, buffer2);
    ptr = getStream(ptr);

    struct Token *tk;
    do{
        tk = getNextToken();
        printToken(tk);
    }while(tk!=NULL);
    return 0;
}