#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc != 4) {
		printf("Invalid call. Program should be called as: \n%s <discname> <old file name> <new file name>\n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[1]))
		exit(1);

	rename_vfs_file(argv[1], argv[2], argv[3]);
	
	return 0;
}