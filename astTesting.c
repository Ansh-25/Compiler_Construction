#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {TK_ID, TK_PLUS, TK_MINUS, TK_NUM, TK_RNUM, TK_EQ, TK_MUL, TK_LT, TK_LE, TK_DEF, TK_DRIVERDEF, TK_COMMA, TK_SEMICOLON, TK_COLON, TK_ASSIGNOP, TK_BO, TK_BC, TK_GT, TK_GE, TK_ENDDEF, TK_DRIVERENDDEF, TK_SQBO, TK_SQBC, TK_NE, TK_DIV, TK_RANGEOP, TK_INTEGER, TK_REAL, TK_BOOLEAN, TK_OF, TK_ARRAY, TK_START, TK_END, TK_DECLARE, TK_MODULE, TK_DRIVER, TK_PROGRAM, TK_GET_VALUE, TK_PRINT, TK_USE, TK_WITH, TK_PARAMETERS, TK_TAKES, TK_INPUT, TK_RETURNS, TK_FOR, TK_IN, TK_SWITCH, TK_CASE, TK_BREAK, TK_DEFAULT, TK_WHILE, TK_AND, TK_OR, TK_TRUE, TK_FALSE, TK_EOF, EPS}tokentype;

union TokenVal
{
    int integer;
    double decimal;
    char identifier[32];
};

struct Token    
{
    tokentype type;
    int lineNo;
    union TokenVal val;
};

struct hashentry {
    char str[20];
    tokentype tk;
};

int is_init=0;
struct hashentry* hash[47];

int sumchars(char* s){
    int res=0;
    for(int i=0;s[i]!='\0';++i){
        res = (res+s[i])%47;
    }
    return res;
}

void insert_hash(char* str, tokentype tk){
    int ind = sumchars(str);
    while(hash[ind] != NULL) {ind++; ind %=47;}
    hash[ind] = (struct hashentry*)malloc(sizeof(struct hashentry));
    strcpy(hash[ind] -> str,str);
    hash[ind] -> tk = tk;
}

int search_hash(char* str){
    int ind = sumchars(str);
    // printf("hello := %d\n",hash[ind]!=NULL);
    while(hash[ind]!=NULL){
        // printf("%s == %s\n",str,hash[ind]->str);
        // printf("%s",hash[ind]);
        if(strcmp(str,hash[ind] -> str)==0) return hash[ind] -> tk;
        ind++;
        ind %= 47;
    }
    //printf("hello\n");
    return -1;
}

void init_hash()
{ 
    if(is_init==0){
        char *keywords[] = {"integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use", "with", "parameters", "takes", "input", "returns", "for", "in", "switch", "case", "break", "default", "while", "AND", "OR", "true", "false"};
        tokentype keywordenum[] = {TK_INTEGER, TK_REAL, TK_BOOLEAN, TK_OF, TK_ARRAY, TK_START, TK_END, TK_DECLARE, TK_MODULE, TK_DRIVER, TK_PROGRAM, TK_GET_VALUE, TK_PRINT, TK_USE, TK_WITH, TK_PARAMETERS, TK_TAKES, TK_INPUT, TK_RETURNS, TK_FOR, TK_IN, TK_SWITCH, TK_CASE, TK_BREAK, TK_DEFAULT, TK_WHILE, TK_AND, TK_OR, TK_TRUE, TK_FALSE};
        for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) insert_hash(keywords[i],keywordenum[i]);
        is_init ++;
    }
}

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
            else {
                printf("\nERROR: Symbol not recognized at line %d\n\n", line);
                begin ++;
            }
            forward++;
            break;

        // ID/keyword final state
        case 1:
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9')){
                if(forward<begin && (bufferSize-begin+forward+1 > 20)){
                    if(is_exceeding==0) printf("\nERROR: identifier length cannot exceed 20\n\n");
                    is_exceeding = 1;
                    begin = forward;
                    flag = 1;
                }else if(forward-begin>20){
                    if(is_exceeding==0) printf("\nERROR: identifier length cannot exceed 20\n\n");
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
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
                printf("\nERROR: Token not recognized at line %d\n\n", line);
                state = 0;
                begin = forward;
        }
        if((forward == bufferSize-1) && (state == 14 || state == 15)){
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
        if(state==0 && ch=='\0') return NULL;
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
        printf("lexeme := %30.17lf\t\t",tk->val.decimal);
    }
    else{
        printf("lexeme := %s\t\t",tk->val.identifier);
    }
    printf("Token_name := %s\n",tokens[tk->type]);
}


typedef enum {program, moduleDeclarations, otherModules, driverModule, moduleDeclaration, module, moduleDef, input_plist, ret, output_plist, dataType, _input_plist, _type, _output_plist, range_arrays, index_arr, statements, statement, ioStmt, simpleStmt, declareStmt, conditionalStmt, iterativeStmt, var_print, P1, boolConstt, new_index, assignmentStmt, moduleReuseStmt, whichStmt, lvalueIDStmt, lvalueARRStmt, expression, element_index_with_expressions, sign, optional, actual_para_list, K, N_12, N_11, idList, N3, arithmeticOrBooleanExpr, U, unary_op, new_NT, arithmeticExpr, var_id_num, AnyTerm, N7, logicalOp, N8, relationalOp, term, N4, op1, factor, N5, op2, N_10, arrExpr, arrTerm, arr_N4, arr_N5, arrFactor, caseStmts, _default, value, N9, range_for_loop, index_for_loop, sign_for_loop, new_index_for_loop}non_terminal;
typedef enum {TERMINAL, NONTERMINAL}type;
typedef struct ASTNode ASTNode;

union grammaritem {
    non_terminal nt;
    tokentype t;
};

typedef struct grammarchar {
    union grammaritem g;
    type t;
}grammarchar;

typedef struct ListNode {
    grammarchar val;
    struct ListNode* next;
}ListNode;

union treenodeval {
    non_terminal nt;
    struct Token* t;
};

typedef struct ParseNode {
    type t;
    int ruleno;
    struct ASTNode* addr;
    union treenodeval val;
    struct ParseNode* child;
    struct ParseNode* sibling;
}ParseNode;

typedef struct StackNode {
    struct ParseNode* val;
    struct StackNode* next;
}StackNode;

#define MAXTERMLEN 50 
#define NO_RULES 139 //No. of grammar rules: 143
#define NO_TERMS 58 //No. of terminals: 58
#define NO_NONTERMS 73 //No. of Non Terminals: 73
#define MAXTERMLEN 50 

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];
struct ListNode* synchronizingSet[NO_NONTERMS];

tokentype mapttoenum(char* s) { //maps string of token name with token
    if (strcmp(s,"ID") == 0) return TK_ID;
    if (strcmp(s,"PLUS") == 0) return TK_PLUS;
    if (strcmp(s,"MINUS") == 0) return TK_MINUS;
    if (strcmp(s,"NUM") == 0) return TK_NUM;
    if (strcmp(s,"RNUM") == 0) return TK_RNUM;
    if (strcmp(s,"EQ") == 0) return TK_EQ;
    if (strcmp(s,"MUL") == 0) return TK_MUL;
    if (strcmp(s,"LT") == 0) return TK_LT;
    if (strcmp(s,"LE") == 0) return TK_LE;
    if (strcmp(s,"DEF") == 0) return TK_DEF;
    if (strcmp(s,"DRIVERDEF") == 0) return TK_DRIVERDEF;
    if (strcmp(s,"COMMA") == 0) return TK_COMMA;
    if (strcmp(s,"SEMICOL") == 0) return TK_SEMICOLON;
    if (strcmp(s,"COLON") == 0) return TK_COLON;
    if (strcmp(s,"ASSIGNOP") == 0) return TK_ASSIGNOP;
    if (strcmp(s,"BO") == 0) return TK_BO;
    if (strcmp(s,"BC") == 0) return TK_BC;
    if (strcmp(s,"GT") == 0) return TK_GT;
    if (strcmp(s,"GE") == 0) return TK_GE;
    if (strcmp(s,"ENDDEF") == 0) return TK_ENDDEF;
    if (strcmp(s,"DRIVERENDDEF") == 0) return TK_DRIVERENDDEF;
    if (strcmp(s,"SQBO") == 0) return TK_SQBO;
    if (strcmp(s,"SQBC") == 0) return TK_SQBC;
    if (strcmp(s,"NE") == 0) return TK_NE;
    if (strcmp(s,"DIV") == 0) return TK_DIV;
    if (strcmp(s,"RANGEOP") == 0) return TK_RANGEOP;
    if (strcmp(s,"INTEGER") == 0) return TK_INTEGER;
    if (strcmp(s,"REAL") == 0) return TK_REAL;
    if (strcmp(s,"BOOLEAN") == 0) return TK_BOOLEAN;
    if (strcmp(s,"OF") == 0) return TK_OF;
    if (strcmp(s,"ARRAY") == 0) return TK_ARRAY;
    if (strcmp(s,"START") == 0) return TK_START;
    if (strcmp(s,"END") == 0) return TK_END;
    if (strcmp(s,"DECLARE") == 0) return TK_DECLARE;
    if (strcmp(s,"MODULE") == 0) return TK_MODULE;
    if (strcmp(s,"DRIVER") == 0) return TK_DRIVER;
    if (strcmp(s,"PROGRAM") == 0) return TK_PROGRAM;
    if (strcmp(s,"GET_VALUE") == 0) return TK_GET_VALUE;
    if (strcmp(s,"PRINT") == 0) return TK_PRINT;
    if (strcmp(s,"USE") == 0) return TK_USE;
    if (strcmp(s,"WITH") == 0) return TK_WITH;
    if (strcmp(s,"PARAMETERS") == 0) return TK_PARAMETERS;
    if (strcmp(s,"TAKES") == 0) return TK_TAKES;
    if (strcmp(s,"INPUT") == 0) return TK_INPUT;
    if (strcmp(s,"RETURNS") == 0) return TK_RETURNS;
    if (strcmp(s,"FOR") == 0) return TK_FOR;
    if (strcmp(s,"IN") == 0) return TK_IN;
    if (strcmp(s,"SWITCH") == 0) return TK_SWITCH;
    if (strcmp(s,"CASE") == 0) return TK_CASE;
    if (strcmp(s,"BREAK") == 0) return TK_BREAK;
    if (strcmp(s,"DEFAULT") == 0) return TK_DEFAULT;
    if (strcmp(s,"WHILE") == 0) return TK_WHILE;
    if (strcmp(s,"AND") == 0) return TK_AND;
    if (strcmp(s,"OR") == 0) return TK_OR;
    if (strcmp(s,"TRUE") == 0) return TK_TRUE;
    if (strcmp(s,"FALSE") == 0) return TK_FALSE;
    if (strcmp(s,"EPS") == 0) return EPS;
    return -1;
}

non_terminal mapnttoenum(char* s) { //maps string of non-terminal to nonterminal 
    if (strcmp(s,"program") == 0) return program;
    else if (strcmp(s,"moduleDeclarations") == 0) return moduleDeclarations;
    else if (strcmp(s,"otherModules") == 0) return otherModules;
    else if (strcmp(s,"driverModule") == 0) return driverModule;
    else if (strcmp(s,"moduleDeclaration") == 0) return moduleDeclaration;
    else if (strcmp(s,"module") == 0) return module;
    else if (strcmp(s,"moduleDef") == 0) return moduleDef;
    else if (strcmp(s,"input_plist") == 0) return input_plist;
    else if (strcmp(s,"ret") == 0) return ret;
    else if (strcmp(s,"output_plist") == 0) return output_plist;
    else if (strcmp(s,"dataType") == 0) return dataType;
    else if (strcmp(s,"_input_plist") == 0) return _input_plist;
    else if (strcmp(s,"_type") == 0) return _type;
    else if (strcmp(s,"_output_plist") == 0) return _output_plist;
    else if (strcmp(s,"range_arrays") == 0) return range_arrays;
    else if (strcmp(s,"index_arr") == 0) return index_arr;
    else if (strcmp(s,"statements") == 0) return statements;
    else if (strcmp(s,"statement") == 0) return statement;
    else if (strcmp(s,"ioStmt") == 0) return ioStmt;
    else if (strcmp(s,"simpleStmt") == 0) return simpleStmt;
    else if (strcmp(s,"declareStmt") == 0) return declareStmt;
    else if (strcmp(s,"conditionalStmt") == 0) return conditionalStmt;
    else if (strcmp(s,"iterativeStmt") == 0) return iterativeStmt;
    else if (strcmp(s,"var_print") == 0) return var_print;
    else if (strcmp(s,"P1") == 0) return P1;
    else if (strcmp(s,"boolConstt") == 0) return boolConstt;
    else if (strcmp(s,"new_index") == 0) return new_index;
    else if (strcmp(s,"assignmentStmt") == 0) return assignmentStmt;
    else if (strcmp(s,"moduleReuseStmt") == 0) return moduleReuseStmt;
    else if (strcmp(s,"whichStmt") == 0) return whichStmt;
    else if (strcmp(s,"lvalueIDStmt") == 0) return lvalueIDStmt;
    else if (strcmp(s,"lvalueARRStmt") == 0) return lvalueARRStmt;
    else if (strcmp(s,"expression") == 0) return expression;
    else if (strcmp(s,"element_index_with_expressions") == 0) return element_index_with_expressions;
    else if (strcmp(s,"sign") == 0) return sign;
    else if (strcmp(s,"optional") == 0) return optional;
    else if (strcmp(s,"actual_para_list") == 0) return actual_para_list;
    else if (strcmp(s,"K") == 0) return K;
    else if (strcmp(s,"N_12") == 0) return N_12;
    else if (strcmp(s,"N_11") == 0) return N_11;
    else if (strcmp(s,"idList") == 0) return idList;
    else if (strcmp(s,"N3") == 0) return N3;
    else if (strcmp(s,"arithmeticOrBooleanExpr") == 0) return arithmeticOrBooleanExpr;
    else if (strcmp(s,"U") == 0) return U;
    else if (strcmp(s,"unary_op") == 0) return unary_op;
    else if (strcmp(s,"new_NT") == 0) return new_NT;
    else if (strcmp(s,"arithmeticExpr") == 0) return arithmeticExpr;
    else if (strcmp(s,"var_id_num") == 0) return var_id_num;
    else if (strcmp(s,"AnyTerm") == 0) return AnyTerm;
    else if (strcmp(s,"N7") == 0) return N7;
    else if (strcmp(s,"logicalOp") == 0) return logicalOp;
    else if (strcmp(s,"N8") == 0) return N8;
    else if (strcmp(s,"relationalOp") == 0) return relationalOp;
    else if (strcmp(s,"term") == 0) return term;
    else if (strcmp(s,"N4") == 0) return N4;
    else if (strcmp(s,"op1") == 0) return op1;
    else if (strcmp(s,"factor") == 0) return factor;
    else if (strcmp(s,"N5") == 0) return N5;
    else if (strcmp(s,"op2") == 0) return op2;
    else if (strcmp(s,"N_10") == 0) return N_10;
    else if (strcmp(s,"arrExpr") == 0) return arrExpr;
    else if (strcmp(s,"arrTerm") == 0) return arrTerm;
    else if (strcmp(s,"arr_N4") == 0) return arr_N4;
    else if (strcmp(s,"arr_N5") == 0) return arr_N5;
    else if (strcmp(s,"arrFactor") == 0) return arrFactor;
    else if (strcmp(s,"caseStmts") == 0) return caseStmts;
    else if (strcmp(s,"_default") == 0) return _default;
    else if (strcmp(s,"value") == 0) return value;
    else if (strcmp(s,"N9") == 0) return N9;
    else if (strcmp(s,"range_for_loop") == 0) return range_for_loop;
    else if (strcmp(s,"index_for_loop") == 0) return index_for_loop;
    else if (strcmp(s,"sign_for_loop") == 0) return sign_for_loop;
    else if (strcmp(s,"new_index_for_loop") == 0) return new_index_for_loop;
    else return -1;
}

char* mapttokentostring(tokentype t) { //maps token to string of token name
    if (t == TK_ID) return "TK_ID";
    if (t == TK_PLUS) return "TK_PLUS";
    if (t == TK_MINUS) return "TK_MINUS";
    if (t == TK_NUM) return "TK_NUM";
    if (t == TK_RNUM) return "TK_RNUM";
    if (t == TK_EQ) return "TK_EQ";
    if (t == TK_MUL) return "TK_MUL";
    if (t == TK_LT) return "TK_LT";
    if (t == TK_LE) return "TK_LE";
    if (t == TK_DEF) return "TK_DEF";
    if (t == TK_DRIVERDEF) return "TK_DRIVERDEF";
    if (t == TK_COMMA) return "TK_COMMA";
    if (t == TK_SEMICOLON) return "TK_SEMICOLON";
    if (t == TK_COLON) return "TK_COLON";
    if (t == TK_ASSIGNOP) return "TK_ASSIGNOP";
    if (t == TK_BO) return "TK_BO";
    if (t == TK_BC) return "TK_BC";
    if (t == TK_GT) return "TK_GT";
    if (t == TK_GE) return "TK_GE";
    if (t == TK_ENDDEF) return "TK_ENDDEF";
    if (t == TK_DRIVERENDDEF) return "TK_DRIVERENDDEF";
    if (t == TK_SQBO) return "TK_SQBO";
    if (t == TK_SQBC) return "TK_SQBC";
    if (t == TK_NE) return "TK_NE";
    if (t == TK_DIV) return "TK_DIV";
    if (t == TK_RANGEOP) return "TK_RANGEOP";
    if (t == TK_INTEGER) return "TK_INTEGER";
    if (t == TK_REAL) return "TK_REAL";
    if (t == TK_BOOLEAN) return "TK_BOOLEAN";
    if (t == TK_OF) return "TK_OF";
    if (t == TK_ARRAY) return "TK_ARRAY";
    if (t == TK_START) return "TK_START";
    if (t == TK_END) return "TK_END";
    if (t == TK_DECLARE) return "TK_DECLARE";
    if (t == TK_MODULE) return "TK_MODULE";
    if (t == TK_DRIVER) return "TK_DRIVER";
    if (t == TK_PROGRAM) return "TK_PROGRAM";
    if (t == TK_GET_VALUE) return "TK_GET_VALUE";
    if (t == TK_PRINT) return "TK_PRINT";
    if (t == TK_USE) return "TK_USE";
    if (t == TK_WITH) return "TK_WITH";
    if (t == TK_PARAMETERS) return "TK_PARAMETERS";
    if (t == TK_TAKES) return "TK_TAKES";
    if (t == TK_INPUT) return "TK_INPUT";
    if (t == TK_RETURNS) return "TK_RETURNS";
    if (t == TK_FOR) return "TK_FOR";
    if (t == TK_IN) return "TK_IN";
    if (t == TK_SWITCH) return "TK_SWITCH";
    if (t == TK_CASE) return "TK_CASE";
    if (t == TK_BREAK) return "TK_BREAK";
    if (t == TK_DEFAULT) return "TK_DEFAULT";
    if (t == TK_WHILE) return "TK_WHILE";
    if (t == TK_AND) return "TK_AND";
    if (t == TK_OR) return "TK_OR";
    if (t == TK_TRUE) return "TK_TRUE";
    if (t == TK_FALSE) return "TK_FALSE";
    if (t == TK_EOF) return "TK_EOF";
    if (t == EPS) return "EPS";
    return NULL;
}

char* mapnttostring(non_terminal n) { //maps non-terminal to string of non-terminal name
    if (n == program) return "program";
    if (n == moduleDeclarations) return "moduleDeclarations";
    if (n == otherModules) return "otherModules";
    if (n == driverModule) return "driverModule";
    if (n == moduleDeclaration) return "moduleDeclaration";
    if (n == module) return "module";
    if (n == moduleDef) return "moduleDef";
    if (n == input_plist) return "input_plist";
    if (n == ret) return "ret";
    if (n == output_plist) return "output_plist";
    if (n == dataType) return "dataType";
    if (n == _input_plist) return "_input_plist";
    if (n == _type) return "_type";
    if (n == _output_plist) return "_output_plist";
    if (n == range_arrays) return "range_arrays";
    if (n == index_arr) return "index_arr";
    if (n == statements) return "statements";
    if (n == statement) return "statement";
    if (n == ioStmt) return "ioStmt";
    if (n == simpleStmt) return "simpleStmt";
    if (n == declareStmt) return "declareStmt";
    if (n == conditionalStmt) return "conditionalStmt";
    if (n == iterativeStmt) return "iterativeStmt";
    if (n == var_print) return "var_print";
    if (n == P1) return "P1";
    if (n == boolConstt) return "boolConstt";
    if (n == new_index) return "new_index";
    if (n == assignmentStmt) return "assignmentStmt";
    if (n == moduleReuseStmt) return "moduleReuseStmt";
    if (n == whichStmt) return "whichStmt";
    if (n == lvalueIDStmt) return "lvalueIDStmt";
    if (n == lvalueARRStmt) return "lvalueARRStmt";
    if (n == expression) return "expression";
    if (n == element_index_with_expressions) return "element_index_with_expressions";
    if (n == sign) return "sign";
    if (n == optional) return "optional";
    if (n == actual_para_list) return "actual_para_list";
    if (n == K) return "K";
    if (n == N_12) return "N_12";
    if (n == N_11) return "N_11";
    if (n == idList) return "idList";
    if (n == N3) return "N3";
    if (n == arithmeticOrBooleanExpr) return "arithmeticOrBooleanExpr";
    if (n == U) return "U";
    if (n == unary_op) return "unary_op";
    if (n == new_NT) return "new_NT";
    if (n == arithmeticExpr) return "arithmeticExpr";
    if (n == var_id_num) return "var_id_num";
    if (n == AnyTerm) return "AnyTerm";
    if (n == N7) return "N7";
    if (n == logicalOp) return "logicalOp";
    if (n == N8) return "N8";
    if (n == relationalOp) return "relationalOp";
    if (n == term) return "term";
    if (n == N4) return "N4";
    if (n == op1) return "op1";
    if (n == factor) return "factor";
    if (n == N5) return "N5";
    if (n == op2) return "op2";
    if (n == N_10) return "N_10";
    if (n == arrExpr) return "arrExpr";
    if (n == arrTerm) return "arrTerm";
    if (n == arr_N4) return "arr_N4";
    if (n == arr_N5) return "arr_N5";
    if (n == arrFactor) return "arrFactor";
    if (n == caseStmts) return "caseStmts";
    if (n == _default) return "_default";
    if (n == value) return "value";
    if (n == N9) return "N9";
    if (n == range_for_loop) return "range_for_loop";
    if (n == index_for_loop) return "index_for_loop";
    if (n == sign_for_loop) return "sign_for_loop";
    if (n == new_index_for_loop) return "new_index_for_loop";
    return NULL;
}

ListNode* insertlast (ListNode* head, grammarchar gc) {
    ListNode* newnode = (ListNode *)malloc(sizeof(ListNode));
    newnode -> val = gc;
    newnode -> next = NULL;
    if (head == NULL)
        head = newnode;
    else {
        ListNode* curr = head;
        while (curr -> next != NULL)
            curr = curr -> next;
        curr -> next = newnode;
    }
    return head;
}

void deletelast (ListNode* head) {
    if (head != NULL) {
        if (head -> next == NULL)
            head = NULL;
        ListNode* curr = head;
        while (curr -> next -> next != NULL)
            curr = curr -> next;
        curr -> next = NULL;
    }
}

StackNode* push (StackNode* top, ParseNode* newnode) {
    //printf("hi");
    StackNode* newstacknode = (StackNode*)malloc(sizeof(StackNode));
    newstacknode -> val = newnode;
    newstacknode -> next = top;
    top = newstacknode;
    //printf("%d",newstacknode->val->val.nt);
    return top;
}

int isEmpty (StackNode* top) {
    return top == NULL;
}

ParseNode* top (StackNode* top) {
    if (isEmpty(top))
        return NULL;
    return top->val;
}

StackNode* pop (StackNode* top) {
    if (isEmpty(top))
        return NULL;
    StackNode* curr = top;
    top = top -> next;
    return top;
}

void loadgrammar(char* filename) { //reads through the grammar file and converts it into an array of linkedlists that our code can use
    for(int i=0;i<NO_RULES;++i) grammar[i] = NULL;
    for(int i=0;i<NO_NONTERMS;++i) follow[i] = NULL;
    for(int i=0;i<NO_NONTERMS;++i) first[i] = NULL;
    FILE* fp = fopen(filename,"r");
    if (!fp) {
        printf("Grammar file cannot be opnened\n");
        exit(1);
    }
    char ch;
    int buff_index = 0, input_index = 0, rule = 0;
    char currinput[MAXTERMLEN];
    char temp[MAXTERMLEN];
    char grammar_buff[2 * MAXTERMLEN];
    for (int i = 0; i < 2 * MAXTERMLEN; i ++) grammar_buff[i] = '\0';
    while (!feof(fp)) { //read through entire file
        fread(grammar_buff, 1, 2 * MAXTERMLEN - 1, fp);
        buff_index = 0;
        while (buff_index < 2 * MAXTERMLEN - 1) {
            ch = grammar_buff[buff_index];
            if (ch == ' ' || ch == '\n') {
                grammarchar gc ;
                if (currinput[0] == '<') { //recognises non-terminals with the <> around the word
                    gc.t = NONTERMINAL;
                    for (int i = 0; i < MAXTERMLEN; i ++) temp[i] = '\0';
                    strncpy(temp, currinput + 1, input_index - 2);
                    gc.g.nt = mapnttoenum(temp);
                }
                else {
                    gc.t = TERMINAL;
                    gc.g.t = mapttoenum(currinput);
                }
                grammar[rule] = insertlast(grammar[rule],gc); //adds to array
                if (ch == '\n')
                    rule++;
                for (int i = 0; i < MAXTERMLEN; i ++)
                    currinput[i] = '\0';
                input_index = 0;
            }
            else {
                currinput[input_index] = ch;
                input_index ++;
            }
            buff_index ++;
        }
    }
    fclose(fp);
}

int derivesepsilon(grammarchar gc){ //checks whether the particular grammar character derives an epsilon - helpful for deriving the first and follow sets
    if (gc.t == TERMINAL)
        return 0;
    if (first[gc.g.nt] == NULL)
        return 0;
    ListNode* curr = first[gc.g.nt];
    while (curr -> next != NULL) //goes through the first set and checks iif it contains epsilon
        curr = curr -> next;
    if (curr -> val.g.t == EPS)
        return 1;
    return 0;
}

void computeFirst (non_terminal A)
{
    if (first[A]!=NULL) return; //using dp
    for(int i=0; i<NO_RULES; i++){ //for all the rules in the grammar
        if (grammar[i] -> val.g.nt == A){//if lhs of rule is A
            ListNode* rhs_current = grammar[i] -> next;
            if (rhs_current -> val.t == TERMINAL){//if rhs starts with terminal, add that to first set
                first[A] = insertlast(first[A], rhs_current -> val);
            }

            else if (rhs_current -> val.t == NONTERMINAL){ //else add the first set of the first non-terminal
                computeFirst(rhs_current -> val.g.nt);
                ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                while (first_rhs_current != NULL) {
                    if (first_rhs_current -> val.g.t != EPS)
                        first[A] = insertlast(first[A], first_rhs_current -> val);
                    first_rhs_current = first_rhs_current -> next;
                }
                while (derivesepsilon(rhs_current -> val)){ //if it derives epsilon, we go to the next character
                    rhs_current = rhs_current -> next;
                    if (rhs_current == NULL){
                        grammarchar add_eps;
                        add_eps.t = TERMINAL;
                        add_eps.g.t = EPS;
                        first[A] = insertlast(first[A], add_eps);
                    }
                    else if (rhs_current -> val.t == TERMINAL){ //if its a terminal
                        first[A] = insertlast(first[A], rhs_current -> val);
                    }
                    else{
                        computeFirst(rhs_current -> val.g.nt); //if not a terminal, do computefirst of the non-terminal, and add that and continue
                        ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                        while (first_rhs_current != NULL) {
                            if (first_rhs_current -> val.g.t != EPS)
                                first[A] = insertlast(first[A], first_rhs_current -> val);
                            first_rhs_current = first_rhs_current -> next;
                        }
                    }
                }
            }
        }
    }
}

int contains(ListNode* ln, tokentype term) { //check if the terminal is contained in the list
    if (ln == NULL)
        return 0;
    for (ListNode* curr = ln; curr != NULL; curr = curr -> next) {
        if (curr -> val.t == TERMINAL && curr -> val.g.t == term)
            return 1;
    }
    return 0;
}

void computeFollow(non_terminal A) {
    if (follow[A]!=NULL) return; //using DP
    for(int i=0; i<NO_RULES; i++){
        ListNode* lhs_current = grammar[i];
        ListNode* rhs_A = grammar[i] -> next;
        ListNode* rhs_current;

        while (rhs_A != NULL) {
            //checking if A is in RHS
            if (rhs_A -> val.t == NONTERMINAL && rhs_A -> val.g.nt == A) {
                
                rhs_current = rhs_A -> next;

                //if A is the rightmost element add follow of LHS to follow of A
                if (rhs_current == NULL) {
                    //if A is also the LHS, do not do anything
                    if (lhs_current -> val.g.nt != A) {
                        computeFollow(lhs_current -> val.g.nt);
                        ListNode* lhsfollow = follow[lhs_current -> val.g.nt];
                        while (lhsfollow != NULL) {
                            if (!contains(follow[A], lhsfollow -> val.g.t))
                                follow[A] = insertlast(follow[A], lhsfollow -> val);
                            lhsfollow = lhsfollow -> next;
                        }
                    }
                }

                //if there is a terminal after A, add that terminal to the follow set of A
                else if (rhs_current -> val.t == TERMINAL){
                    if (!contains(follow[A], rhs_current -> val.g.t))
                        follow[A] = insertlast(follow[A], rhs_current -> val);
                }

                //if there is a non terminal
                else{
                    ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                    while (first_rhs_current != NULL) {
                        if (first_rhs_current -> val.g.t != EPS && !contains(follow[A], first_rhs_current -> val.g.t))
                            follow[A] = insertlast(follow[A], first_rhs_current -> val);
                        first_rhs_current = first_rhs_current -> next;
                    }

                    while(derivesepsilon(rhs_current -> val)){ //if the non-terminal derives epsilon, we continue to the next non-terminal
                        rhs_current = rhs_current -> next;
                        if (rhs_current == NULL){
                            if (lhs_current -> val.g.nt != A) {
                                computeFollow(lhs_current -> val.g.nt);
                                ListNode* lhsfollow = follow[lhs_current -> val.g.nt];
                                while (lhsfollow != NULL) {
                                    if (!contains(follow[A], lhsfollow -> val.g.t))
                                        follow[A] = insertlast(follow[A], lhsfollow -> val);
                                    lhsfollow = lhsfollow -> next;
                                }
                            }
                            break;
                        }
                        else if (rhs_current -> val.t == TERMINAL && !contains(follow[A], rhs_current -> val.g.t)){
                            follow[A] = insertlast(follow[A], rhs_current -> val);
                        }
                        else{
                            computeFirst(rhs_current -> val.g.nt);
                            ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                            while (first_rhs_current != NULL) {
                                if (!contains(follow[A], first_rhs_current -> val.g.t))
                                    follow[A] = insertlast(follow[A], first_rhs_current -> val);
                                first_rhs_current = first_rhs_current -> next;
                            }
                        }
                    }
                }
            }
            rhs_A = rhs_A -> next;
        }
    }
}

void computefirstandfollow (){
    for(int i=0; i<NO_NONTERMS; i++)
        computeFirst(i); //compute all first sets first
    grammarchar gc;
    gc.t = TERMINAL;
    gc.g.t = TK_EOF;
    follow[program] = insertlast(follow[program], gc);
    for(int i=0; i<NO_NONTERMS; i++)
        computeFollow(i); //then compute all follow sets
}

void createParseTable(){ //creating the parse table
    int s = (NO_TERMS-1)*(NO_NONTERMS);
    memset(parseTable,-1,s*sizeof(int));

    for(int i=0;i<NO_RULES;i++){ //checking all the rules
        struct ListNode* A = grammar[i];
        struct ListNode* B = grammar[i];
        int a = A->val.g.nt; //A is RHS of rule
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t==EPS)){ //if the rule derives epsilon, add that rule into the table for all the terminals in A's follow set 
                struct ListNode* C = follow[a];
                while(C!=NULL){
                    int c = C->val.g.t; 
                    parseTable[a][c] = i;
                    C = C->next;
                }
                break; //if it derives epsilon, then we move on to the next rule
            }

            if(B->val.t==TERMINAL){ //if LHS of the rule gives a terminal, add that rule into thee table under the terminal's column
                b = B->val.g.t; 
                parseTable[a][b] = i;
            }
            else{
                b = B->val.g.nt;
                struct ListNode* C = first[b]; //otherwise add that rule into the columns of all the terminals in A's first set
                while(C!=NULL){
                    int c = C->val.g.t; 
                    if(C->val.g.t!=EPS)
                        parseTable[a][c] = i;
                    C = C->next;
                }
            } //if the first non-terminal on the RHS derives epsilon, then go on to the next non-terminal
        } while(derivesepsilon(B->val)); //derivesEpsilon returns 0 on input -1
    }
}

void printStack(StackNode* S) {
    for (StackNode* curr = S; curr != NULL; curr = curr -> next) {
        if (curr->val->t == TERMINAL)
            printf("TERMINAL: %s\t",mapttokentostring(curr->val->val.t->type));
        else
            printf("NONTERMINAL: %s\t",mapnttostring(curr->val->val.nt));
    }
    printf("\n \n");
}

void createSynchronizingSet(){
    ListNode* follow_set = NULL;
    for (int i = 0; i < NO_NONTERMS; i ++) //pushes all elements of the follow set into the respective synchronizing set
        for (follow_set = follow[i]; follow_set != NULL; follow_set = follow_set -> next)
            synchronizingSet[i] = insertlast(synchronizingSet[i], follow_set -> val);

    
}

StackNode* pushrule(int rule, StackNode* S){
	StackNode* aux = NULL; //creates auxilliary stack
	ListNode* x = grammar[rule]->next; //takes RHS of the grammar rule
    ParseNode* prev = (ParseNode*)malloc(sizeof(ParseNode));

    while(x!=NULL){
        ParseNode* temp = (ParseNode*)malloc(sizeof(ParseNode));
        if(x==grammar[rule]->next){//if this is the first node of the RHS of the rule, then we point the child pointer of the parent(i.e. top of the main stack) to it

            top(S)->child = temp;
        }
        temp->t = x->val.t; //initialise the TreeNode with information of the grammar character depending on whether it is a terminal or non-terminal
        if(x->val.t==TERMINAL){
            temp->val.t = (struct Token*)malloc(sizeof(struct Token));
            temp->val.t->type = x->val.g.t;
        }
        else
            temp->val.nt = x->val.g.nt;
        temp->ruleno = rule;
        temp->child = NULL;
        temp->sibling = NULL;
        prev->sibling = temp; //point the previous node's sibling pointer to the current TreeNode
        prev = temp;
        aux = push(aux,temp); //push to auxilliary stack
        x = x->next;
    }
    S = pop(S); //remove the top of the main stack
    while(isEmpty(aux) == 0){ //add the LHS of the rule to the main stack. We add it from the auxilliary stack since we need it to be pushed from right to left
        S = push(S,top(aux));
        aux = pop(aux);
    }
    return S;
}

ParseNode* parse(){
    createSynchronizingSet(); //first create the synchronizing set for all non-terminals
    StackNode* S = NULL; //create stack
    ParseNode* Eof = (ParseNode*)malloc(sizeof(ParseNode)); //create EOF ParseNode
    Eof->val.t = (struct Token*)malloc(sizeof(struct Token));
    Eof->val.t->type = TK_EOF;
    Eof->t = TERMINAL;
    Eof->sibling = NULL;
    Eof->child = NULL;
    ParseNode* Root = (ParseNode*)malloc(sizeof(ParseNode)); //create Root ParseNode
    Root->val.nt = program; //our start symbol is program
    Root->t = NONTERMINAL;
    Root->ruleno = 0;
    Root->sibling = NULL;
    ParseNode* X = Root;
    S = push(S,Eof); //push into stack, EOF at bottom then program
    S = push(S,X);
    struct Token* L = getNextToken(); //start at the first token
    while(L!=NULL){
        if(isEmpty(S)){ //if stack is empty and we still have a token, it means there is an error
            printf("\nSyntax Error at line no %d ... empty stack\n",L->lineNo); 
            break;
        }
        //printf("hi");
        X = top(S); //get top of stack
        if(X->t==TERMINAL){ //if top of stack is terminal,
            if(X->val.t->type == L->type){
                X->t = TERMINAL; //we convert the treenode to leafnode,
                X->val.t = L;
                S = pop(S); //pop the terminal at the top of the stack
                if (L -> type == TK_EOF) { //if we reach the end of the file, then break
                    break;
                }
                L = getNextToken(); //get next token
                if (L == NULL) { //if we are unable to get next token, then we convert it to EOF token
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
            else if(X->val.t->type==TK_SEMICOLON && L->type==TK_END){
                printf("\nERROR : Expected semicolon at line no %d \n",L->lineNo);
                S = pop(S);
            }
            else{ //if we the top of the stack is a different terminal to the token, then we have an error
                printf("\nSyntax Error at line no %d ... terminal mismatch\n",L->lineNo);
                printf("actual : %s\n",mapttokentostring(L->type));
                printf("exp : %s\n",mapttokentostring(X->val.t->type));
                S = pop(S); //we pop the stack and also get the next token
                if (L -> type == TK_EOF) { //if we reach the end of the file, then break
                    break;
                }
                L = getNextToken(); //get next token
                if (L == NULL) { //if we are unable to get next token, then we convert it to EOF token
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
        }
        else if(X->t==NONTERMINAL){ //else if top of stack is a non-terminal
            //printf("hi");
            if(parseTable[X->val.nt][L->type]>=0){ //check the parse table for the right rule
                X->ruleno = parseTable[X->val.nt][L->type];
                //X->child = grammar[parseTable[X->val.nt][L->type]]->next;
                if(grammar[parseTable[X->val.nt][L->type]]->next->val.t==TERMINAL && grammar[parseTable[X->val.nt][L->type]]->next->val.g.t==EPS)
                    S = pop(S); //if the rule derives epsilon, just pop S
                else 
                    S = pushrule(X->ruleno, S); //otherwise push the LHS of the rule (the pushrule function pops S on its own at the end)
                // printStack(S);
            }

            else if(contains(synchronizingSet[X->val.nt],L->type)){ //otherwise if we can't find the right rule, check if the non-terminal contains the top of the stack in the synchronizing set. if yes, we can pop the stack and continue
                printf("\nSyntax Error at line no %d ... non-terminal mismatch, popping stack\n",L->lineNo);
                S = pop(S);
            }

            else { //otherwise get next token
                printf("\nSyntax Error at line no %d ... non-terminal mismatch, getting new token\n",L->lineNo);
                if (L -> type == TK_EOF) { //if L is TK_EOF, we can't get next token, so we just break
                    break;
                }
                L = getNextToken();
                if (L == NULL) {
                    L = malloc(sizeof(struct Token));
                    L -> type = TK_EOF;
                }
            }
        }
        // printf("%d %s: ",L->lineNo,mapttokentostring(L->type));
        //printStack(S);
    }
    if(!isEmpty(S)){ //if stack is not empty after consuming all tokens, we have an error
        printf("\nERROR: stack not empty yet\n");
    }
    return Root;
}

void printTree(ParseNode* root, FILE* fp) {
    if(root==NULL) return;
    if(!fp){
        printf("\nERROR: file pointer invalid\n\n");
        exit(1);
    }
    ParseNode* curr = root->child;
    printTree(root->child,fp);
    if(root->t==TERMINAL && root->val.t->type==TK_NUM) fprintf(fp,"Terminal %s with token value %d\n", mapttokentostring(root->val.t->type),root->val.t->val.integer);
    else if(root->t==TERMINAL && root->val.t->type==TK_RNUM) {fprintf(fp,"Terminal %s with token value %lf\n", mapttokentostring(root->val.t->type),root->val.t->val.decimal);}
    else if(root->t==TERMINAL) {fprintf(fp,"Terminal %s with token value %s\n", mapttokentostring(root->val.t->type),root->val.t->val.identifier);}
    else fprintf(fp,"Non-Terminal %s\n",mapnttostring(root->val.nt));
    if (curr != NULL)
        curr=curr->sibling;
    for(;curr!=NULL; curr=curr->sibling){
        printTree(curr,fp);
    }
}

typedef enum {PROGRAM,UNARY_PLUS, UNARY_MINUS, ID, NUM, RNUM, ARRAY,ARR_INDEX1, ARR_INDEX2, PLUS, MINUS, MUL, DIV, AND, OR, LT, LE, GT, GE, EQ, NE, MODULEDECLARATION, DRIVERMODULE,MODULE_REUSE, MODULE, RET, PARAMETER, INTEGER_, REAL_, BOOLEAN_, RANGE_WHILE,RANGE_FOR, STATEMENTS, INPUT, OUTPUT, ARR_OUTPUT, TRUE, FALSE, ASSIGN, ARR_ASSIGN, INDEX_ARR, DECLARE, ID_LIST, CASE,CASE_STMT,RANGE, INPUT_PLIST, OUTPUT_PLIST} Label;

typedef enum {INTEGER, REAL, BOOLEAN} Prim_type;

typedef struct Array_tuple{
    Prim_type pt; 
    int lower_bound;
    int upper_bound;
}Array_tuple;

typedef union Type{
    Prim_type pt;
    Array_tuple at;
}Type;

typedef struct DataType{
    bool is_primitive;
    Type t;
}DataType;

typedef struct ASTNode{
    Label label;
    DataType type;
    struct Token* tk;
    struct ASTNode* child;
    struct ASTNode* sibling;
}ASTNode;

ASTNode* astroot;

void printAST(ASTNode* root){
    if(root==NULL) return;
    if(root->label == ID)
        printf("%s\n",root->tk->val.identifier);
    printf("%d\n",root->label);
    ASTNode* temp = root->child;
    while(temp!=NULL){
        printAST(temp);
        temp=temp->sibling;
    }
}

ASTNode* makeNode(Label label_1,struct Token* token_1,struct ASTNode* child_1,struct ASTNode* sibling_1){
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    newNode->label = label_1;
    newNode->tk = token_1;
    newNode->child = child_1;
    newNode->sibling = sibling_1;
    return newNode;
}

//rule no for leafnodes is same as that of their parents, change in parser;
void makeAST(struct ParseNode* parserNode){
    ASTNode *newNode = NULL, *newNode1 = NULL;
    ParseNode *c1=NULL,*c2=NULL,*c3=NULL;
    printf("%d ",parserNode->ruleno + 1);
    printf("%s\n",mapnttostring(parserNode->val.nt));
    switch(parserNode->ruleno) {

        case 0:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            astroot = makeNode(PROGRAM,NULL,parserNode->child->addr,NULL);
            if (astroot->child != NULL)
                astroot->child->sibling = parserNode->child->sibling->addr;
            else
                astroot->child = parserNode->child->sibling->addr;
            if (astroot->child == NULL)
                astroot->child = parserNode->child->sibling->sibling->addr;
            else if (astroot->child->sibling == NULL)
                astroot->child->sibling = parserNode->child->sibling->sibling->addr;
            else
                astroot->child->sibling->sibling = parserNode->child->sibling->sibling->addr;
            parserNode->child->sibling->sibling->addr->sibling = parserNode->child->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            free(parserNode);
            break;

        case 1:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 2:
            parserNode->addr = NULL;
            break;
        
        case 3:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = MODULEDECLARATION;
            parserNode->addr->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 4:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 5:
            parserNode->addr = NULL;
            break;

        case 6:
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = DRIVERMODULE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 7:
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = MODULE;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = ID;
            parserNode->addr->child->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child->child = NULL;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->child->sibling->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            parserNode->addr->child->sibling->sibling->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 8:
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = RET;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 9:
            parserNode->addr = NULL;
            break;
        
        case 10:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INPUT_PLIST;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = PARAMETER;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 11:
            makeAST(parserNode->child->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 12:
            parserNode->addr = NULL;
            break;

        case 13:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INPUT_PLIST;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->child->label = PARAMETER;
            parserNode->addr->child->tk = parserNode->child->val.t;
            parserNode->addr->child->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 14:
            makeAST(parserNode->child->sibling->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = PARAMETER;
            parserNode->addr->tk = parserNode->child->sibling->val.t;
            parserNode->addr->child = parserNode->child->sibling->sibling->sibling->addr;
            parserNode->addr->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 15:
            parserNode->addr = NULL;
            break;

        case 16:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INTEGER_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 17:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 18:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 19:
            makeAST(parserNode->child->sibling->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ARRAY;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 20:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ARRAY;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->addr;
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling); 
            free(parserNode->child);
            break;

        case 21:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INTEGER_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 22:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = REAL_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 23:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = BOOLEAN_;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 24:
            makeAST(parserNode->child->sibling);
            parserNode->addr =  makeNode(STATEMENTS,NULL,parserNode->child->sibling->addr,NULL);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 25:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->addr->sibling = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 26:
            parserNode->addr = NULL;
            break;

        case 27:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 28:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 29:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 30:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 31:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 32:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INPUT;
            parserNode->addr->tk = parserNode->child->sibling->sibling->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 33:
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = OUTPUT;
            parserNode->addr->tk = NULL;
            parserNode->addr->sibling = NULL;
            parserNode->addr->child = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 34:
            makeAST(parserNode->child->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->sibling = NULL;
            if (parserNode->child->sibling->addr == NULL) {
                parserNode->addr->label = ID;
                parserNode->addr->tk = parserNode->child->val.t;
                parserNode->addr->child = NULL;
            }
            else {
                parserNode->addr->label = ARR_OUTPUT;
                parserNode->addr->tk = NULL;
                parserNode->addr->child = (ASTNode*)malloc(sizeof(ASTNode));
                parserNode->addr->child->label = ID;
                parserNode->addr->child->tk = parserNode->child->val.t;
                parserNode->addr->child->child = NULL;
                parserNode->addr->child->sibling = parserNode->child->sibling->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 35:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = NUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 36:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = RNUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 37:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 38:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = TRUE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 39:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = FALSE;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 40:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
            
        case 41:
            parserNode->addr = NULL;
            break;

        case 42:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 43:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 44:
            parserNode->child->sibling->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->child->sibling->addr->label = ID;
            parserNode->child->sibling->addr->tk = parserNode->child->val.t;
            parserNode->child->sibling->addr->child = NULL;
            parserNode->child->sibling->addr->sibling = NULL;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 45:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 46:
            parserNode->child->addr = parserNode->addr;
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 47:
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ASSIGN;
            newNode->tk = NULL;
            newNode->child = parserNode->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 48:
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_ASSIGN;
            newNode->tk = NULL;
            newNode->child = parserNode->addr;
            newNode->sibling = parserNode->child->sibling->sibling->sibling->sibling->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ASSIGN;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = newNode;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 49:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = INDEX_ARR;
            parserNode->addr->tk = NULL;
            if (parserNode->child->addr != NULL) {
                parserNode->addr->child = parserNode->child->addr;
                parserNode->addr->child->child = parserNode->child->sibling->addr;
            }
            else
                parserNode->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 50:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = NUM;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 51:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = ID;
            parserNode->addr->tk = parserNode->child->val.t;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child);
            break;

        case 52:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = UNARY_PLUS;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 53:
            parserNode->addr = (ASTNode*)malloc(sizeof(ASTNode));
            parserNode->addr->label = UNARY_MINUS;
            parserNode->addr->tk = NULL;
            parserNode->addr->child = NULL;
            parserNode->addr->sibling = NULL;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 54:
            parserNode->addr = NULL;
            break;

        
        // ANSH
        case 55:
            c1= parserNode->child;
            c2= c1->sibling->sibling->sibling->sibling->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(MODULE_REUSE,NULL,c1->addr,NULL);
            printf("line:= %d\n",parserNode->child->sibling->val.t->lineNo);
            if(c1->addr!=NULL) newNode->child->sibling = c2->addr;
            else newNode->child = c2->addr;
            parserNode->addr = newNode;
            free(c2->sibling->val.t); 
            free(c2->sibling); 
            free(c2);
            free(c1->sibling->sibling->sibling->sibling->sibling->val.t);
            free(c1->sibling->sibling->sibling->sibling->sibling);
            free(c1->sibling->sibling->sibling->sibling->val.t);
            free(c1->sibling->sibling->sibling->sibling);
            free(c1->sibling->sibling->sibling->val.t);
            free(c1->sibling->sibling->sibling);
            free(c1->sibling->sibling->val.t);
            free(c1->sibling->sibling);
            free(c1->sibling->val.t);
            free(c1->sibling);
            free(c1);
            break;
        
        case 56:
            c1= parserNode->child;
            c2= c1->sibling;
            c3= c2->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            newNode = makeNode(ID_LIST,NULL,c1->addr,NULL);
            if(c1->addr!=NULL){
                newNode->child->sibling = c2->addr;
                newNode->child->sibling->sibling = c3->addr;
            }else{
                newNode->child = c2->addr;
                newNode->child->sibling = c3->addr;
            }
            free(c3); 
            free(c2);
            free(c1);
            break;
        
        case 57:
            c1= parserNode->child->sibling;
            c2= c1->sibling;
            c3= c2->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            if(c1->addr==NULL){
                parserNode->addr = c2->addr;
                c2->addr->sibling = c3->addr;
            }else{
                parserNode->addr = c1->addr;
                c1->addr->sibling = c2->addr;
                c2->addr->sibling = c3->addr;
            }
            free(c3);
            free(c2); 
            free(c1);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 58:
            parserNode->addr = NULL;
            break;
        
        case 59:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 60:
            parserNode->addr = makeNode(RNUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 61:
            c1= parserNode->child;
            makeAST(c1); 
            newNode = makeNode(BOOLEAN_,NULL,c1->addr,NULL);
            free(c1);
            break;

        case 62:
            c1= parserNode->child->sibling;
            makeAST(c1); 
            newNode = makeNode(ID,parserNode->child->val.t,NULL,NULL);
            if(c1->addr==NULL){
                parserNode->addr = newNode;
            }else{
                parserNode->addr = makeNode(ARRAY,NULL,newNode,NULL);
                newNode->sibling = c1->addr;
            }
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 63:
            c1= parserNode->child->sibling;
            makeAST(c1); 
            parserNode->addr = makeNode(ASSIGN,NULL,c1->addr,NULL);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(c1);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 64:
            parserNode->addr = NULL;

        case 65:
            c1= parserNode->child;
            c2= c1->sibling;
            makeAST(c2); 
            newNode1 = makeNode(ID,c1->val.t,NULL,c2->addr);
            parserNode->addr = makeNode(ID_LIST,NULL,newNode1,NULL);
            free(c2);
            free(c1);
            break;
        
        case 66:
            c1 = parserNode->child->sibling;
            c2 = c1->sibling;
            makeAST(c2); 
            parserNode->addr = makeNode(ID,c1->val.t,NULL,c2->addr);
            free(c2);
            free(c1);
            free(parserNode->child);
            break;

        case 67:
            parserNode->addr = NULL;
            break;

        //TOSHIT
        case 68:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 69:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;
        
        case 70:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->addr;
            parserNode->child->addr->child = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 71:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 72:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 73:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 74:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = UNARY_MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 75:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 76:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 77:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 78:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 79:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 80:
            break;

        case 81:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 82:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 83:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 84:
            break;

        case 85:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 86:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 87:
            break;

        case 88:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 89:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 90:
            break;

        case 91:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 92:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 93:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = RNUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 94:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 95:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            makeAST(parserNode->child->sibling);
            newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
            newNode1->label = ARRAY;
            newNode1->child = newNode;
            newNode1->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode1;
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 96:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child); 
            break;

        case 97:
            parserNode->addr = NULL; //no index case
            break;

        case 98:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX1;
            newNode->child = parserNode->child->addr;
            newNode->child->sibling = parserNode->child->sibling->addr;
            parserNode->addr = newNode;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 99:
            makeAST(parserNode->child);
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ARR_INDEX2;
            newNode->child = parserNode->child->addr;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child);
            break;

        case 100:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 101:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 102:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 103:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 104:
            break;

        case 105:
            makeAST(parserNode->child);
            parserNode->child->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 106:
            makeAST(parserNode->child);
            makeAST(parserNode->child->sibling);
            parserNode->child->addr->child = parserNode->addr;
            parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
            parserNode->child->sibling->sibling->addr = parserNode->child->addr;
            makeAST(parserNode->child->sibling->sibling);
            parserNode->addr = parserNode->child->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child);
            break;

        case 107:
            break;

        case 108:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = ID;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 109:
            makeAST(parserNode->child->sibling);
            parserNode->addr = parserNode->child->sibling->addr;
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 110:
            makeAST(parserNode->child);
            parserNode->addr = parserNode->child->addr;
            free(parserNode->child);
            break;

        case 111:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NUM;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 112:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = PLUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 113:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MINUS;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 114:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = MUL;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 115:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = DIV;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 116:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = AND;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 117:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = OR;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 118:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 119:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = LE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 120:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GT;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 121:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = GE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 122:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = EQ;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 123:
            newNode = (ASTNode*)malloc(sizeof(ASTNode));
            newNode->label = NE;
            newNode->tk = parserNode->child->val.t;
            newNode->child = NULL;
            newNode->sibling = NULL;
            parserNode->addr = newNode;
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;     

        // ANSH
        case 124:
            makeAST(parserNode->child->sibling);
            makeAST(parserNode->child->sibling->sibling->sibling);
            parserNode->addr = makeNode(DECLARE,NULL,parserNode->child->sibling->addr,NULL);
            parserNode->addr->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 125:
            c1= parserNode->child->sibling->sibling->sibling->sibling->sibling;
            c2= c1->sibling;
            makeAST(c1);
            makeAST(c2);
            newNode = makeNode(CASE_STMT,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            free(c2->sibling->val.t);
            free(c2->sibling);
            free(c2);
            free(c1);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling->val.t);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 126:
            c1= parserNode->child->sibling;
            c2= c1->sibling->sibling;
            c3= c1->sibling->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            newNode = makeNode(CASE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            newNode->child->sibling->sibling = c3->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 127:
            c1= parserNode->child->sibling;
            c2= c1->sibling->sibling;
            c3= c1->sibling->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            makeAST(c3);
            newNode = makeNode(CASE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            if(c3->addr!=NULL) newNode->child->sibling->sibling = c3->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->sibling);
            free(parserNode->child->sibling->sibling->val.t);
            free(parserNode->child->sibling->sibling);
            free(parserNode->child->sibling);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 128:
            parserNode->addr = NULL;
            break;

        case 129:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 130:
            parserNode->addr = makeNode(TRUE,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        case 131:
            parserNode->addr = makeNode(FALSE,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;
        
        case 132:
            c1 = parserNode->child->sibling->sibling;
            makeAST(c1); 
            newNode = makeNode(RANGE,NULL,c1->addr,NULL);
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;

        case 133:
            parserNode->addr = NULL;
            break;

        case 134:
            c1= parserNode->child->sibling->sibling->sibling->sibling;
            c2= c1->sibling->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(RANGE_FOR,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;

        case 135:
            c1= parserNode->child->sibling->sibling;
            c2= c1->sibling->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(RANGE_WHILE,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling->sibling->val.t); 
            free(parserNode->child->sibling->sibling->sibling); 
            free(parserNode->child->sibling->sibling); 
            free(parserNode->child->sibling->val.t); 
            free(parserNode->child->sibling); 
            free(parserNode->child->val.t); 
            free(parserNode->child); 
            break;
        
        case 136:
            c1= parserNode->child;
            c2= c1->sibling->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(RANGE_FOR,NULL,c1->addr,NULL);
            newNode->child->sibling = c2->addr;
            free(c2); 
            free(c1->sibling->val.t);
            free(c1->sibling);
            free(c1);
            break;
        
        case 137:
            c1= parserNode->child;
            c2= c1->sibling;
            makeAST(c1); 
            makeAST(c2);
            newNode = makeNode(RANGE_FOR,NULL,c1->addr,NULL);
            if(newNode->child!=NULL) newNode->child->sibling = c2->addr;
            else newNode->child = c2->addr;
            free(c2); 
            free(c1);
            break;

        case 138:
            parserNode->addr = makeNode(NUM,parserNode->child->val.t,NULL,NULL);
            free(parserNode->child->val.t);
            free(parserNode->child);
            break;

        default:
            printf("ERROR: No such rule exist\n");
    }
}

ASTNode* AST(){
    ptr = fopen("testcase4.txt","r");
    ptr = initLexer(ptr, 32);
    loadgrammar("grammar.txt");
    computefirstandfollow();
    createParseTable();
	ParseNode* parserNode = parse();
    // FILE* fp = fopen("checktree.txt","w");
    // printTree(parserNode,fp);
    // fflush(fp); fclose(fp);
	makeAST(parserNode);
    printf("hi\n");
    printAST(astroot);
	return astroot;
}

int main(){
    AST();
    return 0;
}