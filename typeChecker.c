#include "symbolTableDef.h"

MainTableEntry** SymbolTable;
ModuleTableEntry** curr; //set curr at every module node

/* how to handle if module reuse before module definition */

typechecker(ASTNode* astNode){
    int c = astNode->label;
    switch(c){
        

    }
}