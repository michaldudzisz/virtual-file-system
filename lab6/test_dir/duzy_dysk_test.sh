#! /usr/bin/sh
# Skrypt dziala po wykonaniu make install w katalogu lab6/src
# Inaczej należy dodać "./" przed każdy program

create_vfs dysk 75000 # dysk o rozmiarze 75 MB, czyli ok 33 000 i-wezlow

create_sized_file plik_60_mb 65920 # 60 MB

create_sized_file plik_4_kb 4 # 4 kB
touch pusty_plik1
touch pusty_plik2
touch pusty_plik3

cp_to_vfs dysk plik_60_mb plik_4_kb pusty_plik1 pusty_plik2 pusty_plik3
rename_vfs_file dysk pusty_plik2 nowa_nazwa_pustego_pliku2
remove_vfs_file dysk plik_4_kb

ls_vfs dysk -il