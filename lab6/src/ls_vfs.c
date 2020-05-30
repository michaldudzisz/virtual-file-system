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

	if (inodes_opt) // do memory map
		vfs_mem_map(argv[optind]);
	else if (l_files_opt) // list files with their sizes
		ls_vfs(argv[optind], true);
	else // list files
		ls_vfs(argv[optind], false);
		


/*	int start_ind = optind;
	for (optind; optind < argc; ++optind) {
		if (optind == start_ind) {
			if (!check_vdisc(argv[optind])) {
				printf("Type '%s --help' for more informations.\n", argv[0]);
				exit(1);
			}
		} else {
			printf("Given file: %s\n", argv[optind]);
		}

	}*/
	//char * discname = argv[1];

/*	for (int i = 1; i < argc; ++i) {
		cp_to_vfs(di);
	}*/

	return 0;
}