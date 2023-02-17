#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*>>>>>>>>>>>> INIITIALIZING STRUCTS AND GLOBAL VARS <<<<<<<<<<<<<<<<<*/

char buffer1[32], buffer2[32];
FILE *ptr;

union TokenVal
{
    int integer;
    float decimal;
    char *identifier;
};

struct Token
{
    char *type;
    int lineNo;
    union TokenVal val;
};

/*>>>>>>>>>>>>>>>>>>>>> UTILITY FUNCTIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<*/

int isNum(char ch)
{
    return (ch >= '0' && ch <= '9');
}

int isChar(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

int readBuffer()
{
    if (!ptr)
    {
        ptr = fopen("./Program.txt", "r");
        if (!ptr)
        {
            fprintf(stderr, "File cannot be opened\n");
            exit(1);
        }
    }
    if (!feof(ptr))
        fread(buffer2, sizeof(buffer2) - 1, 1, ptr);
    else
        return -1;
    return 0;
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>> HASH TABLE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// collisions resolved using linear probing
char *hash[50];

int sumChars(char *s)
{
    int res = 0;
    for (int i = 0; s[i] != '\0'; ++i)
    {
        res = (res + s[i] - 'a');
    }
    return res;
}

void insert_hash(char *str)
{
    int ind = sumChars(str);
    while (hash[ind] != NULL)
        ind++;
    hash[ind] = str;
}

int search_hash(char *str)
{
    int ind = sumChars(str);
    while (hash[ind] != NULL)
    {
        printf("%s == %s\n", str, hash[ind]);
        if (strcmp(str, hash[ind]) == 0)
            return 1;
        ind++;
    }
    return 0;
}

void init_hash()
{
    char *keywords[] = {"integer", "real", "boolean", "of", "array", "start", "end", "declare", "module", "driver", "program", "get_value", "print", "use", "with", "parameters", "takes", "input", "returns", "for", "in", "switch", "case", "break", "default", "while"};
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i)
        insert_hash(keywords[i]);
}

/*>>>>>>>>>>>>>>>>>>>>>> LEXER FUNCTIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

void Tokenize(int begin, int forward, char *tokenType, int lineNo)
{
    char s[64];
    int size = 0;
    struct Token tk;
    // assuming id size < 32
    if (begin < forward)
    {
        strncpy(s, buffer1 + begin, forward - begin + 1);
        size = forward - begin;
    }
    else
    {
        strncpy(s, buffer1 + begin, 32 - begin);
        for (int i = 0; i < forward; i++)
        {
            s[32 - begin + 1 + i] = buffer2[i];
        }
        s[32 - begin + forward + 1] = '\0';
        strcpy(buffer1, buffer2);
        readBuffer();
        size = 32 - begin + forward;
    }
    tk.type = tokenType;
    tk.lineNo = lineNo;
    // ID to keyword resolution
    if (tokenType == "TK_ID")
    {
        int is_keword = search_hash(s);
        tk.val.identifier = s;
        int i = 0;
        // capitalize s
        while (s[i] != '\0')
            s[i] += 'A' - 'a';
        tk.type = s;
    }
    else if (tokenType == "TK_NUM")
    {
        int r = 0, k = 1;
        for (int i = size; i >= 0; --i)
        {
            r += k * (s[i] - '0');
            k *= 10;
        }
        tk.val.integer = r;
    }
    else if (tokenType == "TK_RNUM")
    {
        // do something
    }
    else
    {
        tk.val.identifier = s;
    }
    // call parser with token tk
    // parser(tk);
}

void Lexer()
{
    int begin = 0, forward = 0, line = 0;
    char ch = buffer1[forward];
    int state = 0;
    while (state != -1)
    {
        switch (state)
        {
        // start state
        case 0:
            if (isChar(ch))
                state = 1;
            else if (ch == '+')
                state = 2;
            else if (ch == '-')
                state = 3;
            else if (isNum(ch))
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
            else if (ch == '\b' || ch == '\t')
                state = 39;
            forward++;
            break;

        // ID/keyword final state
        case 1:
            if (isChar(ch) || isNum(ch))
                forward++;
            // if whitespace encountered
            else
            {
                Tokenize(begin, forward, "TK_ID", line);
                begin = forward;
            }
            break;

        // tokenize TK_PLUS
        case 2:
            Tokenize(begin, forward, "TK_PLUS", line);
            forward++;
            begin = forward;
            state = 0;
            break;

        // tokenize TK_MINUS
        case 3:
            Tokenize(begin, forward, "TK_MINUS", line);
            forward++;
            begin = forward;
            state = 0;
            break;

        // ----------------------------------------Numbers------------------------------------------

        // TK_NUM final state
        case 4:
            if (isNum(ch))
                forward++;
            else if (ch == '.') 
            {
                forward++;
                state = 5;
            }
            else
            {
                Tokenize(begin, forward - 1, "TK_NUM", line);
                begin = forward;
            }
            break;

        // dot after number
        case 5:
            if (isNum(ch))
            {
                state = 7;
                forward++;
            }
            else if (ch == '.')
            {
                Tokenize(begin, forward - 2, "TK_NUM", line);
                forward -= 1;
                begin = forward;
                state = 0;
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
            if (isNum(ch))
                forward++;
            else if (ch == 'E' || ch == 'e')
            {
                forward++;
                state = 8;
            }
            else
            {
                Tokenize(begin, forward - 1, "TK_RNUM", line);
                begin = forward;
                state = 0;
            }
            break;
        // <num>.<num>E
        case 8:
            if (isNum(ch))
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
            if (isNum(ch))
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
            if (isNum(ch))
                forward++;
            else
            {
                Tokenize(begin, forward - 1, "TK_RNUM", line);
                begin = forward;
                state = 0;
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
            Tokenize(begin, forward, "TK_EQ", line);
            forward++;
            begin = forward;
            state = 0;
            break;
        //tokenise comments
        case 13:
            if(ch=='*'){
                forward++;
                state=14;
            }
            else{
                Tokenize(begin, forward - 1, "TK_MUL", line);
                begin = forward;
                state = 0;
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
                Tokenize(begin, forward - 1, "TK_LT", line);
                begin = forward;
                state = 0;
            }
            break;
        case 18:
            Tokenize(begin, forward - 1, "TK_LE", line);
            begin = forward;
            state = 0;
            break;
        
        case 19:
            if(ch=='<'){
                forward++;
                state=20;
            }
            else {
            Tokenize(begin, forward - 1, "TK_DEF", line);
            begin = forward;
            state = 0;
            }
            break;
            
        case 20:
            Tokenize(begin, forward - 1, "TK_DRIVERDEF", line);
            begin = forward;
            state = 0;
            break;
        //Shreekar ends
        // tokenize open sq bracket

        //Sriram Cases: 21 to 30
        //Tokenize Comma and Semicolon
        case 21:
            Tokenize(begin, forward, "TK_COMMA", line);
            forward ++;
            begin = forward;
            state = 0;
            break;

        case 22:
            Tokenize(begin, forward, "TK_SEMICOLON", line);
            forward ++;
            begin = forward;
            state = 0;
            break;

        //Tokenize :, :=, ( and )
        case 23:
            if (ch == '='){
                state = 24;
            }
            else{
                Tokenize(begin, forward - 1, "TK_COLON", line);
                begin = forward;
                state = 0;
            }
            break;

        case 24:
            Tokenize(begin, forward, "TK_ASSIGNOP", line);
            forward ++;
            begin = forward;
            state = 0;
            break;

        case 25:
            Tokenize(begin, forward, "TK_BO", line);
            forward ++;
            begin = forward;
            state = 0;
            break;

        case 26:
            Tokenize(begin, forward, "TK_BC", line);
            forward ++;
            begin = forward;
            state = 0;
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
                Tokenize(begin, forward - 1, "TK_GT", line);
                begin = forward;
                state = 0;
            }
            break;
        case 27:
            Tokenize(begin, forward - 1, "TK_GE", line);
            begin = forward;
            state = 0;
            break;
        
        case 29:
            if(ch=='>'){
                forward++;
                state=30;
            }
            else {
            Tokenize(begin, forward - 1, "TK_ENDDEF", line);
            begin = forward;
            state = 0;
            }
            break;
            
        case 30:
            Tokenize(begin, forward - 1, "TK_DRIVERENDDEF", line);
            begin = forward;
            state = 0;
            break;

        case 31:
            Tokenize(begin, forward - 1, "TK_SQBO", line);
            // forward ++;
            begin = forward;
            state = 0;
            break;

        // tokenize close sq bracket
        case 32:
            Tokenize(begin, forward - 1, "TK_SQBC", line);
            // forward ++;
            begin = forward;
            state = 0;
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
            Tokenize(begin, forward, "TK_NE", line);
            forward++;
            begin = forward;
            state = 0;
            break;

        // tokenize div operator
        case 35:
            Tokenize(begin, forward - 1, "TK_DIV", line);
            // forward ++;
            begin = forward;
            state = 0;
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
            Tokenize(begin, forward, "TK_RANGEOP", line);
            forward++;
            begin = forward;
            state = 0;
            break;

        // updates line no
        case 38:
            line++;
            state = 0;
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
                state = 0;
                break;
            }

        default:
            printf("ERROR: State does not exist\n");
            exit(1);
        }
        // character update
        if (forward < 32)
            ch = buffer1[forward];
        else
        {
            // case when forward in buf2 nd begin in buf1
            forward %= 32;
            ch = buffer2[forward];
        }
    }
}

/* >>>>>>>>>>>>>>>>>>>>>>>>> DRIVER CODE <<<<<<<<<<<<<<<<<<<<<<<<<<*/

int main()
{
    init_hash();
    readBuffer();
    strcpy(buffer1, buffer2);
    readBuffer();
    // lexer();

    printf("%s\n", buffer1);
    printf("%s\n", buffer2);

    return 0;
}