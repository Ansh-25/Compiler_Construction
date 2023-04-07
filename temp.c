        // case 67:
        //     makeAST(parserNode->child->sibling);
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ID;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = parserNode->child->sibling->addr;
        //     newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode1->label = ID_LIST;
        //     newNode1->tk = NULL;
        //     newNode1->child = newNode;
        //     newNode1->sibling = NULL;
        //     parserNode->addr = newNode1;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;


        // case 69:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 70:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;
        
        // case 71:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->addr;
        //     parserNode->child->addr->child = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 72:
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 73:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 74:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = UNARY_PLUS;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;
        
        // case 75:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = UNARY_MINUS;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 76:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ID;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 77:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = NUM;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 78:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = RNUM;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 79:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 80:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->child->sibling->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling->sibling);
        //     parserNode->addr = parserNode->child->sibling->sibling->addr;
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 81:
        //     break;

        // case 82:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 83:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 84:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 85:
        //     break;

        // case 86:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 87:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->child->sibling->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling->sibling);
        //     parserNode->addr = parserNode->child->sibling->sibling->addr;
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 88:
        //     break;

        // case 89:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 90:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->child->sibling->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling->sibling);
        //     parserNode->addr = parserNode->child->sibling->sibling->addr;
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 91:
        //     break;

        // case 92:
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 93:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = NUM;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 94:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = RNUM;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 95:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 96:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ID;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     makeAST(parserNode->child->sibling);
        //     newNode1 = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode1->label = ARRAY;
        //     newNode1->child = newNode;
        //     newNode1->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->addr = newNode1;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 97:
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child); 
        //     break;

        // case 98:
        //     parserNode->addr = NULL; //no index case
        //     break;

        // case 99:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ARR_INDEX1;
        //     newNode->child = parserNode->child->addr;
        //     newNode->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 100:
        //     makeAST(parserNode->child);
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ARR_INDEX2;
        //     newNode->child = parserNode->child->addr;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child);
        //     break;

        // case 101:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 102:
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 103:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 104:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->child->sibling->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling->sibling);
        //     parserNode->addr = parserNode->child->sibling->sibling->addr;
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 105:
        //     break;

        // case 106:
        //     makeAST(parserNode->child);
        //     parserNode->child->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 107:
        //     makeAST(parserNode->child);
        //     makeAST(parserNode->child->sibling);
        //     parserNode->child->addr->child = parserNode->addr;
        //     parserNode->child->addr->child->sibling = parserNode->child->sibling->addr;
        //     parserNode->child->sibling->sibling->addr = parserNode->child->addr;
        //     makeAST(parserNode->child->sibling->sibling);
        //     parserNode->addr = parserNode->child->sibling->sibling->addr;
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child);
        //     break;

        // case 108:
        //     break;

        // case 109:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = ID;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 110:
        //     makeAST(parserNode->child->sibling);
        //     parserNode->addr = parserNode->child->sibling->addr;
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 111:
        //     makeAST(parserNode->child);
        //     parserNode->addr = parserNode->child->addr;
        //     free(parserNode->child);
        //     break;

        // case 112:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = NUM;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 113:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = PLUS;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 114:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = MINUS;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 115:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = MUL;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 116:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = DIV;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 117:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = AND;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 118:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = OR;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 119:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = LT;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 120:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = LE;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 121:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = GT;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 122:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = GE;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 123:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = EQ;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;

        // case 124:
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = NE;
        //     newNode->tk = parserNode->child->val.t;
        //     newNode->child = NULL;
        //     newNode->sibling = NULL;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;     

        // case 125:
        //     makeAST(parserNode->child->sibling);
        //     makeAST(parserNode->child->sibling->sibling->sibling);
        //     newNode = (ASTNode*)malloc(sizeof(ASTNode));
        //     newNode->label = DECLARE;
        //     newNode->tk = NULL; 
        //     newNode->child = parserNode->child->sibling->addr;
        //     newNode->child->sibling = parserNode->child->sibling->sibling->sibling->addr;
        //     parserNode->addr = newNode;
        //     free(parserNode->child->sibling->sibling->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling->sibling->sibling);
        //     free(parserNode->child->sibling->sibling->sibling);
        //     free(parserNode->child->sibling->sibling->val.t);
        //     free(parserNode->child->sibling->sibling);
        //     free(parserNode->child->sibling);
        //     free(parserNode->child->val.t);
        //     free(parserNode->child);
        //     break;