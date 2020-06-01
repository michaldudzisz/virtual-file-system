#! /usr/bin/sh
# Skrypt dziala po wykonaniu make install w katalogu lab6/src
# Inaczej należy dodać ścieżkę do każdego programu przed jego nazwę przy wywołaniu

create_vfs dysk 45 # dysk o rozmiarze 45 kB, czyli ok 20 i-wezlow

create_sized_file plik1 6 # 6 kB
create_sized_file plik2 12 # 12 kB
create_sized_file plik3 8 # 8 kB
create_sized_file plik4 10 # 10 kB

cp_to_vfs dysk plik1 plik2 plik3
remove_vfs_file dysk plik1 plik3
cp_to_vfs dysk plik4

ls_vfs dysk -il