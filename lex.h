#ifndef _LEX_H_
#define _LEX_H_

#define MAX_TOKEN_LEN 40

extern char token_string[MAX_TOKEN_LEN+1];

TokenType get_token(void);

#endif
