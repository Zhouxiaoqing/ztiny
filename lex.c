#include "global.h"
#include "util.h"
#include "lex.h"

typedef enum {
	START, 					// 0
	INASSIGN, 
	INCOMMENT, 
	INNUM, 
	INID, 
	DONE					// 5
} StateType;

char token_string[MAX_TOKEN_LEN+1];

#define MAX_LINE_BUFF 256

static char line_buff[MAX_LINE_BUFF];
static int line_pos = 0;
static int buff_size = 0;			// strlen(line_buff)

static int EOF_FLAG = 0;

static int 
get_next_char() {
	if (line_pos >= buff_size) {
		line_no ++;
		if (fgets(line_buff, MAX_LINE_BUFF-1, source)) {
			buff_size = strlen(line_buff);
			line_pos = 0;
			return line_buff[line_pos++];
		}else {
			// read nothing .. 
			EOF_FLAG = 1;
			return EOF;
		}
	}else {
		return line_buff[line_pos++];
	}
}

/* 由于最多只会多读一个char, 所以可以在此处 `--' */
static void 
unget_next_char() {
	if (!EOF_FLAG) {
		line_pos --;
	}
}

static struct {
	char *str;
	TokenType tok;
} reserved_words[MAX_RESERVED_WORDS] = {
	{"if", IF}, 
	{"then", THEN}, 
	{"else", ELSE}, 
	{"end", END}, 
	{"repeat", REPEAT}, 
	{"until", UNTIL}, 
	{"read", READ}, 
	{"write", WRITE}, 
};

static TokenType
_reserved_lookup(char *s) {
	int i = 0;
	while (i < MAX_RESERVED_WORDS) {
		if (0 == strcmp(reserved_words[i].str, s)){
			return reserved_words[i].tok;
		}
		i++;
	}

	return ID;
}

static int 
_is_digit(char c) {
	if (c >= '0' && c <= '9')
		return TRUE;
	return FALSE;
}

static int
_is_alpha(char c) {
	if ((c >= 'a' && c <= 'z') 
		|| (c >= 'A' && c <= 'Z'))
		return TRUE;
	return FALSE;
}

TokenType get_token(void) {
	int state = START;
	int token_string_pos = 0;			// for save: ID, NUM ...
	TokenType current_token;
	int save;

	while (state != DONE) {
		int c = get_next_char();

		save = TRUE;
		switch (state) {
			case START:
				// multi words.
				if (_is_digit((char)c))
					state = INNUM;
				else if (_is_alpha((char)c))
					state = INID;
				else if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
					save = FALSE;
				else if (c == ':') {
					save = FALSE;
					state = INASSIGN;
				}
				else if (c == '{') {
					save = FALSE;
					state = INCOMMENT;
				}
				// single word.
				else {
					state = DONE;
					switch (c) {
						case EOF:
							save = FALSE;
							current_token = ENDFILE;
							break;
						case '+':
							current_token = PLUS;
							break;
						case '-':
							current_token = MINUS;
							break;
						case '*':
							current_token = TIMES;
							break;
						case '/':
							current_token = OVER;
							break;
						case '=':
							current_token = EQ;
							break;
						case '<':
							current_token = LT;
							break;
						case '(':
							current_token = LPAREN;
							break;
						case ')':
							current_token = RPAREN;
							break;
						case ';':
							current_token = SEMI;
							break;
						default :
							current_token = ERROR;
							break;
					} // endof `switch'
				}
				break;
			case INCOMMENT:
				save = FALSE;
				if (c == EOF) {
					state = DONE;
					current_token = ENDFILE;
				}else if (c == '}')
					state = START;
				break;
			case INASSIGN: 
				state = DONE;
				if (c == '=')
					current_token = ASSIGN;
				else {
					unget_next_char();
					save = FALSE;
					current_token = ERROR;
				}
				break;
			case INNUM:
				if (!_is_digit(c)) {
					unget_next_char();
					save = FALSE;			// 退还的char不存
					state = DONE;
					current_token = NUM;
				}
				break;
			case INID:
				if (!_is_alpha(c)) {
					unget_next_char();
					save = FALSE;
					state = DONE;
					current_token = ID;//_reserved_lookup(token_string);
				}
				break;
			case DONE : // should never happen
			default :
				fprintf(listing, " [Error] lex BUG, current state = %d\n", state);
				state = DONE;
				current_token = ERROR;
				break;
		} // endof `switch'

		// do save ..
		if (save) {
			token_string[token_string_pos++] = (char)c;
		}
	}

	token_string[token_string_pos] = '\0';
	if (current_token == ID) {
		current_token = _reserved_lookup(token_string);
	}

	// for `TEST'
//	print_token(current_token, token_string);

	return current_token;
}
