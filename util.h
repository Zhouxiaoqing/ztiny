#ifndef _UTIL_H_
#define _UTIL_H_

void 
print_token(TokenType, const char *);

char *
copy_string(const char *);


TreeNode *
new_stmt_node(StmtKind kind);

TreeNode *
new_exp_node(ExpKind kind);

void print_tree(TreeNode *t);

#endif
