
void printTree(TreeNode* root) {
    if(root==NULL) return;
    TreeNode* curr = root->child;
    printTree(root->child);
    if(root->t==TERMINAL && root->val.t.type==TK_NUM) printf("Terminal %s with token value %d\n", maptokentostring(root->val.t.type),root->val.t.val.integer);
    else if(root->t==TERMINAL && root->val.t.type==TK_RNUM) {("Terminal %s with token value %lf\n", maptokentostring(root->val.t.type),root->val.t.val.decimal);}
    else if(root->t==TERMINAL) {("Terminal %s with token value %d\n", maptokentostring(root->val.t.type),root->val.t.val.identifier);}
    else printf("Non-Terminal %s\n",mapnttostring(root->val.nt));
    curr=curr->sibling;
    for(;curr!=NULL; curr=curr->sibling){
        printTree(curr);
    }
}
