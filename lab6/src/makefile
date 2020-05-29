CC = gcc
CFLAGS = -g -lm

all: create_vfs ls_vfs cp_to_vfs cp_from_vfs remove_vfs_file

drop_vfs: vfs.o drop_vfs.c
	$(CC) drop_vfs.c vfs.o -o $@ $(CFLAGS)

remove_vfs_file: vfs.o remove_vfs_file.c
	$(CC) remove_vfs_file.c vfs.o -o $@ $(CFLAGS)

rename_vfs_file: vfs.o rename_vfs_file.c
	$(CC) rename_vfs_file.c vfs.o -o $@ $(CFLAGS)

create_vfs: vfs.o create_vfs.c
	$(CC) create_vfs.c vfs.o -o $@ $(CFLAGS)

ls_vfs: vfs.o ls_vfs.c
	$(CC) ls_vfs.c vfs.o -o $@ $(CFLAGS)

cp_to_vfs: vfs.o cp_to_vfs.c
	$(CC) cp_to_vfs.c vfs.o -o $@ $(CFLAGS)

cp_from_vfs: vfs.o cp_from_vfs.c
	$(CC) cp_from_vfs.c vfs.o -o $@ $(CFLAGS)

test: vfs.o test.c
	$(CC) test.c vfs.o -o $@ $(CFLAGS)

vfs.o: vfs.h vfs.c
	$(CC) vfs.c -c -o $@ $(CFLAGS)

clean:
	@rm *.o