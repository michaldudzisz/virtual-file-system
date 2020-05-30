CC = gcc
CFLAGS = -g -lm
objects = create_vfs ls_vfs cp_to_vfs cp_from_vfs remove_vfs_file rename_vfs_file drop_vfs create_sized_file


all: $(objects) vfs.o

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

create_sized_file: vfs.o create_sized_file.c
	$(CC) create_sized_file.c vfs.o -o $@ $(CFLAGS)

vfs.o: vfs.h vfs.c
	$(CC) vfs.c -c -o $@ $(CFLAGS)

clean:
	@rm *.o

install: $(objects) vfs.o
	@ rm *.o
	@ echo "Copying executables to /usr/local/bin/"
	@ sudo cp $(objects) /usr/local/bin/
	@ sudo mkdir -p /usr/local/src/vfs/src
	@ sudo mkdir -p /usr/local/src/vfs/help_files
	@ echo "Copying files to /usr/local/src/vfs/"
	@ sudo cp vfs.h vfs.c cp_from_vfs.c cp_to_vfs.c ls_vfs.c create_vfs.c rename_vfs_file.c \
	remove_vfs_file.c drop_vfs.c create_sized_file.c makefile /usr/local/src/vfs/src
	@ sudo cp -r ../help_files /usr/local/src/vfs/
	@ echo "Removing executables from `pwd`"
	@ rm $(objects)
	@ echo "Installation complete. You can remove vfs files from: "
	@ cd .. && pwd 
