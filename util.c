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
			fprintf(listing, " reverse words : %s \n", token_string);
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
	while ( (s[i] = str[i]) != '\0' ) {
		
		i++;
	}
	s[i] = '\0';

	return s;
}

