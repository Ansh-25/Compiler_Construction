#include "lexer.h"
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
    if (strcmp(s,"SEMICOLON") == 0) return TK_SEMICOLON;
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

void loadgrammar(char* filename) {
    FILE* fp = fopen(filename,"r");
    if (!fp) {
        printf("Grammar file cannot be opnened\n");
        exit(1);
    }
    char ch;
    int index = 0, rule = 0;
    char currinput[MAXTERMLEN];
    char temp[MAXTERMLEN];
    for (int i = 0; i < MAXTERMLEN; i ++)
        currinput[i] = '\0';
    while (!feof(fp)) {
        ch = fgetc(fp);
        if (ch == ' ' || ch == '\n') {
            grammarchar gc ;
            if (currinput[0] == '<') {
                gc.t = NONTERMINAL;
                for (int i = 0; i < MAXTERMLEN; i ++) temp[i] = '\0';
                strncpy(temp, currinput + 1, index - 2);
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
            index = 0;
        }
        else {
            currinput[index] = ch;
            index ++;
        }
    }
}

int main(void) {
    loadgrammar("GrammarForParser");
    ListNode* curr;
    for (int i = 0; i < NO_RULES; i ++) {
        printf("RULE: %d\n",i);
        curr = grammar[i];
        while (curr != NULL) {
            if (curr -> val.t == TERMINAL)
                printf("%d ", curr -> val.g.t);
            else
                printf("%d ", curr -> val.g.nt);
            curr = curr -> next;
        }
        printf("\n");
    }
    return 0;
}