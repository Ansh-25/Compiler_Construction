
void printTree(TreeNode* root) {
    if(root==NULL) return;
    TreeNode* curr = root->child;
    printTree(root->child);
    if(root.t==TERMINAL && tokentype==NUM) printf("Terminal %s\n"); //need mapping functoin
    else if(root.t==TERMINAL && tokentype==RNUM) {}
    else if(root.t==TERMINAL) {}
    else printf("Non-Terminal %s\n"); //need mapping function
    curr=curr->sibling;
    for(;curr!=NULL; curr=curr->sibling){
        printTree(curr);
    }
}
