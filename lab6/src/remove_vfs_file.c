#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc != 3) {
		printf("Invalid call. Program should be called as: \n%s <discname> <ilename>\n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[1]))
		exit(1);

	remove_vfs_file(argv[1], argv[2]);
	
	return 0;
}