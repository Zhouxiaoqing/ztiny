#include "global.h"
#include "util.h"

void 
print_token(TokenType type, const char *token_string) {
	switch(type) {
		case IF: 
		case THEN:
		case ELSE:
		case END:
		case REPEAT:
		case UNTIL:
		case READ:
		case WRITE:
			fprintf(listing, "Reverse words : %s \n", token_string);
			break;
		case ASSIGN: fprintf(listing, ":=\n"); break;
		case LT: fprintf(listing, "<\n"); break;
		case EQ: fprintf(listing, "=\n"); break;
		case LPAREN: fprintf(listing, "(\n"); break;
		case RPAREN: fprintf(listing, ")\n"); break;
		case SEMI: fprintf(listing, ";\n"); break;
		case PLUS: fprintf(listing, "+\n"); break;
		case MINUS: fprintf(listing, "-\n"); break;
		case TIMES: fprintf(listing, "*\n"); break;
		case OVER: fprintf(listing, "/\n"); break;
		case ENDFILE: fprintf(listing, "EOF\n"); break;
		case NUM:
			fprintf(listing, "NUM, val = %s \n", token_string);
			break;
		case ID:
			fprintf(listing, "ID, name = %s \n", token_string);
			break;
		case ERROR:
			fprintf(listing, "ERROR, token string : %s \n", token_string);
			break;
		default :
			fprintf(listing, "Unknow token.\n");
	}
}

char *
copy_string(const char *str) {
	if (!str) {
		return NULL;
	}

	char *s = (char *)malloc(strlen(str) + 1);
	int i = 0;
	while ((s[i] = str[i]) != '\0')
		i++;
	s[i] = '\0';

	return s;
}

TreeNode *
new_stmt_node(StmtKind kind) {
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	for(i=0; i<3; i++)
		t->child[i] = NULL;
	t->sibling = NULL;

	t->node_kind = StmtK;
	t->kind.sk = kind;
	t->line_no = line_no;

	return t;
}

TreeNode *
new_exp_node(ExpKind kind) {
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	for(i=0; i<3; i++)
		t->child[i] = NULL;
	t->sibling = NULL;

	t->node_kind = ExpK;
	t->kind.ek = kind;
	t->line_no = line_no;

	return t;
}

int ident_no = 0;

void _print_space() {
	int i = 0;
	for (i=0; i<ident_no; i++)
		fprintf(listing, " ");
}

void print_tree(TreeNode *t) {
	ident_no += 2;
	while(t != NULL) {
		_print_space();
	// 1. current node
		if (t->node_kind == StmtK) {
			switch(t->kind.sk) {
				case IfK:
					fprintf(listing, "If\n");
					break;
				case RepeatK:
					fprintf(listing, "Repeat\n");
					break;
				case AssignK:
					fprintf(listing, "Assign to : %s \n", t->attr.name);
					break;
				case ReadK:
					fprintf(listing, "Read : %s \n", t->attr.name);
					break;
				case WriteK:
					fprintf(listing, "Write \n");
					break;
				default:
					fprintf(listing, "Unknown Stmt Kind. \n");
					break;
			}
		}else if (t->node_kind == ExpK) {
			switch(t->kind.ek) {
				case OpK:
					fprintf(listing, "Op: ");
					print_token(t->attr.op, "\0");
					break;
				case ConstK:
					fprintf(listing, "Const: %d\n", t->attr.val);
					break;
				case IdK:
					fprintf(listing, "Id: %s\n", t->attr.name);
					break;
				default:
					fprintf(listing, "Unknown Exp Kind. \n");
					break;
			}
		}else
			fprintf(listing, "Unknown type of node.\n");

	// 2. current node children
		int i;
		for (i=0; i<3; i++)
			print_tree(t->child[i]);
	// 3. sibling
		t = t->sibling;
	}
	ident_no -= 2;
}

void free_tree(TreeNode *t) {
	if (t == NULL)
		return;
	else {
		int i;
		for (i=0; i<3; i++)
			free_tree(t->child[i]);
		TreeNode *s = t->sibling;
		free(t);			// really free
		free_tree(s);
	}
}
