#include "lexer_for_testing.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 58
#define NO_NONTERMS 72
#define MAXTERMLEN 50

struct ListNode* grammar[NO_RULES];
struct ListNode* first[NO_NONTERMS];
struct ListNode* follow[NO_NONTERMS];
int parseTable [NO_NONTERMS][NO_TERMS - 1];

tokentype mapttoenum(char* s) {
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
}

non_terminal mapnttoenum(char* s) {
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
    else if (strcmp(s,"array_element_for_print") == 0) return array_element_for_print;
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
    else return 0;
}

char* mapttokentostring(tokentype t) {
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
}

char* mapnttostring(non_terminal n) {
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
    if (n == array_element_for_print) return "array_element_for_print";
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
}

void loadgrammar(char* filename) {
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
    for (int i = 0; i < 2 * MAXTERMLEN; i ++) grammar_buff[i] == '\0';
    while (!feof(fp)) {
        fread(grammar_buff, 1, 2 * MAXTERMLEN - 1, fp);
        buff_index = 0;
        while (buff_index < 2 * MAXTERMLEN - 1) {
            ch = grammar_buff[buff_index];
            if (ch == ' ' || ch == '\n') {
                grammarchar gc ;
                if (currinput[0] == '<') {
                    gc.t = NONTERMINAL;
                    for (int i = 0; i < MAXTERMLEN; i ++) temp[i] = '\0';
                    strncpy(temp, currinput + 1, input_index - 2);
                    gc.g.nt = mapnttoenum(temp);
                }
                else {
                    gc.t = TERMINAL;
                    gc.g.t = mapttoenum(currinput);
                }
                grammar[rule] = insertlast(grammar[rule],gc);
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
}

int derivesepsilon(grammarchar gc){
    if (gc.t == TERMINAL)
        return 0;
    if (first[gc.g.nt] == NULL)
        return 0;
    ListNode* curr = first[gc.g.nt];
    while (curr -> next != NULL)
        curr = curr -> next;
    if (curr -> val.g.t == EPS)
        return 1;
    return 0;
}

void computeFirst (non_terminal A)
{
    if (first[A]!=NULL) return;
    for(int i=0; i<NO_RULES; i++){
        if (grammar[i] -> val.g.nt == A){
            ListNode* rhs_current = grammar[i] -> next;
            if (rhs_current -> val.t == TERMINAL){
                first[A] = insertlast(first[A], rhs_current -> val);
            }

            else if (rhs_current -> val.t == NONTERMINAL){
                computeFirst(rhs_current -> val.g.nt);
                ListNode* first_rhs_current = first[rhs_current -> val.g.nt];
                while (first_rhs_current != NULL) {
                    if (first_rhs_current -> val.g.t != EPS)
                        first[A] = insertlast(first[A], first_rhs_current -> val);
                    first_rhs_current = first_rhs_current -> next;
                }
                while (derivesepsilon(rhs_current -> val)){
                    rhs_current = rhs_current -> next;
                    if (rhs_current == NULL){
                        grammarchar add_eps;
                        add_eps.t = TERMINAL;
                        add_eps.g.t = EPS;
                        first[A] = insertlast(first[A], add_eps);
                    }
                    else if (rhs_current -> val.t == TERMINAL){
                        first[A] = insertlast(first[A], rhs_current -> val);
                    }
                    else{
                        computeFirst(rhs_current -> val.g.nt);
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

int contains(ListNode* ln, tokentype term) {
    if (ln == NULL)
        return 0;
    for (ListNode* curr = ln; curr != NULL; curr = curr -> next) {
        if (curr -> val.t == TERMINAL && curr -> val.g.t == term)
            return 1;
    }
    return 0;
}

void computeFollow(non_terminal A) {
    if (follow[A]!=NULL) return;
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

                    while(derivesepsilon(rhs_current -> val)){
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
        computeFirst(i);
    grammarchar gc;
    gc.t = TERMINAL;
    gc.g.t = TK_EOF;
    follow[program] = insertlast(follow[program], gc);
    for(int i=0; i<NO_NONTERMS; i++)
        computeFollow(i);
}

void createParseTable(){
    int s = (NO_TERMS-1)*(NO_NONTERMS);
    memset(parseTable,-1,s*sizeof(int));

    for(int i=0;i<NO_RULES;i++){
        struct ListNode* A = grammar[i];
        struct ListNode* B = grammar[i];
        int a = A->val.g.nt;
        int b = -1;
        do{
            B = B->next;
            if(B==NULL || (B->val.t==TERMINAL && B->val.g.t==EPS)){
                struct ListNode* C = follow[a];
                while(C!=NULL){
                    int c = C->val.g.t; 
                    parseTable[a][c] = i;
                    C = C->next;
                }
                break;
            }

            if(B->val.t==TERMINAL){
                b = B->val.g.t; 
                parseTable[a][b] = i;
            }
            else{
                b = B->val.g.nt;
                struct ListNode* C = first[b];
                while(C!=NULL){
                    int c = C->val.g.t; 
                    if(C->val.g.t!=EPS)
                        parseTable[a][c] = i;
                    C = C->next;
                }
            }
        } while(derivesepsilon(B->val)); //derivesEpsilon returns 0 on input -1
    }
}