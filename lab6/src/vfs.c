#include "vfs.h"

// PROGRAMY:
void create_vfs(char * filename, size_t size) {
	vdisc disc = create_empty_vdisc(filename);

	// Wypelnianie superbloku:
	struct super_block super_block;
	strcpy(super_block.vfs_name, filename);
	strcpy(super_block.vfs_key, VFS_KEY);
	super_block.fs_size = size;
	super_block.block_size = BLOCK_SIZE;
	super_block.inodes_nr = count_possible_inodes(size);
	super_block.used_inodes_nr = 0;
	// Zapis superbloku do wirtualnego dysku:
	fwrite(&super_block, sizeof(super_block), 1, disc);

	// Zapis i-wezlow na dysk:
	struct inode inode;
	inode.flags = 0;
	for (int i = 0; i < super_block.inodes_nr; ++i) {
		inode.index = i + 1;
		fwrite(&inode, sizeof(inode), 1, disc);
	}

	// Stworzenie pozostalej przestrzeni dyskowej:
	char zeros[BLOCK_SIZE];
	memset(zeros, 0, sizeof(zeros));
	for (int i = 0; i < super_block.inodes_nr; ++i)
		fwrite(zeros, sizeof(zeros), 1, disc);

	leave_vdisc(disc);
}


void remove_whole_vfs(char * filename) {
	if (remove(filename)) {
		printf("Cannot remove %s.\n", filename);
	}
}


void ls_vfs(char * discname, bool with_size) {
	vdisc disc = enter_vdisc(discname);

	struct super_block super_block = get_super_block(disc);
	struct inode inode;
	for (int i = 0; i < super_block.inodes_nr; ++i) {
		inode = get_nth_inode(disc, i + 1);
		if ((inode.flags & IN_USE) && (inode.flags & IS_START)) {
			printf("%s", inode.name);
			if (with_size)
				printf("\t\t%ld", (inode.file_inodes_nr) * (super_block.block_size));
			printf("\n");
		}
	}

	leave_vdisc(disc);
}

void cp_to_vfs(char * discname, char * filename) {
	vdisc disc = enter_vdisc(discname);

	if (is_name_present(disc, filename)) {
		char opt = 0;
		printf("Filename %s is already in use in virtual file system.\n", filename);
		printf("Would you like to overwrite this file? [y/n] ");
		scanf("%c", &opt);
		while ((getchar()) != '\n');
		if (opt == 'n')
			return;
		else if (opt != 'y') {
			printf("Not recognized option: %c. Aborting copying.\n", opt);
			return;
		}
		else if (opt == 'y') {
			remove_vfs_file(discname, filename);
		}
	}
	struct stat file_stat;
	stat(filename, &file_stat);
	size_t file_size = file_stat.st_size;

	if (free_space_in_vfs(disc) < file_size) {
		printf("Not enough free space in virtual file system to carry file %s.\n", filename);
		return;
	}

	struct super_block super_block = get_super_block(disc);
	int required_inodes_nr = ceil((double) file_size / (double) super_block.block_size);

	struct inode current_inode, prev_inode;
	FILE * unix_file = fopen(filename, "r");
	long file_offset = 0;
	for (int i = 0; i < required_inodes_nr; ++i) {
		prev_inode = current_inode;
		current_inode = find_next_free_inode_from_index(disc, i + 1);
		prev_inode.next_node = current_inode.index;

		if (i == 0) {
			current_inode.flags = IN_USE | IS_START;
			current_inode.file_inodes_nr = required_inodes_nr;
		}
		else
			current_inode.flags = IN_USE;

		strcpy(current_inode.name, filename);
		file_offset = i * BLOCK_SIZE;
		current_inode.used_bytes = copy_binary_block_to_vfs(unix_file, file_offset, disc, current_inode);

		super_block.used_inodes_nr += 1;
		save_inode(disc, prev_inode);
		save_inode(disc, current_inode);
		save_super_block(disc, super_block);
	}
	save_inode(disc, current_inode);

	fclose(unix_file);
	save_super_block(disc, super_block);
	leave_vdisc(disc);
}

bool cp_from_vfs(char * discname, char * filename) {
	vdisc disc = enter_vdisc(discname);

	if (!is_name_present(disc, filename)) {
		printf("No file %s found in virtual file system.\n", filename);
		return false;
	}

	if (access(filename, F_OK) == 0) {
		char opt;
		printf("The file %s already exists in your operating system.\n", filename);
		printf("Would you like to overwrite this file? [y/n] ");
		scanf("%c", &opt);
		while ((getchar()) != '\n');
		if (opt == 'n')
			return false;
		else if (opt != 'y') {
			printf("Not recognized option: %c. Aborting copying.\n", opt);
			return false;
		}
		if (access(filename, W_OK) != 0) {
			printf("No permission to write in file %s.\n", filename);
			return false;
		}
	}

	FILE * unix_file = fopen(filename, "w");

	struct super_block super_block = get_super_block(disc);
	long pos = 0;
	struct inode current_inode = find_first_inode_of_file(disc, filename);
	int file_inodes_nr = current_inode.file_inodes_nr;
	for (int i = 0; i < file_inodes_nr; ++i) {
		copy_binary_block_from_vfs(unix_file, pos, disc, current_inode);
		pos += super_block.block_size;
		current_inode = get_nth_inode(disc, current_inode.next_node);
	}

	fclose(unix_file);
	leave_vdisc(disc);
	return true;
}

void rename_vfs_file(char * discname, char * old_name, char * new_name) {
	vdisc disc = enter_vdisc(discname);
	if (sizeof(new_name) > MAX_NAME_LENGTH) {
		printf("New name is too long. File name should be max 128 characters long.\n");
		return;
	}
	if (is_name_present(disc, new_name)) {
		printf("Name %s is already in use. Please try another name.\n", new_name);
		return;
	}
	struct inode inode = find_first_inode_of_file(disc, old_name);
	if (inode.index == -1)
		return;

	strcpy(inode.name, new_name);
	save_inode(disc, inode);
	leave_vdisc(disc);
}

bool remove_vfs_file(char * discname, char * filename) {
	vdisc disc = enter_vdisc(discname);

	struct inode start_inode = find_first_inode_of_file(disc, filename);
	if (start_inode.index == -1)
		return false;

	struct inode current_inode = start_inode;
	for (int i = 0; i < start_inode.file_inodes_nr; ++i) {
		current_inode.flags = 0;
		save_inode(disc, current_inode);
		current_inode = get_nth_inode(disc, current_inode.next_node);
	}

	leave_vdisc(disc);
	return true;
}

void vfs_mem_map(char * discname) {
	vdisc disc = enter_vdisc(discname);

	struct super_block super_block = get_super_block(disc);
	struct inode inode;
	for (int i = 0; i < super_block.inodes_nr; ++i) {
		inode = get_nth_inode(disc, i + 1);

		printf("%d\t", inode.index);

		if (inode.flags & IS_START) printf("1");
		else printf("0");
		if (inode.flags & IN_USE) printf("1");
		else printf("0");

		if (inode.flags & IN_USE)
			printf("\t%s", inode.name);
		printf("\n");
	}

	leave_vdisc(disc);
}

// VFS pomocnicze:
// Wszystkie pliki i dyski przekazywane tym funkcjom powinny byc juz otwarte
bool check_vdisc(char * discname) {
	if (access(discname, F_OK)) {
		printf("Cannot find file %s.\n", discname);
		return false;
	}

	if (access(discname, R_OK | W_OK)) {
		printf("No permission to read or write in disc %s.\n", discname);
		return false;
	}

	vdisc disc = enter_vdisc(discname);

	struct super_block super_block = get_super_block(disc);
	if (strcmp(super_block.vfs_key, VFS_KEY) != 0) {
		printf("File %s is not a virtual file system.\n", discname);
		return false;
	}

	leave_vdisc(disc);
	return true;
}

bool check_unix_file_to_copy_from(char * filename) {
	if (access(filename, F_OK)) {
		printf("Cannot find file %s.\n", filename);
		return false;
	}

	if (access(filename, R_OK)) {
		printf("No permission to read from file %s.\n", filename);
		return false;
	}

	return true;
}

struct inode find_first_inode_of_file(vdisc disc, char * filename) {
	struct super_block super_block = get_super_block(disc);
	struct inode current_inode;
	for (int i = 1; i <= get_super_block(disc).inodes_nr; ++i) {
		current_inode = get_nth_inode(disc, i);
		if (current_inode.flags == (IN_USE | IS_START) && strcmp(current_inode.name, filename) == 0)
			return current_inode;
	}
	printf("No file %s found in virtual file system.\n", filename);
	current_inode.index = -1;
	return current_inode;
}

size_t find_nth_memory_block_pos(vdisc disc, int index) {
	return (sizeof(struct super_block) + sizeof(struct inode) * (get_super_block(disc).inodes_nr) 
		+ (index - 1) * BLOCK_SIZE);
}

size_t find_nth_inode_pos(vdisc disc, int index) {
	return (sizeof(struct super_block) + (index - 1) * sizeof(struct inode));
}

size_t copy_binary_block_to_vfs(FILE * unix_file, long file_offset, vdisc disc, struct inode inode) {
	fseek(unix_file, file_offset, SEEK_SET);
	unsigned char bytes[BLOCK_SIZE];
	size_t read_bytes = fread((void *) bytes, 1, BLOCK_SIZE, unix_file);

	fseek(disc, find_nth_memory_block_pos(disc, inode.index), SEEK_SET);
	fwrite((void *) bytes, 1, read_bytes, disc);

	return read_bytes;
}

size_t copy_binary_block_from_vfs(FILE * unix_file, long file_offset, vdisc disc, struct inode inode) {
	size_t bytes_to_copy = inode.used_bytes;

	fseek(unix_file, file_offset, SEEK_SET);
	fseek(disc, find_nth_memory_block_pos(disc, inode.index), SEEK_SET);

	unsigned char bytes[BLOCK_SIZE];
	fread((void *) bytes, 1, bytes_to_copy, disc);
	fwrite((void *) bytes, 1, bytes_to_copy, unix_file);

	return bytes_to_copy;
}

void save_inode(vdisc disc, struct inode node) {
	long offset = find_nth_inode_pos(disc, node.index);
	fseek(disc, offset, SEEK_SET);
	fwrite((void *) &node, sizeof(struct inode), 1, disc);
}

void save_super_block(vdisc disc, struct super_block sb) {
	fseek(disc, 0, SEEK_SET);
	fwrite((void *) &sb, sizeof(sb), 1, disc);
}

struct inode find_next_free_inode_from_index(vdisc disc, int index) {
	struct inode inode;
	int inodes_nr = get_super_block(disc).inodes_nr;

	if (index > inodes_nr) {
		printf("Given index must not be greater than summary count of inodes.\n");
		inode.index = -1;
		return inode;
	}
	for (int i = index; i < inodes_nr; ++i ) {
		inode = get_nth_inode(disc, i);
		if ( !(inode.flags & IN_USE) )
			return inode;
	}
	printf("No free inodes in %s.\n", get_super_block(disc).vfs_name);
	inode.index = -1;
	return inode;
}

size_t free_space_in_vfs(vdisc disc) {
	struct super_block super_block = get_super_block(disc);
	return (super_block.inodes_nr - super_block.used_inodes_nr) * super_block.block_size;
}


bool is_name_present(vdisc disc, char * name) {
	struct super_block super_block = get_super_block(disc);
	struct inode inode;
	for (int i = 0; i < super_block.inodes_nr; ++i) {
		inode = get_nth_inode(disc, i + 1);
		if ((inode.flags & IN_USE) && (inode.flags & IS_START) && (strcmp(inode.name, name) == 0))
			return true;
	}
	return false;
}

vdisc create_empty_vdisc(char * discname) {
	vdisc d = fopen(discname, "w+");
	if (!d) {
		printf("Unable to create empty file %s.\n", discname);
		exit(1);
	}
	return d;
}

vdisc enter_vdisc(char * discname) {
	vdisc d = fopen(discname, "r+");
	if (!d) {
		printf("Unable to open file %s.\n", discname);
		exit(1);
	}
	return d;
}

void leave_vdisc(vdisc d) {
	fclose(d);
}

struct super_block get_super_block(vdisc disc) {
	struct super_block super_block;
	fseek(disc, 0, SEEK_SET);
	fread((void *) &super_block, sizeof(super_block), 1, disc);
	return super_block;
}

struct inode get_nth_inode(vdisc disc, unsigned int index) {
	unsigned int pos = sizeof(struct super_block) + (index - 1) * sizeof(struct inode);
	fseek(disc, pos, SEEK_SET);
	struct inode inode;
	fread((void *) &inode, sizeof(struct inode), 1, disc);
	return inode;
}

unsigned int count_possible_inodes(size_t fs_size) {
	return ( (fs_size - sizeof(struct super_block) ) / (sizeof(struct inode) + BLOCK_SIZE) );
}

bool ask_y_n() {
	char opt = 0;
	scanf("%c", &opt);
	while ((getchar()) != '\n');
	if (opt == 'n')
		return false;
	else if (opt != 'y') {
		printf("Not recognized option: %c.\n", opt);
		return false;
	}
	else {
		return true;;
	}	
}

void print_helpfile(char * filename) {
	char command[sizeof("more $(find .. -name ") + sizeof(filename) + sizeof(")")] = "";
	strcat(command, "more $(find .. -name ");
	strcat(command, filename);
	strcat(command, ")");

	system(command);
}

void create_sized_file(char * filename, size_t size_kB) {
	FILE * file = fopen(filename, "w");
	if (!file) {
		printf("Error occured while opening file %s.\n", filename);
		return;
	}

	unsigned char mem_block[1024];
	memset(mem_block, 'a', sizeof(mem_block));
	
	for (int i = 0; i < size_kB; ++i) {
		fwrite(mem_block, sizeof(mem_block), 1, file);
	}

	fclose(file);
}