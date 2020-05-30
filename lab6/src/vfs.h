#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>

#define BLOCK_SIZE 2048
#define MAX_NAME_LENGTH 256

#define VFS_KEY "is_virtual_file_system"
#define VFS_KEY_LENGTH 23

#define IN_USE (1 << 0)
#define IS_START (1 << 1)

struct super_block {
	char vfs_name[MAX_NAME_LENGTH];
	char vfs_key[VFS_KEY_LENGTH];
	size_t fs_size;
	size_t block_size;
	unsigned int inodes_nr;
	unsigned int used_inodes_nr;
};

struct inode {
	char name[MAX_NAME_LENGTH];
	unsigned int index;
	unsigned int flags;
	int next_node; // -1, gdy nie ma nastepnego wezla
	unsigned int file_inodes_nr; // aktywne tylko gdy IS_START
	size_t used_bytes;
};

typedef FILE * vdisc;

void create_vfs(char * filename, size_t size);
void remove_whole_vfs(char * filename);
void cp_to_vfs(char * discname, char * filename);
bool cp_from_vfs(char * discname, char * filename);
void ls_vfs(char * discname, bool with_size);
void rename_vfs_file(char * discname, char * old_name, char * new_name);
bool remove_vfs_file(char * discname, char * filename);
void vfs_mem_map(char * discname);

bool check_vdisc(char * discname);
bool check_unix_file_to_copy_from(char * filename);

vdisc create_empty_vdisc(char * discname);
vdisc enter_vdisc(char * discname);
void leave_vdisc(vdisc d);

struct inode find_first_inode_of_file(vdisc disc, char * filename);
void save_super_block(vdisc disc, struct super_block sb);
void save_inode(vdisc disc, struct inode inode);
size_t copy_binary_block_to_vfs(FILE * unix_file, long file_offset, vdisc disc, struct inode inode);
size_t copy_binary_block_from_vfs(FILE * unix_file, long file_offset, vdisc disc, struct inode inode);
struct inode find_next_free_inode_from_index(vdisc disc, int index);
size_t free_space_in_vfs(vdisc disc);
bool is_name_present(vdisc disc, char * name);
struct super_block get_super_block(FILE * file);
struct inode get_nth_inode(vdisc disc, unsigned int index);
unsigned int count_possible_inodes(size_t fs_size);
bool ask_y_n();
void print_helpfile(char * filename);
void create_sized_file(char * filename, size_t size_kB);