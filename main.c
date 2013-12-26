#include "global.h"
#include "util.h"
#include "lex.h"
#include "parser.h"

int line_no = 0;

FILE *source = NULL;
FILE *listing = NULL;
FILE *code = NULL;

static void print_usage() {
	printf("Usage : twt <file-name>\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		print_usage();
		exit(1);
	}

	listing = stdout;

	source = fopen(argv[1], "r");
	if (source == NULL) {
		fprintf(stdout, " [Error] No such file..");
		exit(1);
	}

	// lex 'TEST'
//	while(get_token() != ENDFILE) ;
	
	TreeNode *t = parse();
	print_tree(t);

	return 0;
}
