#include "global.h"
#include "lex.h"

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

	fprintf(listing, " Start lex --> \n");

	while(get_token() != ENDFILE) ;

	fprintf(listing, "END\n");

	return 0;
}
