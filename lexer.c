#include "lexerDef.h"
#include "hash.h"

FILE *ptr;
int bufferSize, begin = 0, forward = 0, line = 1;
char* buffer1; char* buffer2;

FILE *getStream(FILE *fp){
    if (!fp){
        printf("ERROR: File not opened\n");
        exit(1);
    }
    for(int i=0;i<bufferSize;++i) buffer2[i]='\0';
    if (!feof(fp)) fread(buffer2, bufferSize - 1 , 1, fp);
    else return NULL;
    return fp;
}

struct Token Tokenize(int begin, int forward, tokentype type, int lineNo){
    char* s = malloc(2 * bufferSize * sizeof(char));
    int size = 0;
    struct Token tk;
    tk.type = type;
    tk.lineNo = lineNo;
    
    // assuming id size < BufferSize
    if (begin < forward){
        strncpy(s, buffer1 + begin, forward - begin);
        size = forward - begin;
    }
    else{
        strncpy(s, buffer1 + begin, bufferSize - 1 - begin);
        for (int i = 0; i < forward; i++)
        {
            s[bufferSize - 1 - begin + i] = buffer2[i];
        }
        s[bufferSize - 1 - begin + forward] = '\0';
        size = bufferSize - 1 - begin + forward;
    }
    s[size]='\0';

    // ID to keyword resolution
    if (type == TK_ID) {
        init_hash();
        int hashresult = search_hash(s);
        if (hashresult != -1) 
            tk.type = hashresult;
        strcpy(tk.val.identifier,s);
    }
    else if (type == TK_NUM){
        int r = 0, k = 1;
        for (int i = size-1; i >= 0; --i)
        {
            r += k * (s[i] - '0');
            k *= 10;
        }
        tk.val.integer = r;
    }
    else if (type == TK_RNUM){
        long long i,j;
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
            double pow10=1;
            if(s[i]=='+'){
                i++;
                for(int p=1;p<=size-i-1;++p){
                    pow10*=10;
                }
                int t = 0, k = pow10;
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                for(int p=1;p<=t;++p){
                    r*=10;
                }
            }
            else if(s[i]=='-'){
                i++;
                for(int p=1;p<=size-i-1;++p){
                    pow10*=10;
                }
                int t = 0, k = pow10;
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                for(int p=1;p<=t;++p){
                    r*=0.1;
                }
            }
            else{
                for(int p=1;p<=size-i-1;++p){
                    pow10*=10;
                }
                int t = 0, k = pow10;
                for(int j=i;j<size;j++){
                    t+= k*(s[j]-'0');
                    k/=10;
                }
                for(int p=1;p<=t;++p){
                    r*=10;
                }
            }
        }
        tk.val.decimal = r;
    }
    else
    {
        strcpy(tk.val.identifier,s);
    }
    return tk;
}

struct Token* getNextToken()
{
    struct Token* tk = (struct Token*)malloc(sizeof(struct Token));
    int flag=0;
    char ch = buffer1[forward];
    int state = 0;
    int is_exceeding=0;
    if(ch=='\0') return NULL;
    while (state != -1)
    {
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
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9')){
                if(forward<begin && (bufferSize-begin+forward+1 > 20)){
                    printf("ERROR: identifier length cannot exceed 20\n");
                    is_exceeding = 1;
                    begin = forward;
                    flag = 1;
                }else if(forward-begin>20){
                    printf("ERROR: identifier length cannot exceed 20\n");
                    is_exceeding = 1;
                    begin = forward;
                }
                forward++;
            }
            else
            {
                // if whitespace encountered
                if(is_exceeding){
                    is_exceeding = 0;
                    begin = forward;
                    state = 0;
                }else{
                    *tk = Tokenize(begin, forward, TK_ID, line);
                    begin = forward;
                    state = -1;
                }
            }
            break;

        // tokenize TK_PLUS
        case 2:
            *tk = Tokenize(begin, forward, TK_PLUS, line);
            begin = forward;
            state = -1;
            break;

        // tokenize TK_MINUS
        case 3:
            *tk = Tokenize(begin, forward, TK_MINUS, line);
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
                *tk = Tokenize(begin, forward, TK_NUM, line);
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
                forward --;
                *tk = Tokenize(begin, forward, TK_NUM, line);
                begin = forward;
                state = -1;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                state = 0;
                begin = forward;
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
                *tk = Tokenize(begin, forward, TK_RNUM, line);
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
                state = 0;
                begin = forward;
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
                state = 0;
                begin = forward;   
            }
            break;
        // scientific notation final state
        case 10:
            if ((ch >= '0' && ch <= '9'))
                forward++;
            else
            {
                *tk = Tokenize(begin, forward, TK_RNUM, line);
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
                state = 0;
                begin = forward;
            }
            break;
        
        case 12:
            *tk = Tokenize(begin, forward, TK_EQ, line);
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
                *tk = Tokenize(begin, forward, TK_MUL, line);
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
            else {
                if(ch == '\n') line++;
                forward++;
            }
            break;
        case 15:
            if(ch=='*'){
                //discard token
                forward++;
                begin=forward;
                state=0;
            }
            else {
                if(ch == '\n') line++;
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
                *tk = Tokenize(begin, forward, TK_LT, line);
                begin = forward;
                state = -1;
            }
            break;
        case 18:
            *tk = Tokenize(begin, forward, TK_LE, line);
            begin = forward;
            state = -1;
            break;
        
        case 19:
            if(ch=='<'){
                forward++;
                state=20;
            }
            else {
                *tk = Tokenize(begin, forward, TK_DEF, line);
                begin = forward;
                state = -1;
            }
            break;
            
        case 20:
            *tk = Tokenize(begin, forward, TK_DRIVERDEF, line);
            begin = forward;
            state = -1;
            break;
        //Shreekar ends
        // tokenize open sq bracket

        //Sriram Cases: 21 to 30
        //Tokenize Comma and Semicolon
        case 21:
            *tk = Tokenize(begin, forward, TK_COMMA, line);
            begin = forward;
            state = -1;
            break;

        case 22:
            *tk = Tokenize(begin, forward, TK_SEMICOLON, line);
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
                *tk = Tokenize(begin, forward, TK_COLON, line);
                begin = forward;
                state = -1;
            }
            break;

        case 24:
            *tk = Tokenize(begin, forward, TK_ASSIGNOP, line);
            begin = forward;
            state = -1;
            break;

        case 25:
            *tk = Tokenize(begin, forward, TK_BO, line);
            begin = forward;
            state = -1;
            break;

        case 26:
            *tk = Tokenize(begin, forward, TK_BC, line);
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
                *tk = Tokenize(begin, forward, TK_GT, line);
                begin = forward;
                state = -1;
            }
            break;
        case 27:
            *tk = Tokenize(begin, forward, TK_GE, line);
            begin = forward;
            state = -1;
            break;
        
        case 29:
            if(ch=='>'){
                forward++;
                state=30;
            }
            else {
                *tk = Tokenize(begin, forward, TK_ENDDEF, line);
                begin = forward;
                state = -1;
            }
            break;
            
        case 30:
            *tk = Tokenize(begin, forward, TK_DRIVERENDDEF, line);
            begin = forward;
            state = -1;
            break;

        case 31:
            *tk = Tokenize(begin, forward, TK_SQBO, line);
            begin = forward;
            state = -1;
            break;

        // tokenize close sq bracket
        case 32:
            *tk = Tokenize(begin, forward, TK_SQBC, line);
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
                state = 0;
                begin = forward;
            }

        // tokenize not equal
        case 34:
            *tk = Tokenize(begin, forward, TK_NE, line);
            forward++;
            begin = forward;
            state = -1;
            break;

        // tokenize div operator
        case 35:
            *tk = Tokenize(begin, forward, TK_DIV, line);
            begin = forward;
            state = -1;
            break;

        // error if not '.' occurs after '.'
        case 36:
            if (ch == '.')
            {
                state = 37;
            }
            else
            {
                printf("ERROR: Token not recognized at line %d\n", line);
                state = 0;
                begin = forward;
            }
            break;
        // tokenize range op
        case 37:
            forward++;
            *tk = Tokenize(begin, forward, TK_RANGEOP, line);
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
                printf("ERROR: Token not recognized at line %d\n", line);
                state = 0;
                begin = forward;
        }
        if((forward == bufferSize-1) && (state == 14)){
            begin = forward = 0;
            flag = 1;
        }
        forward %= bufferSize - 1;
        if(forward<begin){
            flag = 1;
            ch = buffer2[forward];
        }else {
            if(flag){
                for(int i=0;i<bufferSize;++i) buffer1[i]='\0';
                strcpy(buffer1,buffer2);
                if(ptr==NULL) return NULL;
                ptr = getStream(ptr);
                flag = 0;
            }
            ch = buffer1[forward];
        }
        if(ch=='\0') return NULL;
    }
    return tk;
}

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

FILE* initLexer(FILE* ptr, int buffSize) {
    begin = 0, forward = 0, line = 1;
    for(int i=0;i<bufferSize;++i){
        buffer1[i] = buffer2[i] = '\0';
    }
    bufferSize = buffSize;
    buffer1 = realloc(buffer1, bufferSize * sizeof(char));
    buffer2 = realloc(buffer2, bufferSize * sizeof(char));

    ptr = getStream(ptr);
    strcpy(buffer1, buffer2);
    ptr = getStream(ptr);

    return ptr;
}

void printToken(struct Token *tk){
    char *tokens[] = {"TK_ID","TK_PLUS","TK_MINUS","TK_NUM","TK_RNUM","TK_EQ","TK_MUL","TK_LT","TK_LE","TK_DEF","TK_DRIVERDEF","TK_COMMA","TK_SEMICOLON","TK_COLON","TK_ASSIGNOP","TK_BO","TK_BC","TK_GT","TK_GE","TK_ENDDEF","TK_DRIVERENDDEF","TK_SQBO","TK_SQBC","TK_NE","TK_DIV","TK_RANGEOP","TK_INTEGER","TK_REAL","TK_BOOLEAN","TK_OF","TK_ARRAY","TK_START","TK_END","TK_DECLARE","TK_MODULE","TK_DRIVER","TK_PROGRAM","TK_GET_VALUE","TK_PRINT","TK_USE","TK_WITH","TK_PARAMETERS","TK_TAKES","TK_INPUT","TK_RETURNS","TK_FOR","TK_IN","TK_SWITCH","TK_CASE","TK_BREAK","TK_DEFAULT","TK_WHILE","TK_AND","TK_OR","TK_TRUE","TK_FALSE","EPS"};
    if(tk==NULL) {
        printf("\t\t\t!!! END OF FILE !!!\n\n");
        return;
    }
    printf("Line_number := %d\t",tk->lineNo);
    if(tk -> type == TK_NUM){
        printf("lexeme := %d\t\t",tk->val.integer);
    }
    else if(tk -> type == TK_RNUM){
        printf("lexeme := %f\t\t",tk->val.decimal);
    }
    else{
        printf("lexeme := %s\t\t",tk->val.identifier);
    }
    printf("Token_name := %s\n",tokens[tk->type]);
}