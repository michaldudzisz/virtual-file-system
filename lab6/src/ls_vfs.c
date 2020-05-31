#include "vfs.h"

int main(int argc, char * argv[]) {
	bool inodes_opt = false;
	bool l_files_opt = false;
	int option;

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_helpfile("ls_vfs_help.txt");
		exit(0);
	}

	while ((option = getopt(argc, argv, ":il")) != -1) { 
		switch (option) {
			case 'i':
				inodes_opt = true;
				break;
			case 'l':
				l_files_opt = true;
				break;
			case '?':
				printf("Unknown option: -%c.\n", optopt);
				printf("Invalid call of program. Type '%s --help' for more informations. \n", argv[0]);
				exit(1);
				break;
		}
	}

	if (argc - optind != 1) {
		printf("Invalid call. Type '%s --help' for more informations. \n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[optind])) {
		printf("Type '%s --help' for more informations.\n", argv[0]);
		exit(1);
	}

	if (inodes_opt && l_files_opt) {
		printf("File list:\n");
		ls_vfs(argv[optind], true);
		printf("\nInode list:\n");
		vfs_mem_map(argv[optind]);
	} 
	else if (inodes_opt) {
		vfs_mem_map(argv[optind]);
	}
	else if (l_files_opt)
		ls_vfs(argv[optind], true);
	else
		ls_vfs(argv[optind], false);

	return 0;
}