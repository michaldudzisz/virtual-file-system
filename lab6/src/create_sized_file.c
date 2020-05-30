#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_helpfile("create_sized_file_help.txt");
		exit(0);
	}

	if (argc != 3) {
		printf("Invalid call. For more informations type '%s --help'\n", argv[0]);
		exit(1);
	}

	size_t size_kB = atol(argv[2]);
	if (size_kB < 1) {
		printf("Error. Size must be a number greater than zero. Size given: %s\n", argv[2]);
		printf("For more informations type '%s --help'\n", argv[0]);
		exit(1);
	}

	create_sized_file(argv[1], size_kB);

	return 0;
}
