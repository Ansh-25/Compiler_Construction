#include "lexer.h"
#include "parserdef.h"

#define NO_RULES 141
#define NO_TERMS 51
#define NO_NONTERMS 70
#define MAXTERMLEN 31

ListNode* grammar[NO_RULES];
ListNode* first[NO_NONTERMS];
ListNode* follow[NO_NONTERMS];
int parseTable [NO_TERMS][NO_NONTERMS];

non_terminal maptoenum(char* s) {
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
    init_hash();
    while (!feof(fp)) {
        ch = fgetc(fp);
        if (ch == ' ' || ch == '\n') {
            grammarchar gc ;
            if (currinput[0] == '<') {
                gc.t = NONTERMINAL;
                for (int i = 0; i < MAXTERMLEN; i ++) temp[i] = '\0';
                strncpy(temp, currinput + 1, index - 2);
                gc.g.nt = maptoenum(temp);
            }
            else {
                gc.t = TERMINAL;
                if (strcmp(currinput,"EPS") == 0) {
                    printf("EPS\n");
                    gc.g.t = EPS;
                }
                else 
                    gc.g.t = search_hash(currinput);
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