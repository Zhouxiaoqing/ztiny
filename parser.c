#include "global.h"
#include "lex.h"
#include "util.h"

#include "parser.h"

static TokenType token;

static void
_syntax_error(char *s) {
	fprintf(listing, " syntax error(line:%d) -- token:%d -- %s\n", line_no, token, s);
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

// 注意 attr 中的值的填入时机
// declarations
static TreeNode *_stmt_sequence(void);
static TreeNode *_stmt(void);
static TreeNode *_if_stmt(void);
static TreeNode *_repeat_stmt(void);
static TreeNode *_assign_stmt(void);
static TreeNode *_read_stmt(void);
static TreeNode *_write_stmt(void);
static TreeNode *_exp(void);
static TreeNode *_simple_exp(void);
static TreeNode *_term(void);
static TreeNode *_factor(void);

//static TreeNode *_comparison_op(void);			// 由于非终结符 comparisonop 只能匹配算符, 故在此舍去
//static TreeNode *_add_op(void);				// 由于非终结符 addop 只能匹配算符, 故在此舍去
//static TreeNode *_mul_op(void);				// 由于非终结符 mulop 只能匹配算符, 故在此舍去

static TreeNode *
_stmt_sequence(void) {
	TreeNode *t = _stmt();
	TreeNode *p = t;
	// when a `stmt' is end, the next token should not be tokens below here.
	// when they happened, it means the `program' is over or error.
	while (token != ENDFILE && token != END && token != UNTIL && token != ELSE) {
		_match(SEMI);
		TreeNode *q = _stmt();
		if (q != NULL) {
			if (p) {
				p->sibling = q;
				p = q;
			}else
				// 赋值运算 右结合
				t = p = q;
		}
	}
	return t;
}

static TreeNode *
_stmt(void) {
	TreeNode *t = NULL;

	switch (token) {
		case IF: t = _if_stmt(); break;
		case REPEAT: t = _repeat_stmt(); break;
		case ID: t = _assign_stmt(); break;
		case READ: t = _read_stmt(); break;
		case WRITE: t = _write_stmt(); break;
		default :
			_syntax_error(" Unknown Stmt type(Unexpected token).\n");
			print_token(token, token_string);
			token = get_token();					// 该处调用保证了不会一直处于递归错误中
			break;
	}

	return t;
}

static TreeNode *
_if_stmt(void) {
//     each node have 3 children, for IF-STMT:
// 0 : is EXP tree
// 1 : is stmt-sequence after THEN
// 2 : is stmt-sequence after ELSE
	TreeNode *t = new_stmt_node(IfK);
	_match(IF);
	if (t != NULL) t->child[0] = _exp();
	_match(THEN);
	if (t != NULL) t->child[1] = _stmt_sequence();
	if (token == ELSE) {
		_match(ELSE);
		if (t != NULL) t->child[2] = _stmt_sequence();
	}
	_match(END);
	return t;
}

static TreeNode *
_repeat_stmt(void) {
	_match(REPEAT);
	TreeNode *t = new_stmt_node(RepeatK);
	if (t != NULL) t->child[0] = _stmt_sequence();
	_match(UNTIL);
	if (t != NULL) t->child[1] = _exp();
	return t;
}

static TreeNode *
_assign_stmt(void) {
	TreeNode *t = new_stmt_node(AssignK);
	if (t != NULL && token == ID)
		t->attr.name = copy_string(token_string);
	_match(ID);
	_match(ASSIGN);
	if (t != NULL) t->child[0] = _exp();
	return t;
}

static TreeNode *
_read_stmt(void) {
	TreeNode *t = new_stmt_node(ReadK);
	_match(READ);
	if (t != NULL && token == ID)
		t->attr.name = copy_string(token_string);
	_match(ID);				// take care of token `ID' should be matched
	return t;
}

static TreeNode *
_write_stmt(void) {
	TreeNode *t = new_stmt_node(WriteK);
	_match(WRITE);
	if (t != NULL) t->child[0] = _exp();
	return t;
}


/// `OpK' represents comparison, addop, mulop kinds
// < && ==
static TreeNode *
_exp(void) {
// first of all, accept a simple_exp, then 
// if there is an op, generate an op-node and move this 
// simple_exp to op-node's child
	TreeNode *t = _simple_exp();
	if (token == LT || token == EQ) {
		TreeNode *p = new_exp_node(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		_match(token);
		if (t != NULL) t->child[1] = _simple_exp();
	}
	return t;
}

// + && -
static TreeNode *
_simple_exp(void) {
	TreeNode *t = _term();
	while (token == PLUS || token == MINUS) {
		TreeNode *q = new_exp_node(OpK);
		q->child[0] = t;
		q->attr.op = token;
		t = q;
		_match(token);
		t->child[1] = _term();
	}
	return t;
}

// * && /
static TreeNode *
_term(void) {
	TreeNode *t = _factor();
	while (token == TIMES || token == OVER) {
		TreeNode *q = new_exp_node(OpK);
		if (q != NULL) {
			q->child[0] = t;
			q->attr.op = token;
			t = q;
			_match(token);
			t->child[1] = _factor();
		}
	}
	return t;
}

// ID && NUM && ...
static TreeNode *
_factor(void) {
	TreeNode *t = NULL;
	switch(token) {
		case LPAREN:
			_match(LPAREN);
			t = _exp();
			_match(RPAREN);
			break;
		case NUM:
			t = new_exp_node(ConstK);
			t->attr.val = atoi(token_string);
			_match(NUM);
			break;
		case ID:
			t = new_exp_node(IdK);
			t->attr.name = copy_string(token_string);
			_match(ID);
			break;
		default :
			_syntax_error("Unexpected token.\n");
			print_token(token, token_string);
			token = get_token();
	}
	return t;
}

TreeNode *parse() {
	TreeNode *t = NULL;
	token = get_token();
	t = _stmt_sequence();
	if (t == NULL)
		fprintf(listing, " [Error] parse error.");
	return t;
}
