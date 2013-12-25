#include "global.h"
#include "lex.h"

#include "parser.h"

static TokenType token;

static void
_syntax_error(char *s) {
	fprintf(listing, " [Syntax-Error] token:%d -- %s\n", token, s);
	return;
}

// 匹配掉 `token'
static void
_match(TokenType expected) {
	if (expected == token)
		token = get_token();
	else {
		_syntax_error("Do not match expected token.");
		exit(0);
	}
}


// declarations
static TreeNode *_stmt_sequence(void);
static TreeNode *_stmt(void);
//static TreeNode *_addop(void);				// 由于非终结符 addop 只能匹配算符, 故在此舍去
//static TreeNode *_mulop(void);				// 由于非终结符 mulop 只能匹配算符, 故在此舍去

static TreeNode *
_stmt_sequence(void) {

}

static TreeNode *
_stmt(void) {

}

TreeNode *parse() {
	TreeNode *t = NULL;
	token = get_token();
	t = NULL;
	if (t == NULL)
		fprintf(listing, " [Error] parse error.");
	return t;
}
