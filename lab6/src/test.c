#include "vfs.h"


int main() {
	create_vfs("vfs_file", 10000);
	cp_to_vfs("vfs_file", "plik.txt");
	rename_vfs_file("vfs_file", "plik.txt", "lolz.txt");
	cp_from_vfs("vfs_file", "lolz.txt");
	ls_vfs("vfs_file", true);
	//printf("wielkosc inoda: %ld\n", sizeof(struct inode));

	//remove_vfs("vfs_file");
	return 0;
}
