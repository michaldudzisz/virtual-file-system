# virtual-file-system
Simple virtual file system written in plain C with basic managing files options and moving them from and to native Linux file system

May be compiled with make on Linux. Target "install" install available.

Main header file is `vfs.h` and implementation `vfs.c`. System gives set of programs:

* `cp_from_vfs VFS_DISC FILE...`
* `cp_to_vfs VFS_DISC FILE...`
* `create_sized_file NAME SIZE_KB`
* `create_vfs VFS_DISC SIZE`
* `drop_vfs VFS_DISC`
* `ls_vfs [OPTION]... VFS_DISC`
* `remove_vfs_file VFS_DISC FILE...`
* `rename_vfs_file VFS_DISC OLD_FILENAME NEW_FILENAME`

Which may be called as programs after build only or as commands after installation. More detailed info may be found in `help_files` direcotry or after typing `<commad> --help`.
