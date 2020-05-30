#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_helpfile("cp_to_vfs_help.txt");
		exit(0);
	}

	if (argc != 3) {
		printf("Invalid call. For more informations type '%s --help'\n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[1]))
		exit(1);

	remove_vfs_file(argv[1], argv[2]);
	
	return 0;
}