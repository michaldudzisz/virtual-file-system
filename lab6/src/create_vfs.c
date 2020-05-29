#include "vfs.h"

int main(int argc, char * argv[]) {

	if (argc != 3) {
		printf("Invalid call. Program should be called as:\n%s <discname> <size in kB>\n", argv[0]);
		exit(1);
	}

	if (sizeof(argv[1]) > MAX_NAME_LENGTH) {
		printf("Disc name is too long. Should not be longer than %d.\n", MAX_NAME_LENGTH);
		exit(1);
	}
	
	if (!access(argv[1], F_OK)) {
		printf("File named %s already exists. Would you like to overwrite it? [y/n]\n", argv[1]);
		if (!ask_y_n())
			exit(0);
		if (!access(argv[1], W_OK)) {
			printf("No permission to write in file %s.\n", argv[1]);
			exit(1);
		}
	}

	int size_kB = atoi(argv[2]);
	long size = 1024 * size_kB;

	create_vfs(argv[1], size);

	return 0;
}