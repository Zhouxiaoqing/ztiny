#include <stdio.h>


static void print_usage() {
	printf("Usage : \n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		print_usage();
		return 0;
	}

	return 0;
}
