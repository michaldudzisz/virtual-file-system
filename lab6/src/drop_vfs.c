#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_helpfile("cp_to_vfs_help.txt");
		exit(0);
	}

	if (argc != 2) {
		printf("Invalid call. For more informations type '%s --help'\n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[1]))
		exit(1);

	printf("Are you sure you want to delete whole disc %s? [y/n]\n", argv[1]);
	if (ask_y_n())
		remove_whole_vfs(argv[1]);
	
	return 0;
}

