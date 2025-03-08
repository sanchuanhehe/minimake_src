# Description: Makefile for minimake
# Author: Sanchuanhehe
# Date: 2025-03-08

build: build_dir/minimake

build_dir/minimake: build_dir/temp/main.o build_dir/temp/loger.o build_dir/temp
	gcc -o build_dir/minimake src/main.c build_dir/temp/loger.o -I include -Wall

build_dir/temp/loger.o: src/loger.c include/loger.h build_dir/temp
	gcc -c src/loger.c -o build_dir/temp/loger.o -I include -Wall

build_dir/temp/main.o: src/main.c include/loger.h build_dir/temp
	gcc -c src/main.c -o build_dir/temp/main.o -I include -Wall

build_dir:
	mkdir build_dir

build_dir/temp: build_dir
	mkdir build_dir/temp

clean:
	rm -rf build_dir