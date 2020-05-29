#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc < 3) {
		printf("Invalid call. Program should be called as: \n%s <discname> <file1> [<file2> ...]\n", argv[0]);
		exit(1);
	}

	if (!check_vdisc(argv[1]))
		exit(1);

	for (int i = 0; i < argc - 2; ++i) {
		if (!cp_from_vfs(argv[1], argv[i + 2]))
			printf("File %s has not been copied.\n", argv[i + 2]);
	}
	
	return 0;
}