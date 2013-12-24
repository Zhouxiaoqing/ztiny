#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESERVED_WORDS 8;

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

#endif
