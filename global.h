#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESERVED_WORDS 8

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef enum {
	ENDFILE, ERROR, 
	// reserved words
	IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
	// multi chars token
	ID, NUM,
	// special symbols
	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;

extern FILE *source;
extern FILE *listing;
extern FILE *code;

extern int line_no;

typedef enum {
	StmtK,
	ExpK
} NodeKind;

// for `StmtK'
typedef enum {
	IfK,
	RepeatK,
	AssignK,
	ReadK,
	WriteK
}StmtKind;

// for `ExpK'
typedef enum {
	OpK,				// op exp  --  ???
	ConstK,				// const exp
	IdK				// an identifier exp
}ExpKind;

// in this system , there is 3 types.
typedef enum {
	Void,
	Integer,
	Boolean
}ExpType;

#define MAX_CHILDREN 3

typedef struct _TreeNode {
	struct _TreeNode *child[MAX_CHILDREN];
	struct _TreeNode *sibling;
	int line_no;

	NodeKind node_kind;
	union {
		StmtKind sk;
		ExpKind ek;
	}kind;
	union {
		TokenType op;
		int val;
		char *name;
	}attr;
	ExpType type;					// for type check
}TreeNode;

#endif
